function detections = get_detections_single_scale(img, cascade, patch_size, linear, step_size, verbose)
% 'img' is a [0 1] grayscale image of arbitrary size
% 'cascade' is an Nx2 or Nx3 classifier cascade (or stump)
% 'patch_size' is the width of each patch to be classified.
% 'step_size' is the offset between patch centers. step_size=1 means dense.

% 'detections' is an Nx3 matrix where each row is the y, x, and confidence
%   of the classifier on each crop. 

if(~exist('verbose', 'var'))
    verbose = true;
end
cascade_length = size(cascade,1);

global param; %struct with kernel parameters.

% [crops, centers] = get_img_crops(img, patch_size, step_size, verbose);
% feats = crops2features(crops);

% writing get_img_feats, a wrapper for a function like vl_dsift, can speed
% up training and testing considerably.
[feats, centers] = get_img_feats(img, patch_size, step_size, verbose);

num_crops = size(feats,1);
if(verbose); fprintf('  crops remaining: %d', num_crops); end

if(linear)
    %the first level of the cascade will be applied everywhere, so we can go
    %ahead and do it as a matrix multiply
    confidences = feats*cascade{1,1} + cascade{1,2};
    if(verbose); fprintf(' %d', sum(confidences > 0)); end
    
    for cascade_stage = 2:cascade_length
        for i = 1:num_crops
            if(confidences(i) > 0)
                confidences(i) = ...
                    (feats(i,:)*cascade{cascade_stage,1}+ cascade{cascade_stage,2});    
            end
        end
        if(verbose); fprintf(' %d', sum(confidences > 0)); end
    end
    
    %let's go ahead and discard the negative confidence non-detections
    still_alive = confidences > 0;
    confidences = confidences(still_alive,:);
    centers     = centers(    still_alive,:);

else %non-linear classifier
    confidences = ones(num_crops,1);

    for cascade_stage = 1:cascade_length
        
        chunk_size = 1000; %making this larger lets MATLAB vectorize more, but memory can become an issue.
        cur_idx    = 1;
        while(cur_idx <= num_crops)
            cur_idx_max = min(cur_idx + chunk_size - 1, num_crops);
            K = kernel(feats(cur_idx:cur_idx_max,:), cascade{cascade_stage,3}, param);
            confidences(cur_idx:cur_idx_max) = ...
                K*cascade{cascade_stage,1} + cascade{cascade_stage,2};
            cur_idx = cur_idx + chunk_size;
        end
                
        %remove all features that have not passed a cascade stage
        still_alive = confidences > 0;
        confidences = confidences( still_alive, :); %this is 1d, but the : operator maintains dimensionality
        feats       = feats(       still_alive, :);
        centers     = centers(     still_alive, :);
        
        
        num_crops = size(feats,1);
        if(verbose); fprintf(' %d', num_crops); end
    end    
end

detections = [centers, confidences];

if(verbose); fprintf('\n'); end
