function [bboxes, confidences] = non_max_supr(detections, patch_size, img_size, verbose)
% high confidence detections suppress all overlapping detections (including
% detections at other scales). Detections can partially overlap, but the
% center of one detection can not be within another detection.

% 'detections' is an Nx4 matrix where each row is the y, x, scale, and
%confidence of the classifier on each crop. scale is from 0 to 1, where 1
%is the finest resolution scale.
% 'patch_size' is the width of each patch to be classified.
% 'img_size' is the [y,x] dimensions of the image.

% 'bboxes' is Nx4, N is the number of non-overlapping detections, and each
% row is [x_min, y_min, x_max, y_max]
% 'confidences' is the Nx1 (final cascade node) confidence of each
% detection.

% code for computing overlap from the 2010 Pascal VOC toolkit.
% http://pascallin.ecs.soton.ac.uk/challenges/VOC/voc2010/index.html#devkit


if(~exist('verbose', 'var'))
    verbose = true;
end

[bboxes, confidences] = detection2bbox(detections, patch_size);
x_out_of_bounds = bboxes(:,3) > img_size(2); %xmax is greater than x dimension
y_out_of_bounds = bboxes(:,4) > img_size(1); %ymax is greater than y dimension
bboxes(x_out_of_bounds,3) = img_size(2);
bboxes(y_out_of_bounds,4) = img_size(1);

%if(verbose)
%    fprintf('warning: %d out of %d detections out of x bounds\n', sum(x_out_of_bounds), num_detections)
%    fprintf('warning: %d out of %d detections out of y bounds\n', sum(y_out_of_bounds), num_detections)
%end

pos_inds = confidences > 0;
confidences = confidences(pos_inds);
bboxes = bboxes(pos_inds,:);

num_detections = sum(pos_inds);

%higher confidence detections get priority.
[confidences, ind] = sort(confidences, 'descend');
bboxes = bboxes(ind,:);

% indicator for whether each bbox will be accepted or suppressed
is_valid_bbox = logical(zeros(1,num_detections)); 

for i = 1:num_detections
    cur_bb = bboxes(i,:);
    cur_bb_is_valid = true;
    
    for j = find(is_valid_bbox)
        %compute overlap with each previously confirmed bbox.
        
        prev_bb=bboxes(j,:);
        bi=[max(cur_bb(1),prev_bb(1)) ; ... 
            max(cur_bb(2),prev_bb(2)) ; ...
            min(cur_bb(3),prev_bb(3)) ; ...
            min(cur_bb(4),prev_bb(4))];
        iw=bi(3)-bi(1)+1;
        ih=bi(4)-bi(2)+1;
        if iw>0 && ih>0                
            % compute overlap as area of intersection / area of union
            ua=(cur_bb(3)-cur_bb(1)+1)*(cur_bb(4)-cur_bb(2)+1)+...
               (prev_bb(3)-prev_bb(1)+1)*(prev_bb(4)-prev_bb(2)+1)-...
               iw*ih;
            ov=iw*ih/ua;
            if ov > 0.3 %If the less confident detection overlaps too much with the previous detection
                cur_bb_is_valid = false;
            end
            
            if(verbose)
                fprintf('detection %d, bbox: [%d %d %d %d], %f overlap with %d [%d %d %d %d]\n', ...
                    i, cur_bb(1), cur_bb(2), cur_bb(3), cur_bb(4), ov, j, prev_bb(1), prev_bb(2), prev_bb(3),prev_bb(4))
            end
        end
    end
    
    is_valid_bbox(i) = cur_bb_is_valid;

end
    
bboxes = bboxes(is_valid_bbox,:);
confidences = confidences(is_valid_bbox,:);


fprintf(' non-max suppression: %d detections to %d final bounding boxes\n', num_detections, size(bboxes,1));


