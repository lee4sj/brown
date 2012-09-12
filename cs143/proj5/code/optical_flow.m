function [u,v] = optical_flow(img1, img2)
%img1 and img2 are two frames
%u, v are the flow vectors from img1 to img2.
%u, v, img1, and img2 are all the same size.


[row, col] = size(img1);
I(:, :, 1) = img1;
I(:, :, 2) = img2;

g = ones(15, 15);

[Ix, Iy] = gradient(img1);

Ix2 = conv2(Ix.*Ix,g,'same');
Iy2 = conv2(Iy.*Iy,g,'same');
IxIy = conv2(Ix.*Iy,g,'same');

It = img2 - img1;

IxIt = conv2(Ix.*It,g,'same');
IyIt = conv2(Iy.*It,g,'same');

det = 1 ./ (Ix2.*Iy2 - IxIy.*IxIy);

u = det .* -(Iy2.*IxIt - IxIy.*IyIt);
v = det .* (IxIy.*IxIt - Ix2.*IyIt);

% 
% for r=1:row
%     for c=1:col     
%         M = [ Ix2(r, c) IxIy(r, c); IxIy(r, c) Iy2(r, c) ];
%         
%         A = [ IxIt(r, c); IyIt(r, c) ];
%         
%         R = M \ (-A);
%         
%         u(r, c) = R(1);
%         v(r, c) = R(2);
%     end
% end
% end