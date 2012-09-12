function [ output_args ] = test( input_args )
%TEST Summary of this function goes here
%   Detailed explanation goes here

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
% 
% switch computer %see 'help computer' for explanation of this
%     case 'GLNX86'
%         addpath vlfeat/toolbox/mex/mexglx
%     case 'GLNXA64'
%         addpath vlfeat/toolbox/mex/mexa64
%     case 'MACI'
%         addpath vlfeat/toolbox/mex/mexmaci
%     case 'MACI64'
%         addpath vlfeat/toolbox/mex/mexmaci64
%     case 'PCWIN'
%         addpath vlfeat/toolbox/mex/mexw32
%     case 'PCWIN64'
%         addpath vlfeat/toolbox/mex/mexw64
% end
% 
% tic
% vocab_size = 200;
% 
% load('vocab.mat', 'vocab');
% 
% img = imread('../data/train/bedroom/image_0005.jpg');
% if ndims(img) > 2
%    disp([image_path ' is color-greyscale']);
%    img = rgb2gray(img);
% end
% img = single(img);
% 
% %collect dense features on images (or try sparse)
% %if you do dense, we'd suggest starting with a size ~4, and a step ~8
% binsize = 4;
% step = 8;
% [locations, sift] = vl_dsift(img, 'size', binsize, 'step', step);
% 
% %make your histogram here!
% histogram = zeros(vocab_size, 1);
% compare = vl_alldist2(double(sift), vocab);
% [row, col] = size(compare);
% 
% counter = 0;
% 
% for i=1:row
%     [y, index] = min(compare(i, :));
%     if (y < 100000)
%         counter = counter + 1;
%         y
%     end
%     histogram(index) = histogram(index) + 1;
% end
% 
% counter
% 
% total = sum(histogram);
% histogram = histogram / total;

kerneloption = 1;
n2 = 400

[nk,nk2]=size(kerneloption);
if nk ~=nk2
    if nk>nk2
        kerneloption=kerneloption';
    end;
else
    kerneloption=ones(1,n2)*kerneloption;
end;

if length(kerneloption)~=n2 & length(kerneloption)~=n2+1 
    error('Number of kerneloption is not compatible with data...');
end;


metric = diag(1./kerneloption.^2);
ps = x*metric*xsup'; 
[nps,pps]=size(ps);
normx = sum(x.^2*metric,2);
normxsup = sum(xsup.^2*metric,2);
ps = -2*ps + repmat(normx,1,pps) + repmat(normxsup',nps,1) ; 


K = exp(-ps/2);

K

end

