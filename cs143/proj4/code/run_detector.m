function [bboxes, confidences, image_ids] = run_detector(test_scn_path, cascade, patch_size, linear)
% this function returns all of the (non-max-suppressed) detections on all
% of the images in a given path.

% 'test_scn_path' is a string. This directory contains images which may or
%    may not have faces in them.
% 'cascade' is an Nx2 or Nx3 classifier cascade (or stump)
% 'patch_size' is the width of each patch to be classified.

% 'bboxes' is Nx4, N is the number of detections, and each row is
%   [y_min, y_max, x_min, x_max] 
% 'confidences' is the Nx1 (final cascade node) confidence of each
%   detection. 'image_ids' is the Nx1 image names for each detection.

test_scenes = dir( fullfile( test_scn_path, '*.jpg' ));

%initialize these as empty and incrementally expand them.
bboxes = zeros(0,4);
confidences = zeros(0,1);
image_ids = cell(0,1);

for i = 1:length(test_scenes)
    fprintf('Detecting faces in %s\n', test_scenes(i).name)
    img = imread( fullfile( test_scn_path, test_scenes(i).name ));
    img = single(img)/255;
    if(size(img,3) > 1); img = rgb2gray(img); end
    
    verbose = false;
    step_size = 2;       %reducing this should increase accuracy while reducing speed.
    scale_factor = 1.5;  %reducing this should increase accuracy while reducing speed.
    min_scale_size = 50;
    start_scale = 2;     %reducing this should increase accuracy (small faces might otherwise be missed)
    cur_detections = get_detections_all_scales( ...
        img, cascade, patch_size, linear, verbose, step_size, scale_factor, min_scale_size, start_scale);
    
    verbose = false;
    [cur_bboxes, cur_confidences] = non_max_supr(cur_detections, patch_size, size(img), verbose);
        
    num_detections = size(cur_bboxes,1);
    cur_image_ids = cell(0,1); 
    cur_image_ids(1:num_detections) = {test_scenes(i).name};
    
    bboxes      = [bboxes; cur_bboxes];
    confidences = [confidences; cur_confidences];
    image_ids   = [image_ids; cur_image_ids];
end
