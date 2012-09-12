function [ pb] = pb_lite(im,k,fb,masks,sobel,canny)
% args:
%       im: single channel image, double
%       k: number of clusters/textons
%       fb: filter bank, for creating texton map
%       masks: half-discs, for computing histograms/chi-sqr distance
%       sobel/canny: pb baseline results using sobel/canny edge detection
%output:
%       pb: double valued image of prob(boundary), same size as im


% your pb implementation
im=rgb2gray(im);
pb=im*0;

%========================================
%               do texton clustering
%========================================
tmap=texton_img(im,k,fb);
figure;imagesc(tmap);


%========================================
%           get gradient features
%           you can use a single function 
%           to compute gradients/chi-sqr dist
%========================================
%get texton gradients
%you can consider a different binning scheme
tg=get_gradient(tmap,1:k+1,masks);

%get bg
%you can consider a different binning scheme
bg=get_gradient(uint8(255*im),[0:8:255,256],masks);



%combine feature strength
%you can try a simple mean value at each pixel location


%you need to modify this:
mean_tg = mean(tg, 3);
mean_bg = mean(bg, 3);

pb = (canny) .* mean_tg .* mean_bg;

%normalize pb values to lie in [0,1]
low=min(pb(:));
high=max(pb(:));
pb=(pb-low)/(high-low);
end

