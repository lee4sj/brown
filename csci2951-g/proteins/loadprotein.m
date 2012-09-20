function out=loadprotein(d,seq,params)
if ischar(params), if upper(params(1))=='A',  params=[-40 180 -70];else params=[-135 180 135]; end; end;%params=[-75 180 -30];
names=[]; for i=1:length(d), names{i}=d{i}.name; end; names=upper(names);
out.edges=[];out.charges=[];out.backbone=[];out.types=[];out.residue=[];last=0;
p=[0 0 0]; pos=0;out.struc=[];j=0;
for i=1:length(seq)
    jp=j;j=strmatch(upper(seq{i}),names);
    l=length(d{j}.names);
    if last, out.edges=[out.edges;last pos+min(find(d{j}.backbone))];end
    out.edges=[out.edges; pos+d{j}.edges];    
    out.p(pos+(1:l),:)=repmat(p,l,1)+d{j}.p;
    out.charges=[out.charges;d{j}.charges];
    out.backbone=[out.backbone;d{j}.backbone];
    out.types=[out.types;d{j}.types];
    out.residue(pos+(1:l),1)=i;
    foo=d{j}.struc; if i>1, lookup=pos+[find(d{jp}.backbone)-length(d{jp}.backbone);(1:l)']; foo=lookup(foo+3); else foo=foo+pos; end
    out.struc=[out.struc;foo];
    if i>1,
        lb=out.p(lookup(1:3),:);angs=[1.34 118 params(1);1 124 params(2);1 90 params(3);1 90 90];lb(7,1)=0;
        for k=1:4,o1=lb(k+2,:)-lb(k+1,:);o1=o1/sqrt(sum(o1.^2));o2=lb(k+1,:)-lb(k,:);o2=o2-o1*(o2*o1');o2=o2/sqrt(sum(o2.^2));o3=cross(o1,o2);
            lb(k+3,:)=lb(k+2,:)+angs(k,1)*(-o1*cosd(angs(k,2))+sind(angs(k,2))*(-o2*cosd(angs(k,3))-o3*sind(angs(k,3))));
        end
        cb=d{j}.p(find(d{j}.backbone),:);t1=cb(2,:)-cb(1,:);t1=t1/sqrt(sum(t1.^2));t2=cb(3,:)-cb(2,:);t2=t2-t1*(t2*t1');t2=t2/sqrt(sum(t2.^2));t3=cross(t1,t2);
        M=inv([t1;t2;t3])*diff(lb(4:7,:));out.p(pos+(1:l),:)=repmat(lb(4,:),l,1)+(d{j}.p-repmat(cb(1,:),l,1))*M;
    end
    last=pos+max(find(d{j}.backbone)); p=out.p(last,:); pos=pos+l;
end
out.struc=updatestruc(out);
n=length(out.charges);bb=find(out.backbone);  ct=char(out.types); str=findstruc(out.p,out.struc,0); 
bbb=zeros(3+n,1);bbb(3+bb(4:3:end))=1;
str(3+find(bbb(3+out.struc(:,1)) & ct(:,1)=='H'),6)=180;str(3+find(bbb(3+out.struc(:,1)) & ct(:,1)=='H'),5)=120;
bbb=zeros(3+n,1);bbb(3+bb(3:3:end-1))=1;str(3+find(bbb(3+out.struc(:,1)) & ct(:,1)=='O'),6)=180;
out.p=myang2pos(str); out.p=out.p-repmat(mean(out.p),size(out.p,1),1);
out.seq=lower(seq);