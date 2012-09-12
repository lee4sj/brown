function [ binary_img ] = bin_img(img, min, max)

[ row, col ] = size(img);
binary_img = img * 0;
for r=1:row
    for c=1:col
        if  min <= img(r, c) && img(r, c) < max
            binary_img(r, c) = 1;
        else
            binary_img(r, c) = 0;
        end
    end
end