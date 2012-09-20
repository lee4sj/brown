function x=rigidfit(x,targ,weights)
%from wikipedia, 'fitting quaternions'; also,
%http://openbabel.sourcearchive.com/documentation/2.2.3-1build1/obutil_8cpp-source.html
%qtrfit
s=size(x); st=size(targ); if prod(st(3:end))==1, targ=repmat(targ,[1,1,s(3:end)]);end
if nargin==2,weights=ones(s(1),1);end
weights=repmat(reshape(weights,[],1),[1 s(2:end)]);
x=x-repmat(sum(x.*weights,1)./sum(weights,1),s(1),1);
off=sum(targ.*weights,1)./sum(weights,1); targ=targ-repmat(off,s(1),1);
%M=zeros([3 3 s(3:end)]);
for i=1:prod(s(3:end)) 
    M=x(:,:,i)'*targ(:,:,i); cr=[M(2,3)-M(3,2);M(3,1)-M(1,3);M(1,2)-M(2,1)];
    tr=M(1,1)+M(2,2)+M(3,3); B=[M+M'-tr*eye(3) cr;cr' tr]; 
    [v,~]=eig(B); q=v([4 1:3],end);
    Q=[q(1)^2+q(2)^2-q(3)^2-q(4)^2 2*q(2)*q(3)-2*q(1)*q(4) 2*q(2)*q(4)+2*q(1)*q(3);...
        2*q(2)*q(3)+2*q(1)*q(4) q(1)^2-q(2)^2+q(3)^2-q(4)^2 2*q(3)*q(4)-2*q(1)*q(2);...
        2*q(2)*q(4)-2*q(1)*q(3) 2*q(3)*q(4)+2*q(1)*q(2) q(1)^2-q(2)^2-q(3)^2+q(4)^2];
    x(:,:,i)=x(:,:,i)*Q';
end
x=x+repmat(off,s(1),1);