function [crops, centers] = get_img_crops(img, patch_size, step_size, verbose)
%'img' is a [0 1] grayscale image of arbitrary size
%'patch_size' is a scalar, the width of each patch.
%'step_size' is a scaler, the step size in pixels between patch centers to
%   sample
%'crops' is single, N x D1 matrix, where N is the number of crops and D1
%   is the dimensionality of each crop.

%vl_dsift and get_img_crops return the same number of features for a given
%step_size (as long as the SIFT and patch are the same width).
  
if(~exist('verbose', 'var'))
    verbose = true;
end

height = size(img,1);
width  = size(img,2);

left_margin  = floor((patch_size-1)/2); %also top margin
right_margin = ceil( (patch_size-1)/2); %also bottom margin

num_crops = floor( 1 + ((height - right_margin) - left_margin - 1)/step_size) * ...
            floor( 1 + ((width  - right_margin) - left_margin - 1)/step_size);

if(verbose)
    fprintf('  extracting %d crops from image\n', num_crops)
end

crops   = zeros(num_crops, patch_size^2);
centers = zeros(num_crops, 2);

counter = 0;
for i = 1 + left_margin : step_size : (height - right_margin)
    for j = 1 + left_margin : step_size : (width - right_margin)
        cur_crop = img( i - left_margin: i + right_margin, ...
                        j - left_margin: j + right_margin);
                    
        counter = counter + 1;

        crops(  counter, :) = cur_crop(:);
        centers(counter, :) = [i, j];
    end 
end

