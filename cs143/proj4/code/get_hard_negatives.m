function [crops] = get_hard_negatives(non_face_scn_path, cascade, patch_size, linear, num_crops, max_crops_per_scn)
%Run the cascade on non-face scenes and return detections
% 'non_face_scn_path' is a string. This directory contains many images
%   which have no faces in them.
% 'cascade' is an Nx2 or Nx3 classifier cascade (or stump)
% 'patch_size' is the width of each patch to be classified.
% 'num_crops' is the number of hard negatives to be mined.

% output 'crops' is N by D matrix where N is the number of non-faces and D
%   is the dimensionality of the patch.

% This function can be implemented as a variant of run_detector.m, although
% non-maximum suppression isn't necessarily desirable. Functions which you
% may want to use are:
%   get_detections_all_scales.m
%   detection2bbox.m    %warning, bboxes can be out of image coordinates.
%   bboxes2crops.m
nf_imgs = dir( fullfile( non_face_scn_path, '*.jpg' ));

crops = zeros(0, patch_size^2, 'single');

for i = 1:length(nf_imgs)
    img = imread( fullfile( non_face_scn_path, nf_imgs(i).name ));
    img = single(img)/255;
    if(size(img,3) > 1); img = rgb2gray(img); end
    
    verbose = false;
    step_size = 1;       %reducing this should increase accuracy while reducing speed.
    scale_factor = 1.5;  %reducing this should increase accuracy while reducing speed.
    min_scale_size = 50;
    start_scale = 2;     %reducing this should increase accuracy (small faces might otherwise be missed)
    cur_detection = get_detections_all_scales( ...
        img, cascade, patch_size, linear, verbose, step_size, scale_factor, min_scale_size, start_scale);

    [bboxes, confidences] = detection2bbox(cur_detection, patch_size);
    [row, col] = size(img);

    for j=1:size(bboxes, 1)
        if (bboxes(j, 1) < 1)
            bboxes(j, 1) = 1;
        elseif (bboxes(j, 1) > col)
            bboxes(j, 1) = col;
        end
        if (bboxes(j, 2) < 1)
            bboxes(j, 2) = 1;
        elseif (bboxes(j, 2) > row)
            bboxes(j, 2) = row;
        end
        if (bboxes(j, 3) < 1)
            bboxes(j, 3) = 1;
        elseif (bboxes(j, 3) > col)
            bboxes(j, 3) = col;
        end
        if (bboxes(j, 4) < 1)
            bboxes(j, 4) = 1;
        elseif (bboxes(j, 4) > row)
            bboxes(j, 4) = row;
        end
    end
    cur_crops = bboxes2crops(img, bboxes, patch_size);

%     if (size(cur_crops, 1) > max_crops_per_scn)
%         cur_crops = cur_crops(1:max_crops_per_scn, :);
%     end

    crops = [crops; cur_crops];

    if (size(crops, 1)  > num_crops)
        crops = crops(1:num_crops, :);
        break;
    end
end

end