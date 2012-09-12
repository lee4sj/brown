function [ output_args ] = test( input_args )
%TEST Summary of this function goes here
%   Detailed explanation goes here
tic

ns = 2;
no = 16;

fb = cell(no, ns);

orientations=linspace(0, 360, no+1);%actual orientations
orientations(end)=[];
sigmas = [1, 2];

h = figure;

for i=1:ns
    g = fspecial('gaussian', [3, 3], sigmas(i));
    s = fspecial('sobel');
    filter = conv2(g, s);
    for j=1:no
        fb{j, i} = imrotate(filter, orientations(j));
        h; subplot(no, ns, no*(i-1) + j), imshow(imresize(fb{j, i}, 100));
    end
end

print(h,'-dpng','fb_resize.png');

g_orients=orientations(1:no/2);      %num of orientations for gradient computation
g_radii=[5,10,20];                           %scales for gradient computation (radius of a disc)

ngo = numel(g_orients);
ngr = numel(g_radii);
mask = cell(ngo, ngr, 2);

h2 = figure;

for n=1:ngr
    hd = half_disk(g_radii(n));
    for m=1:ngo
        mask{m, n, 1} = imrotate(hd, g_orients(m));
        mask{m, n, 2} = imrotate(mask{m, n, 1}, 180);
        h2; subplot(ngr, ngo * 2, 2*ngo*(n-1) + 2*(m-1) + 1), imshow(mask{m, n, 1});
        subplot(ngr, ngo * 2, 2*ngo*(n-1) + 2*(m-1) + 2), imshow(imresize(mask{m, n, 2}, 100));
    end
end

print(h2, '-dpng', 'masks_resize.png');

tmap=imread('../data/testset/106005.jpg');

[no, ns] = size(fb);

k=64;

[row, col] = size(tmap);
data = zeros(no*ns, row*col);
for s=1:ns
    for o=1:no
        temp_im = imfilter(tmap, fb{o, s});
        temp_im = reshape(temp_im, 1, row*col);
        data(no*(s-1) + o, :) = temp_im;
    end
end

[membership, means, err] = kmeansML(k, data);
tmap = reshape(membership, row, col);
figure, imagesc(tmap)


%1. filter image for each filter in fb
%2. kmeans clustering (the set of filter responses per pixel location should be considered one
%   single feature vector)
%3. return texton map
tg=get_gradient(tmap,1:k+1,mask);

% Gradient

toc

end