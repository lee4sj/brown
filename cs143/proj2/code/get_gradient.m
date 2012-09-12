function [ gradient ] = get_gradient(img,binvalues,masks)
%args:
%       img: single channel image of uint8
%       binvalues: an array of boundaries for each bin
%       masks: half-disc pairs of size n_orient x n_scales x 2
%output:
%       gradient: 3D matrix of size n x m x dim
%                 where n x m is the size of img
%                 dim is n_orient x n_scales
%
%
%hint:  when computing chi-sqr distance, you might want to
%       add 'eps' to the denominator to avoid dividing by zero
%       in MATLAB, eps = 2.2204e-16




%loop over binvalues (and loop over each mask pair)
%turn img into a binary matrix where 1's indicate the current binvalue
%aggregate chi-sqr dist (a sum over bins)

gradient=[];

[temp1, numbins] = size(binvalues);
[no, nr, temp2] = size(masks);
[row, col] = size(img);

gradient = zeros(row, col, no*nr);
meps = eps * ones(row, col);

for m=1:no
    for n=1:nr
        chi = img * 0;
        for i=1:numbins-1
            tmp = bin_img(img, binvalues(i), binvalues(i+1));
            g = imfilter(tmp, masks{m, n, 1}, 'conv');
            h = imfilter(tmp, masks{m, n, 2}, 'conv');
            chi = double(chi) + ((double(g) - double(h)).^2 ./ (double(g) + double(h) + double(meps)));
        end
        gradient(:, :, no*(n-1) + m) = chi;
    end
end



end