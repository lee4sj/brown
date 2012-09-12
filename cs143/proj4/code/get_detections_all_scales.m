function detections = get_detections_all_scales( ...
    img, cascade, patch_size, linear, verbose, step_size, scale_factor, min_scale_size, start_scale)
% 'img' is a [0 1] grayscale image of arbitrary size
% 'cascade' is an Nx2 or Nx3 classifier cascade (or stump)
% 'patch_size' is the width of each patch to be classified.
% 'step_size' is the offset between patch centers. step_size=1 means dense.
% 'scale_factor' is the ratio between pyramid levels. 2 would be a typical
%  Gaussian pyramid. 1.25 is typical of detection algorithms.
% 'min_scale_size' is the smallest allowable pyramid level.

% 'detections' is an Nx4 matrix where each row is the y, x, scale, and
%confidence of the classifier on each crop. scale is from 0 to 1, where 1
%is the finest resolution scale.

if(~exist('verbose', 'var'))
    verbose = true;
end

if(~exist('step_size', 'var'))
    step_size = 2;
end

if(~exist('scale_factor', 'var'))
    scale_factor = 1.25;
end

if(~exist('min_scale_size', 'var'))
    min_scale_size = 50;
end

if(~exist('start_scale', 'var')) %if you don't want to start at the highest resolution
    start_scale = 1;
end

img_size = size(img);

%scale image down until min width or height is less than or equal to min_scale_size.
num_scales = max(ceil(log(min(img_size))/log(scale_factor) - log(min_scale_size)/log(scale_factor)) + 1, 1);
detection_cell = cell(num_scales,1);
num_detections = 0;

for cur_scale = 1:num_scales
    if(cur_scale >= start_scale)
        if(verbose)
            fprintf(' scale %.3f (%d by %d)\n', (1/scale_factor)^(cur_scale-1), size(img,2), size(img,1))
        end
        detection_cell{cur_scale} = get_detections_single_scale(img, cascade, patch_size, linear, step_size, verbose);
        num_detections = num_detections + size(detection_cell{cur_scale},1);
    end
    img = imresize(img, 1 / scale_factor, 'bilinear');
end

detections = zeros(num_detections, 4);
counter = 1;
for cur_scale = 1:num_scales
    if(cur_scale >= start_scale)
        detections(counter:counter+size(detection_cell{cur_scale},1)-1,1:2) = detection_cell{cur_scale}(:,1:2); % y, x
        detections(counter:counter+size(detection_cell{cur_scale},1)-1,3)   = (1/scale_factor)^(cur_scale-1);   % scale
        detections(counter:counter+size(detection_cell{cur_scale},1)-1,4)   = detection_cell{cur_scale}(:,3);   % confidence

        counter = counter + size(detection_cell{cur_scale},1);
    end
end






