function [bboxes, confidences] = detection2bbox(detections, patch_size)
% 'detections' is an Nx4 matrix where each row is the y, x, scale, and
%confidence of the classifier on each crop. scale is from 0 to 1, where 1
%is the finest resolution scale. Nx3 also works (confidence is not
%examined in that case)
% 'patch_size' is the width of each patch to be classified.

% 'bboxes' is Nx4, N is the number of non-overlapping detections, and each
% row is [x_min, y_min, x_max, y_max].  

% WARNING! Because of repeated rounding there is no guarantee that each
% bounding box fits precisely in the source image boundaries.

num_detections = size(detections,1);
bboxes = zeros(num_detections,4);

y_locs      = detections(:,1);
x_locs      = detections(:,2);
scales      = detections(:,3);

for i = 1:num_detections
    cur_x = (x_locs(i)-1)/scales(i) + 1;
    cur_y = (y_locs(i)-1)/scales(i) + 1;
    
    cur_width = floor(patch_size/scales(i));
    
    bboxes(i, 1) = ceil( cur_x - (cur_width-1/scales(i))/2); %xmin
    bboxes(i, 2) = ceil( cur_y - (cur_width-1/scales(i))/2); %ymin
    bboxes(i, 3) = floor(cur_x + (cur_width-1/scales(i))/2); %xmax
    bboxes(i, 4) = floor(cur_y + (cur_width-1/scales(i))/2); %ymax
end

if(size(detections,2) == 4)
    confidences = detections(:,4);
end