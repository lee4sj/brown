%script for detecting and visualizing faces in class photos.

if(~exist('cascade', 'var'))
    error('variable "cascade" does not exist. Build or load classifier.');
end

if(~exist('linear', 'var'))
	error('variable "linear" does not exist. Set it to true or false'); 
end

if(~exist('patch_size', 'var'))
    fprintf('Setting patch size to 36\n');
    patch_size = 36;
end

class_photo_path = 'class_photos';
class_photo_files = dir( fullfile( class_photo_path, '*.jpg'));

for i = 1:length(class_photo_files)
    cur_img = imread( fullfile( class_photo_path, class_photo_files(i).name));
    cur_img = double(cur_img)/255;
    
    [bboxes, confidences] = run_detector_img(cur_img, cascade, patch_size, linear);

    for conf_thresh = 0:.1:1.5
        fprintf('Visualizing detections with at least %f confidence\n', conf_thresh)
        visualize_all_img_detections(bboxes, confidences, cur_img, conf_thresh);
        pause
    end        
    
end