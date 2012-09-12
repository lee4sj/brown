% For using color.

close all; % closes all figures

%% Setup

% read images and convert to single format
im2 = im2single(imread('../data/dark-knight.jpg'));
im1 = im2single(imread('../data/dark_knight_logo.jpg'));

% use this if you want to align the two images (e.g., by the eyes) and crop
% them to be the same size
[im2, im1] = align_images(im2, im1);

%% Create the Gaussian and Laplacian pyramids
N = 12; % number of pyramid levels (you may use more or fewer, as needed)
[gaussian1, laplacian1] = pyramids_color(im1, N);
[gaussian2, laplacian2] = pyramids_color(im2, N);

%% Choose the cutoff and compute the hybrid image (you supply this code)
cutoff1 = 9;
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
[h, w] = size(im12);
small = imresize(im12, 0.20);
[small_h, small_w, small_d] = size(small);
scales = [im12 ones(h, 5, 3) [small; ones(h - small_h, small_w, small_d)]];
imshow(scales);
