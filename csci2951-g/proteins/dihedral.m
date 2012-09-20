function out=dihedral(x,y,z)
s=size(x); if s(1)~=4, error('must be 4-tuples of points'); end
if (~all(s==size(y))) || (~all(s==size(z))) error('all inputs must have the same size'); end
a=cat(3,reshape(diff(x),3,[]),reshape(diff(y),3,[]),reshape(diff(z),3,[]));
out=reshape(atan2(sqrt(sum(a(2,:,:).^2,3)).*dot(a(1,:,:),cross(a(2,:,:),a(3,:,:))),dot(cross(a(1,:,:),a(2,:,:)),cross(a(2,:,:),a(3,:,:)))),[s(2:end) 1]);

function out=cross(a,b)
%works along the 3rd dimension
out=cat(3,a(:,:,2).*b(:,:,3)-a(:,:,3).*b(:,:,2),a(:,:,3).*b(:,:,1)-a(:,:,1).*b(:,:,3),a(:,:,1).*b(:,:,2)-a(:,:,2).*b(:,:,1));

function out=dot(a,b)
%works along the 3rd dimension
out=sum(a.*b,3);