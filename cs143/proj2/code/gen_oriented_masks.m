function [masks,masks_h]=gen_oriented_masks(g_orients,g_radii)
no=numel(g_orients);
nr=numel(g_radii);
%for each o and r, generate a PAIR of masks (left and right)
masks=cell(no,nr,2);

masks_h=figure; %for visualization
%use subplot to show the masks

for n=1:nr
    hd = half_disk(g_radii(n));
    for m=1:no
        masks{m, n, 1} = imrotate(hd, g_orients(m));
        masks{m, n, 2} = imrotate(masks{m, n, 1}, 180);
        masks_h; subplot(nr, no * 2, 2*no*(n-1) + 2*(m-1) + 1), imshow(masks{m, n, 1});
        subplot(nr, no * 2, 2*no*(n-1) + 2*(m-1) + 2), imshow(masks{m, n, 2});
    end
end


%================================
%create mask pairs at 
%multiple scales and orientations
%================================
end
