function [ histogram ] = make_hist( image_path, vocab, vocab_size )
%MAKE_HIST Generate a histogram of word occurances from an image and a
% vocabulary. This will use very similar code to build_vocabulary. Should
% have the same number of bins as the size of the vocabulary. Don't forget
% to normalize the histograms.

% histogram should be vocab_size x 1

%{
Useful function:
D = vl_alldist2(X,Y) returns the pairwise distance matrix D of the columns of X and Y

Or:

For speed, since this code is called in a tight loop, you might want to
play with a KD-tree algorithm (we found it improved performance modestly.)
A library for KD-trees is included in the stencil code.
 http://www.vlfeat.org/mdoc/VL_KDTREEBUILD.html

You can also, of course, play with your own optimization schemes.

%}
%image handling again
img = imread(image_path);
if ndims(img) > 2
   disp([image_path ' is color-greyscale']);
   img = rgb2gray(img);
end
img = single(img);

%collect dense features on images (or try sparse)
%if you do dense, we'd suggest starting with a size ~4, and a step ~8
binsize = 4;
step = 8;
[locations, sift] = vl_dsift(img, 'size', binsize, 'step', step);

%make your histogram here!
histogram = zeros(vocab_size, 1);
compare = vl_alldist2(double(sift), vocab);
[row, col] = size(compare);

% threshhold = 100000;

for i=1:row
    [y, index] = min(compare(i, :));
%     if (y < threshhold)
    histogram(index) = histogram(index) + 1;
%     end
end

total = sum(histogram);
if (total > 0)
    histogram = histogram / total; 
else
    total
end

end