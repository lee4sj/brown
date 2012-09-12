function [im] = hybridImage(gaussian1, gaussian2, laplacian1, laplacian2, cutoff1, cutoff2)
% takes guassian and laplacian pyramids of two images and cutoff for each,
% then combine them to create a hybrid image. First image is used for lower
% frequency.

im = gaussian1{1};
[a, N] = size(gaussian1);

for n=1:cutoff1
    [size1, size2, size3] = size(gaussian1{n+1});
    im = imresize(im, [size1, size2]);
    im = im + laplacian1{n};
end
 
for n=cutoff2:N-1
    [size1, size2, size3] = size(gaussian2{n+1});
    im = imresize(im, [size1, size2]);
    im = im + laplacian2{n};
end
end