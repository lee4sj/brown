function [feats, centers] = get_img_feats(img, patch_size, step_size, verbose)
% While get_img_crops returns raw image crops, this will directly return a
% feature. This might speed up detection significantly, for instance by
% acting as a wrapper to a function such as vl_dsift().

binsize = (patch_size / 4) + 1;
[centers, feats] = vl_dsift(single(img), 'size', binsize, 'step', step_size);

feats = single(feats');
centers = centers';
centers = [centers(:, 2) centers(:, 1)];