function [crops] = get_random_negatives(non_face_scn_path, patch_size, num_crops, verbose)
% 'non_face_scn_path' is a string. This directory contains many images
%   which have no faces in them.
% 'patch_size' is the width of each patch to be classified (even if you
%   will convert each patch to some other feature space).
% 'num_crops' is the number of random negatives to be mined, if this many
%   false positives cannot be found, this function will return fewer crops.

% 'crops' is N by D matrix where N is the number of non-faces and D is the
% dimensionality (patch_size * patch_size).

if(~exist('num_crops', 'var'))
    num_crops = 1000;
end

if(~exist('verbose', 'var'))
    verbose = false;
end

crops = zeros(num_crops, patch_size^2);
non_face_files = dir( fullfile( non_face_scn_path, '*.jpg' ));
num_imgs = length(non_face_files);
crops_per_img = ceil( num_crops / num_imgs);

%randomize the order so that we're not biased towards alphabetically
%earlier images.
non_face_files = non_face_files(randperm(num_imgs));
counter = 0;
for i = 1:length(non_face_files)
    fprintf('Mining random negatives from %s\n', non_face_files(i).name)
    
    img = imread( fullfile( non_face_scn_path, non_face_files(i).name ));
    img = single(img)/255;
    if(size(img,3) > 1)
        img = rgb2gray(img);
    end
    random_rescale = (rand(1) + 0.5) / 1.5; %random scale
    img = imresize(img, random_rescale, 'bilinear');
    
    step_size = 10;  
    [cur_crops, centers] = get_img_crops(img, patch_size, step_size, verbose);
    
    crops_inds_to_take = randperm(size(cur_crops,1)) <= crops_per_img;
    cur_crops = cur_crops(crops_inds_to_take,:);
    crops(counter+1:counter+size(cur_crops,1),:) = cur_crops;
    counter = counter + size(cur_crops,1);
end

crops = crops(1:min(counter,num_crops),:);


