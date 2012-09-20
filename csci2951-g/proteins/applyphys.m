function o=applyphys(d,p,water)
o.bonds=zeros(size(d.edges,1),4);
ttable=zeros(size(d.types)); for i=1:length(d.types), ttable(i)=strmatch(d.types{i},p.types,'exact'); end
ettable=ttable(d.edges);
for i=1:size(d.edges,1),
    ind=find(all(repmat(ettable(i,:),size(p.bonds,1),1)==p.bonds(:,1:2),2));
    if ~length(ind), ind=find(all(repmat(ettable(i,2:-1:1),size(p.bonds,1),1)==p.bonds(:,1:2),2)); end
    o.bonds(i,:)=[double(d.edges(i,:)) p.bonds(ind,3:4)];
end
o.weights=p.weight(ttable);
na=1;
for i=1:size(d.types,1)
    inc1=find(d.edges(:,1)==i); inc2=find(d.edges(:,2)==i); inca=[inc1;inc2];
    inc=[d.edges(inc1,2);d.edges(inc2,1)];
    for j=1:length(inc)-1, for k=j+1:length(inc),
            ang=ttable([inc(j) i inc(k)]);
            ind=find(all(repmat(ang',size(p.angles,1),1)==p.angles(:,1:3),2));
            if ~length(ind), ind=find(all(repmat(ang(3:-1:1)',size(p.angles,1),1)==p.angles(:,1:3),2)); end
            o.angles(na,:)=[inca([j k])' p.angles(ind,4:5)];
            if (j<=length(inc1))~=(k<=length(inc1)), o.angles(na,4)=180-o.angles(na,4); end
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
            ang1=find(all(o.angles(:,1:2)==repmat([inc1(j) i],size(o.angles,1),1),2));
            if isempty(ang1), ang1=find(all(o.angles(:,2:-1:1)==repmat([inc1(j) i],size(o.angles,1),1),2)); end
            ang2=find(all(o.angles(:,1:2)==repmat([i inc2(k)],size(o.angles,1),1),2));
            if isempty(ang2), ang2=find(all(o.angles(:,2:-1:1)==repmat([i inc2(k)],size(o.angles,1),1),2)); end
            nd=nd+1+inde-ind; o.dihedrals(nd+(ind-inde:0),:)=[repmat([ang1 ang2],1+inde-ind,1) p.dihedrals(ind:inde,5:end)];
%            for k=nd+(ind-inde:0), if mod(o.dihedrals(k,6),2)==0 || mod(swap+sum(o.angles([ang1 ang2],1)==i),2)==0, o.dihedrals(k,5)=180-o.dihedrals(k,5); end;end
            for k=nd+(ind-inde:0), if o.angles(ang1,1)~=i, o.dihedrals(k,5)=-o.dihedrals(k,5); end; if mod(o.dihedrals(k,6),2)==0 || mod(swap+sum(o.angles([ang1 ang2],1)==i),2)==0, o.dihedrals(k,5)=180+o.dihedrals(k,5); end;end
%            for k=nd+(ind-inde:0), if mod(o.dihedrals(k,6),2)==0, o.dihedrals(k,5)=-o.dihedrals(k,5);end;if mod(swap+sum(o.angles([ang1 ang2],1)==i),2)==0, o.dihedrals(k,5)=180+o.dihedrals(k,5); end;end
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
            ang1=find(all(o.angles(:,1:2)==repmat(edges(1:2),size(o.angles,1),1),2));
            if isempty(ang1), ang1=find(all(o.angles(:,2:-1:1)==repmat(edges(1:2),size(o.angles,1),1),2)); end
            ang2=find(all(o.angles(:,1:2)==repmat(edges(2:3),size(o.angles,1),1),2));
            if isempty(ang2), ang2=find(all(o.angles(:,2:-1:1)==repmat(edges(2:3),size(o.angles,1),1),2)); end
            nd=nd+1; o.dihedrals(nd,:)=[ang1 ang2 -1 p.improper(ceil(ind(end)/6),5:7)];
        end
    end
end
%old way of doing improper dihedrals:
%ni=0;test=reshape(permute(cat(3,p.improper(:,1:4),p.improper(:,[2 1 3 4])),[3 1 2]),[],4);      foo3=['X';p.types];
% for i=1:size(d.types,1)
%     inc=find((d.edges(:,1)==i) | (d.edges(:,2)==i));
%     if length(inc)==3,
%         swap=(d.edges(inc,1)==i);
%         dih=ttable([d.edges(inc(1),1+swap(1)),d.edges(inc(2),1+swap(2)),i,d.edges(inc(3),1+swap(3))])';   foo2=0;
%         for j=1:3,
%             perm=[j:3 1:j-1]; perm(perm==3)=4; perm(4)=perm(3); perm(3)=3;
%             ind=ceil(.5*find(all((test==repmat(dih(perm),size(test,1),1)) | (test==0),2),1,'last'));
%             if ~isempty(ind), foo=[d.edges(inc(1),1+swap(1)),d.edges(inc(2),1+swap(2)),i,d.edges(inc(3),1+swap(3))];fprintf('%d ',foo(perm)*3-3);fprintf('%s ',foo3{1+test(ind*2-1,:)});fprintf('(%d)',ind);if (foo2==1), fprintf(' *'); end;fprintf('\n');foo2=1;
%                 ang=inc(mod((-1:0)+j,3)+1)'; ed=inc(mod(1+j,3)+1);
%                 ang1=find(all(o.angles(:,1:2)==repmat(ang,size(o.angles,1),1),2));
%                 if isempty(ang1), ang1=find(all(o.angles(:,2:-1:1)==repmat(ang,size(o.angles,1),1),2)); end
%                 ni=ni+1; o.improper(ni,:)=[ang1 ed p.improper(ind,5:7)];
%             end
%         end
%     end
% end

m=sparse(double(d.edges(:,1)),double(d.edges(:,2)),1,size(d.types,1),size(d.types,1))+speye(size(d.types,1));m=m+m';
m2=m*m; m3=m2*m;
[pairs14(:,1),pairs14(:,2)]=find(tril((m3>0)-(m2>0)));
[pairsfar(:,1),pairsfar(:,2)]=find(tril(~m3));
[pairsnear(:,1),pairsnear(:,2)]=find(tril(m2));
if nargin==3, if ischar(water) && strcmpi(water,'i'), water=-1; else water=1; end, else water=0; end
if water==1 %do new per-vertex force input
    o.pervertex.charges=d.charges; o.pervertex.vdw=p.VDW(ttable,:);
    o.pervertex.special=[pairsnear zeros(size(pairsnear,1),1);pairs14 ones(size(pairs14,1),1)];
else
    type14=reshape(ttable(pairs14),[],2); typefar=reshape(ttable(pairsfar),[],2);
    o.VDW=[pairs14 (p.VDW(type14(:,1),1)+p.VDW(type14(:,2),1)) .5*sqrt(p.VDW(type14(:,1),2).*p.VDW(type14(:,2),2)) 332.0636/1.2*prod(reshape(d.charges(pairs14),[],2),2);...
        pairsfar (p.VDW(typefar(:,1),1)+p.VDW(typefar(:,2),1)) sqrt(p.VDW(typefar(:,1),2).*p.VDW(typefar(:,2),2)) 332.0636*prod(reshape(d.charges(pairsfar),[],2),2)];
    o.VDW(:,3:4)=[12*o.VDW(:,4).*o.VDW(:,3).^12 12*o.VDW(:,4).*o.VDW(:,3).^6];
    %note in amber-thesis.pdf that water "shields" electrostatic interactions,
    %making coulomb exponent 2 instead of 1 (p. 214)
    if water==-1,
        junk=char(d.types);
        o.implicit.types=junk(:,1);
        o.implicit.charges=d.charges;
    end
end
o.big=find(d.backbone);
o=dihedralbump(o,d.seq,d.backbone);    