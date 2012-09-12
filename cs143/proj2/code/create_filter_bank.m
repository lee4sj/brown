function [ fb,h ] = create_filter_bank( orientations, sigmas )
no=numel(orientations);
ns=numel(sigmas);
fb=cell(no,ns);

%must return a cell and a figure handle
h=figure; %use subplot to show the filters

for i=1:ns
    g = fspecial('gaussian', [5, 5], sigmas(i));
    s = fspecial('sobel');
    filter = conv2(g, s);
    for j=1:no
        fb{j, i} = imrotate(filter, orientations(j));
        h; subplot(no, ns, no*(i-1) + j), imshow(fb{j, i});
    end
end

end