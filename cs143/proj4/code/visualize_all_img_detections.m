function visualize_all_img_detections(bboxes, confidences, img, conf_thresh)
%unlike visualize_detections.m, which shows detections over an entire test
%set one at a time, this will show all of the detections for a given photo
%at the same time. 

% 'bboxes' is Nx4, N is the number of non-overlapping detections, and each
% row is [x_min, y_min, x_max, y_max]
% 'confidences' is the Nx1 (final cascade node) confidence of each
% detection.
% 'img' is the image to plot detections on.
% 'conf_thresh' is the confidence threshold. Detections below this
% confidence will not be shown.

if(~exist('conf_thresh', 'var'))
	conf_thresh = 0;
end

high_conf = confidences > conf_thresh;
confidences = confidences(high_conf, :);
bboxes      = bboxes(     high_conf, :);

% sort detections by decreasing confidence
[sc,si]=sort(confidences, 'descend');
bboxes   =bboxes(si,:);

% view detections
nd=length(confidences);

figure(14)
imshow(img);
hold on;

for d=1:nd      
    bb = bboxes(d,:);
    % draw detection bounding box
    plot(bb([1 3 3 1 1]),bb([2 2 4 4 2]),'g:','linewidth',3); 
end

hold off;


