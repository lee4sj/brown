function [ half_disk ] = half_disk( radius )
%HALF_DISK Summary of this function goes here
%   Detailed explanation goes here


half_disk = zeros(radius * 2 + 1, radius * 2 + 1);

rs = radius^2;
for i=1:radius
    is = (i - radius - 1)^2;
    for j=1:(2*radius + 1)
        if is + (j - radius - 1)^2 < rs
            half_disk(i, j) = 1;
        end
    end
end

end

