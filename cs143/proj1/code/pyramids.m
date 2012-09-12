function [gim, lim] = pyramids(im, N)
% create gaussian pyramid and laplacian pyramid for im up to N level.
% gim = gaussiam filtered image.
% lim = laplacian filtered image.

%gaussian pyramid
gim{N} = im;
gfilter = fspecial('gaussian', 20, 2);
for n=(N-1):-1:1
    filtered = imfilter(gim{n+1}, gfilter);
    lim{n} = gim{n+1} - filtered;
    gim{n} = filtered(1:2:end, 1:2:end);
end

end