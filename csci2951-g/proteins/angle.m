function out=angle(x,y,z)
s=size(x); if s(1)~=3, error('must be 3-tuples of points'); end
if (~all(s==size(y))) || (~all(s==size(z))) error('all inputs must have the same size'); end
a=cat(3,reshape(diff(x),2,[]),reshape(diff(y),2,[]),reshape(diff(z),2,[]));
out=reshape(atan(-sqrt(sum(cross(a(1,:,:),a(2,:,:)).^2,3))./dot(a(1,:,:),a(2,:,:))),[s(2:end) 1]);
out(out<0)=out(out<0)+pi;

function out=cross(a,b)
%works along the 3rd dimension
out=cat(3,a(:,:,2).*b(:,:,3)-a(:,:,3).*b(:,:,2),a(:,:,3).*b(:,:,1)-a(:,:,1).*b(:,:,3),a(:,:,1).*b(:,:,2)-a(:,:,2).*b(:,:,1));

function out=dot(a,b)
%works along the 3rd dimension
out=sum(a.*b,3);