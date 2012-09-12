function [ tmap ] = texton_img(img,k,fb)
% args:
%       img: single channel image, should be uint8 format in matlab
%            you can check its type by class(img)
%       k: number of textons/clusters
%       fb: filter bank
% output:
%       tmap: texton map, tmap(i,j)=texton id (int value)
%             the size should be the same as the img.

tmap=img;

[row, col] = size(tmap);
[no, ns] = size(fb);
data = zeros(no*ns, row*col);
for s=1:ns
    for o=1:no
        temp_im = imfilter(tmap, fb{o, s});
        temp_im = reshape(temp_im, 1, row*col);
        data(no*(s-1) + o, :) = temp_im;
    end
end

[membership, means, err] = kmeansML(k, data);
tmap = reshape(membership, row, col);

%1. filter image for each filter in fb
%2. kmeans clustering (the set of filter responses per pixel location should be considered one
%   single feature vector)
%3. return texton map


%we provided a kmeansML.m function created by the berkeley group
%you can also use the matlab built-in kmeans function
%make sure you check the way the arguments are passed in for these
%functions
end

