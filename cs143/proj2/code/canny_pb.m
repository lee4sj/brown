function [ pb ] = canny_pb( im,thresh,sigma )
%  canny version
pb=im*0;
for t=thresh
    for s=sigma
        pb=pb+single(edge(im,'canny',t,s));
    end
end
%linear scale : 0 to 1
low=min(pb(:));
high=max(pb(:));
pb=(pb-low)/(high-low);
end

