function features = crops2features(crops)
% input - single, N x D1 matrix, where N is the number of crops and D1 is
%   the dimensionality of each crop.
% output - single, N x D2 matrix, where N is the number of crops and D2 is
%   the dimensionality of the new feature space.

[N, D1] = size(crops);
patch_size = sqrt(D1);
binsize = (patch_size / 4) + 1;
step = 1;

features = zeros(N, 128, 'single');

for i=1:N
    img = reshape(crops(i,:), patch_size, patch_size);
    [frames, sift] = vl_dsift(single(img), 'size', binsize, 'step', step);

    features(i, :) = sift(:, 1)';
end