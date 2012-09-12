function [bboxes, confidences] = run_detector_img(img, cascade, patch_size, linear)
% this function returns all of the (non-max-suppressed) detections for a
% given image

% 'img' is a an image
% 'cascade' is an Nx2 or Nx3 classifier cascade (or stump)
% 'patch_size' is the width of each patch to be classified.

% 'bboxes' is Nx4, N is the number of detections, and each row is
%   [y_min, y_max, x_min, x_max] 
% 'confidences' is the Nx1 (final cascade node) confidence of each
%   detection. 'image_ids' is the Nx1 image names for each detection.

if(size(img,3) > 1); img = rgb2gray(img); end
    
verbose = true;
step_size = 3;       %reducing this should increase accuracy while reducing speed.
scale_factor = 1.2;  %reducing this should increase accuracy while reducing speed.
min_scale_size = 50;
start_scale = 2;     %reducing this should increase accuracy (small faces might otherwise be missed)
cur_detections = get_detections_all_scales( ...
    img, cascade, patch_size, linear, verbose, step_size, scale_factor, min_scale_size, start_scale);

verbose = false;
[bboxes, confidences] = non_max_supr(cur_detections, patch_size, size(img), verbose);
