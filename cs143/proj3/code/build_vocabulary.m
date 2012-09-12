function [ vocab ] = build_vocabulary( images, vocab_size )
%BUILD_VOCABULARY Build a vocabulary from the given training images
% The inputs are images, a N x 1 cell array of image names and the size of 
% the vocabulary (100 by default).

% 'vocab' should be 128 x vocab_size. Each column is cluster / visual word

%{
Useful functions:
[locations, SIFT_features] = vl_dsift(img) 
 locations is a 2 x n list list of locations, which can be thrown away (or used for extra credit)
 SIFT_features is a 128 x n matrix of SIFT features
  ( documentation: http://www.vlfeat.org/mdoc/VL_DSIFT.html )
  note: there are step, bin size, and smoothing parameters you can manipulate for vl_dsift()

[centers, assignments] = vl_kmeans(matrix, k, 'distance', 'l2')
(Make sure matrix is of type single! single(matrix) to be safe.)
  ( documentation: http://www.vlfeat.org/mdoc/VL_KMEANS.html )
%}
tic

disp(['Processing ' num2str(numel(images)) ' images to build feature vocabulary']);

vocab = zeros(128, vocab_size);
sift = [];
binsize = 4;
step = 8;

for i=1:numel(images)
    %some of the images are in a weird format, this just corrects for it
    disp(images{i});
    img = imread(images{i});
    if ndims(img) > 2
       disp([images{i} ' is color-greyscale']);
       img = rgb2gray(img);
    end
    img = single(img);
   
    
    %collect dense features on images (or try sparse)
    %%if you do dense, we'd suggest starting with a size ~4, and a step ~8
    [locations, temp_sift] = vl_dsift(img, 'size', binsize, 'step', step);
    sift = [sift temp_sift];

    %you can randomly sample the descriptors to save memory and speed up
    %the clustering

    %Cluster the dense SIFT features into a vocabulary of 'visual words' using kmeans
    
end

size(sift),
[vocab, temp] = vl_kmeans(double(sift), vocab_size, 'distance', 'l2');

toc

end



