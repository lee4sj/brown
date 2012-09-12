function [crops] = load_crops(input_path)
% input  - string, path to folder of cropped faces.
% output - single, N x D matrix, where N is the number of crops and D is
%   the dimensionality of each crop.
%
% This function is used to load positive training data. 

image_files = dir( fullfile( input_path, '*.jpg') ); %Caltech Faces stored as .jpg
num_crops = length(image_files);

if(num_crops < 1)
    error('No image files found in %s', input_path)
end

fprintf('Loading %d crops from %s ...\n  ', num_crops, input_path);

% Load one crop to find the dimensionality of the patches
test_crop = imread( fullfile( input_path, image_files(1).name ));
dim = size(test_crop,1) * size(test_crop,2);

crops = zeros(num_crops, dim, 'single');

for n = 1:num_crops
    cur_crop = imread( fullfile( input_path, image_files(n).name ));
    cur_crop = single(cur_crop(:))/255;

    crops(n,:) = cur_crop';
    if(mod(n, 100) == 0)
        fprintf('.')
    end
    if(mod(n, 8000) == 0)
        fprintf('\n  ')
    end
end

fprintf('\n')