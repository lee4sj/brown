
% Debugging tip: You can split your MATLAB code into cells using "%%"
% comments. The cell containing the cursor has a light yellow background,
% and you can press Ctrl+Enter to run just the code in that cell. Use this
% so you don't have to align the images each time you run your code.

close all; % closes all figures

%% Setup

% read images and convert to single format
im1 = im2single(imread('../data/Pikachu2.jpg'));
im2 = im2single(imread('../data/Raichu.jpg'));
im1 = rgb2gray(im1);
im2 = rgb2gray(im2);

% use this if you want to align the two images (e.g., by the eyes) and crop
% them to be the same size
[im2, im1] = align_images(im2, im1);

%% Create the Gaussian and Laplacian pyramids
N = 9; % number of pyramid levels (you may use more or fewer, as needed)
[gaussian1, laplacian1] = pyramids(im1, N);
[gaussian2, laplacian2] = pyramids(im2, N);

%% Choose the cutoff and compute the hybrid image (you supply this code)
cutoff1 = 6;
cutoff2 = cutoff1 + 1;
im12 = hybridImage(gaussian1, gaussian2, laplacian1, laplacian2, cutoff1, cutoff2);
imshow(im12);

%% Crop resulting image (optional)
figure(1), hold off, imshow(im12), axis image
disp('input crop points');
[x, y] = ginput(2);  x = round(x); y = round(y);
im12 = im12(min(y):max(y), min(x):max(x), :);
figure(1), hold off, imshow(im12), axis image

%% Show different scales of the image
[h w] = size(im12);
small = imresize(im12, 0.25);
[small_h small_w] = size(small);
scales = [im12 ones(h, 5) [small; ones(h - small_h, small_w)]];
imshow(scales);
