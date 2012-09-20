function str=updatestruc(d2)
str=d2.struc;
n=length(d2.backbone);
bb=find(d2.backbone);
next=zeros(n,1); next(bb(1:end-1))=bb(2:end);
first=bb(1); last=bb(end);
c=find(~d2.backbone(1:n) & d2.backbone(max(1,d2.struc(:,1))));
str(c,3)=next(d2.struc(c,1));
str((d2.struc(:,1)==first) & ~d2.backbone,2)=bb(2);str((d2.struc(:,1)==first) & ~d2.backbone,3)=bb(3);
str((d2.struc(:,1)==last) & ~d2.backbone,3)=bb(end-2);