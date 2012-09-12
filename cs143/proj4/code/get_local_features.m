function [local_features, centers] = get_local_features(img)
% by James Hays
% local_features is an N by feature dimension matrix
% centers is an N by 3 matrix, where each element is the y, x, scale center
% of each of the patches.

% by default this will return normalized patches. But one could use more
% complex local features (SIFT, HoG, geometric blur, SSIM, etc...)

% Actually, why not start with a feature that vl_feat can give us, like
% sift or phow? Because it's actually more problematic to visualize, and to
% deal with heterogenous data sources. 

[local_features, centers] = get_normalized_patches(img);