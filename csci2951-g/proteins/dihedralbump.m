function p2=dihedralbump(p2,seq,backbone,sc)
if nargin==3, sc=1; end
amino=sort({'ala','gly','ser','thr','leu','ile','val','asn','gln','arg','trp','phe','tyr','lys','pro','cys','met','asp','glu'});
pr=repmat([21 25 1.5],19,1);pr(1,[1 3])=[25 .6];pr(3,[1 3])=[21 .8];pr(4,3)=1;pr(8,3)=0;pr(9,[1 3])=[24 2];pr(11,3)=2;pr(13,3)=2.5;pr(15,3)=.5;pr(16,[1 3])=[25 1];pr(19,[1 3])=[25 2];pr(17:18,3)=2.5;
for i=1:length(amino), dat.(amino{i})=[(50/2/pi)^2/pr(i,2) mod(pr(i,1)*7.2-126+180,360) -sc*pr(i,3)];end, %second param must be between 0 and 360!
foo=sort(reshape(p2.bonds(p2.angles(p2.dihedrals(:,1:2),1:2),1:2),[],8),2);
f=find(all(backbone(foo),2)); g=foo(f,:);
bb=find(backbone);
nins=0;ins=[];insi=[];
for i=1:length(bb)/3-1,
    s=dat.(seq{i}(end-2:end));
    j=f(find(g(:,1)==bb(i*3-2),1)); if p2.dihedrals(j,6)<-50 p2.dihedrals(j,3:end)=[1 s(1)/(s(3)-100) s(2) s(3)-100]; else nins=nins+1; insi(nins)=j; ins(nins,:)=[1 s(1)/(s(3)-100) s(2) s(3)-100]; end
end
[j,k]=sort(insi);
for i=length(j):-1:1,
    p2.dihedrals=[p2.dihedrals(1:j(i)-1,:);[p2.dihedrals(j(i),1:2) ins(k(i),:)];p2.dihedrals(j(i):end,:)];
end
