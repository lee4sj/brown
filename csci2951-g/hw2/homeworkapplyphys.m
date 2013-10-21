function o=homeworkapplyphys(d,p)
o.bonds=zeros(size(d.edges,1),4);o.angles=[];o.dihedrals=[];
ttable=zeros(size(d.types)); for i=1:length(d.types), ttable(i)=strmatch(d.types{i},p.types,'exact'); end
ettable=ttable(d.edges);
for i=1:size(d.edges,1),
    ind=find(all(repmat(ettable(i,:),size(p.bonds,1),1)==p.bonds(:,1:2),2));
    if ~length(ind), ind=find(all(repmat(ettable(i,2:-1:1),size(p.bonds,1),1)==p.bonds(:,1:2),2)); end
    o.bonds(i,:)=[double(d.edges(i,:)) p.bonds(ind,3:4)];
end
na=1;
for i=1:size(d.types,1)
    inc1=find(d.edges(:,1)==i); inc2=find(d.edges(:,2)==i); inca=[inc1;inc2];
    inc=double([d.edges(inc1,2);d.edges(inc2,1)]);
    for j=1:length(inc)-1, for k=j+1:length(inc),
            ang=ttable([inc(j) i inc(k)]);
            ind=find(all(repmat(ang',size(p.angles,1),1)==p.angles(:,1:3),2));
            if ~length(ind), ind=find(all(repmat(ang(3:-1:1)',size(p.angles,1),1)==p.angles(:,1:3),2)); end
            o.angles(na,:)=[inc(j) i inc(k) p.angles(ind,4:5)];
            na=na+1;
     end; end
end
nd=0;test=reshape(permute(cat(3,p.dihedrals(:,1:4),fliplr(p.dihedrals(:,1:4))),[3 1 2]),[],4);
for i=1:size(d.edges,1)
    v1=d.edges(i,1); v2=d.edges(i,2);
    inc1=find(((d.edges(:,1)==v1) | (d.edges(:,2)==v1)) & d.edges(:,2)~=v2);
    inc2=find(((d.edges(:,1)==v2) | (d.edges(:,2)==v2)) & d.edges(:,1)~=v1);
    for j=1:length(inc1), for k=1:length(inc2),
            if d.edges(inc1(j),1)==v1, v0=d.edges(inc1(j),2); swap=0; else v0=d.edges(inc1(j),1); swap=1; end
            if d.edges(inc2(k),1)==v2, v3=d.edges(inc2(k),2); else v3=d.edges(inc2(k),1); swap=1-swap; end
            dih=ttable([v0 v1 v2 v3])';
            ind=ceil(.5*find(all((test==repmat(dih,size(test,1),1)) | test==0,2), 1, 'last' )); inde=ind;
            while ind>1, if ~any(diff(p.dihedrals([ind ind-1],1:4))), ind=ind-1; else break; end; end
            nd=nd+1+inde-ind; o.dihedrals(nd+(ind-inde:0),:)=[repmat(double([v0 v1 v2 v3]),1+inde-ind,1) p.dihedrals(ind:inde,5:end)];
    end;end;
end
perms=flipud([1 2 3 4;1 4 3 2;2 1 3 4;2 4 3 1;4 1 3 2;4 2 3 1])';
test=reshape(p.improper(:,perms(:))',4,[])';
if any(mod(p.improper(:,6),180) | mod(p.improper(:,6),2)), error('I need to work out how to flip signs of dihedrals in the nontrivial cases!'); end
for i=1:size(d.types,1)
    inc=find((d.edges(:,1)==i) | (d.edges(:,2)==i));
    if length(inc)==3,
        [sd,ind2]=setdiff(reshape(d.edges(inc,1:2)',1,[]),i); verts=[sd(1:2) i sd(3)]; 
        ind=find(all((test==repmat(ttable(verts)',size(test,1),1)) | (test==0),2));
        if ~isempty(ind),
            junk=[1 2 0 3];junk=junk(perms([1 2 4],mod(ind(end)-1,6)+1));junk(junk)=1:3;edges=inc(ceil(ind2(junk)/2))';
            nd=nd+1; o.dihedrals(nd,:)=[double(setdiff(d.edges(edges(1),1:2),d.edges(edges(2),1:2))) d.edges(edges(2),1:2) setdiff(d.edges(edges(3),1:2),d.edges(edges(2),1:2)) 1 p.improper(ceil(ind(end)/6),5:7)];
        end
    end
end
o.dihedrals=[o.dihedrals(:,1:4) o.dihedrals(:,6)./o.dihedrals(:,5) o.dihedrals(:,7) abs(o.dihedrals(:,8))];
o.vdw=[p.VDW(ttable,:) d.charges];
