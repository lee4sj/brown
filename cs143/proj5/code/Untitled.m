
image_files = zeros(0, 1);
image_files= dir( fullfile( '../proj5/html', '*.jpg') );

numimages = length(image_files);

figure
for i=1:numimages
    im = imread(fullfile( '../html', image_files(n).name));
    im = imresize(im, 1/2);
    imshow(im);
    pause
end