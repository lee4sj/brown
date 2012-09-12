function crops = bboxes2crops(img, bboxes, patch_size)
%bbox format: [x_min, y_min, x_max, y_max]

num_crops = size(bboxes,1);
crops = zeros(num_crops, patch_size^2);

for i = 1:num_crops
    cur_crop = img(bboxes(i,2):bboxes(i,4), bboxes(i,1):bboxes(i,3));
    cur_crop = imresize(cur_crop, [patch_size patch_size], 'bilinear');
    crops(i,:) = cur_crop(:);
end