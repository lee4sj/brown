%% Step 1: Load the images

% Sequences specified by { count, first index, name format, scale }
dir = '/course/cs143/data/proj5/';
sequences = {
    % Orthographic camera (required)
    { 51, 0, 'hotel.seq%d.png', 1 }

    % Perspective camera (extra credit)
    { 50, 1, 'liberty.%d.jpg', 0.5 }
    { 100, 1, 'nyc.%d.jpg', 0.5 }
    { 50, 1, 'bridge.%d.jpg', 0.5 }
};

% Pick a sequence to load
seq = sequences{1};

% Load the sequence
N = seq{1};
images = cell(N, 1);
for i = 1:N
    path = sprintf([dir seq{3}], i - 1 + seq{2});
    fprintf('loading %s\n', path);
    images{i} = imresize(im2double(imread(path)), seq{4});
    if length(size(images{i})) == 3
        images{i} = rgb2gray(images{i});
    end
end

%% Step 2: Detect points in first frame to track

% Detect points in the first frame and track them throughout the sequence.
% Use interp2 to calculate the displacements, but keep in mind that it
% returns NaN for coordinates outside the image. Feature tracking should
% work well enough in the hotel sequence without detecting and correcting
% for drifting.
% 
% Useful functions: sort, interp2, isnan

% Get interest points
[x y strength] = harris(images{1});

% TODO: only keep the strongest interest points (around 500 or so)
numstrong = 500;
[sorted, index] = sort(strength, 'descend');
points = zeros(0, 2);
for i=1:numstrong
    if isnan(sorted(i))
        numstrong = i;
        break;
    end

    points(i,:) = [x(index(i)) y(index(i))];
end



%% Step 3: Track the points with optical flow

% Implement a Kanade-Lucas-Tomasi tracker. You should not need to implement
% coarse-to-fine tracking as the videos don't have any large movements.
% Discard any keypoints if their predicted location moves out-of-frame.
% 
% Useful functions: fspecial, conv2, gradient
u = cell(N-1);
v = cell(N-1);
[row, col] = size(images{1});
track_x = zeros(0, numstrong);
track_y = zeros(0, numstrong);
track_x(1, :) = points(:, 1)';
track_y(1, :) = points(:, 2)';

movedout_x = cell(0);
movedout_y = cell(0);
mindex = 0;

for i = 2:N
    % TODO: write optical flow function
    [u{i-1}, v{i-1}] = optical_flow(images{i-1}, images{i});
    
    % Visualize optic flow
%     uv = cat(3, u{i-1}, v{i-1}, zeros(size(images{i})));
%     imshow(uv + 0.5);
%     pause(0.1);
    
    % TODO: move points according to flow vectors.
    % TODO: discard points that move off the image    
    interpedu = interp2(u{i-1}, track_x(i-1, :), track_y(i-1, :));
    interpedv = interp2(v{i-1}, track_x(i-1, :), track_y(i-1, :));
    track_x(i, :) = track_x(i-1, :) + interpedu;
    track_y(i, :) = track_y(i-1, :) + interpedv;
    
    j = 1;
    while j <= numstrong
        if (isnan(track_x(i, j)) || isnan(track_y(i, j)))
%             mindex = mindex + 1;
%             movedout_x{mindex} = track_x(1:i, j);
%             movedout_y{mindex} = track_y(1:i, j);
            track_x(:, j) = [];
            track_y(:, j) = [];
            numstrong = numstrong - 1;
        else
            j = j + 1;
        end
    end
end

%points that went outside
% figure, imshow(images{1})
% for i=1:mindex
%     hold on;
%     plot(movedout_x{i}, movedout_y{i})
%     hold off;
% end
% pause

save('track_x.mat', 'track_x');
save('track_y.mat', 'track_y');

%lines
% load('track_x.mat', 'track_x');
% load('track_y.mat', 'track_y');
% figure
% for i=1:20
%     index = randi(400, 1);
%     hold on;
%     plot(track_x(:, index), track_y(:, index));
%     hold off;
% end
% pause

% Visualize tracked points
figure
% for i = 1:N
%     if mod(i, 10) ~= 0
%         continue;
%     end
%     imshow(images{i})
%     hold on;
%     plot(track_x(i, :), track_y(i, :), 'ro');
%     hold off;
% %     pause
%     pause(0.01);
% end

%% Step 4: Structure from motion algorithm

% 1) Given: n images and m tracked features
% 2) For each image i, center the feature coordinates
% 3) Construct a 2n*m measurement matrix D(i, j)
%    - Column j contains the projection of point j in all views
%    - Row i contains one coordinate of the projections of all the m points
%      in image i
% 4) Factorize D
%    - Compute SVD: D = U W V'
%    - Create U3 by taking the first 3 columns of U
%    - Create V3 by taking the first 3 columns of V
%    - Create W3 by taking the upper left 3*3 block of W
% 5) Create the motion (affine) and shape (3D) matrices:
%    - A = U3 * W3 ^ 0.5 and X = W3 ^ 0.5 * V3'
% 6) Eliminate affine ambiguity
%    - See "A Sequential Factorization Method for Recovering Shape and
%      Motion From Image Streams" (Morita and Kanade) for an explaination
% 
% Useful functions: svd, chol

% Temporary data: flat plane + random camera (replace this with your code)
D = zeros(2*N, numstrong);

% 3) Construct a 2n*m measurement matrix D(i, j)
cX = sum(track_x, 2) / numstrong;
cY = sum(track_y, 2) / numstrong;
cX = repmat(cX, 1, numstrong);
cY = repmat(cY, 1, numstrong);

cX = track_x - cX;
cY = track_y - cY;

D = [cX; cY];

% for i=1:N
%     avg_x = sum(track_x(i, :)) / numstrong;
%     avg_y = sum(track_y(i, :)) / numstrong;
%     for j=1:numstrong
%         D(2*i - 1, j) = track_x(i, j) - avg_x;
%         D(2*i, j) = track_y(i, j) - avg_y;
%     end
% end

% 4) Factorize D
[U W V] = svd(D);
V3 = V(:, 1:3);
W3 = W(1:3, 1:3);
U3 = U(:, 1:3);
W3_sqrt = W3 ^ 0.5;

% 5) Create the motion (affine) and shape (3D) matrices:
Mhat = U3 * W3_sqrt;
Shat = W3_sqrt * V3';

% 6) Eliminate affine ambiguity
% get G
G = zeros(3*N, 6);
for i=1:N
    a = Mhat(i, :);
    b = Mhat(i, :);
    gxi = [ a(1)*b(1) (a(1)*b(2) + a(2)*b(1)) (a(1)*b(3) + a(3)*b(1)) a(2)*b(2) (a(2)*b(3) + b(3)*a(2)) a(3)*b(3) ];
    a = Mhat(i + N, :);
    b = Mhat(i + N, :);
    gyi = [ a(1)*b(1) (a(1)*b(2) + a(2)*b(1)) (a(1)*b(3) + a(3)*b(1)) a(2)*b(2) (a(2)*b(3) + b(3)*a(2)) a(3)*b(3) ];
    a = Mhat(i, :);
    b = Mhat(i + N, :);
    gxyi = [ a(1)*b(1) (a(1)*b(2) + a(2)*b(1)) (a(1)*b(3) + a(3)*b(1)) a(2)*b(2) (a(2)*b(3) + b(3)*a(2)) a(3)*b(3) ];

    G(i, :) = gxi;
    G(i + N, :) = gyi;
    G(i + 2*N, :) = gxyi;
end

c = [ones(2*N, 1); zeros(N, 1)];

l = (G' * G) \ (G' * c);
L = [ l(1) l(2) l(3);
      l(2) l(4) l(5);
      l(3) l(5) l(6) ];

A = chol(L);
A = A';
M = Mhat * A;
S = A \ Shat;

points = S';
camera_x = M(1:N, :);
camera_y = M(N+1:2*N, :);

% figure
% k = cross(camera_x, camera_y);
% for i=1:N
%     k(i, :) = k(i,:) / norm(k(i,:));
% end
% hold on;
% plot(k(:,3))%, k(:,2), k(:,3));
% hold off;
% pause

%% Step 5: Plot results

% Create a 2D delaunay triangulation and display a 3D texture-mapped mesh
figure;
x = points(:, 1);
y = points(:, 2);
z = points(:, 3);
tri = delaunay(x, y);
patcht(tri, [x y z], tri, [track_y(1, :)' track_x(1, :)'], repmat(images{1}, [1 1 3]));
set(gca, 'DataAspectRatio', [1 1 1], 'PlotBoxAspectRatio', [1 1 1], 'Projection', 'perspective');
hold on;

% We don't have a camera position because we're using an orthographic
% camera, but we do have camera direction. Plot each direction as a line.
magnitude = max(max(points));
camera_z = cross(camera_x, camera_y);
for i = 1:N
    dir = camera_z(i, :) / norm(camera_z(i, :));
    line = [magnitude * dir; 1.5 * magnitude * dir];
    plot3(line(:, 1), line(:, 2), line(:, 3), 'r');
end
