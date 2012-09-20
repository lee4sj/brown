function [dat,types]=allamino
d=loaddat;p=loadphys;
simple={'ala','gly','ser','thr','leu','ile','val','asn','gln','arg','trp','phe','tyr','lys','pro','cys','met','asp','glu'};
d2=loadprotein(d,simple);
[types,~,j]=unique(d2.types);
rt=['H';'C';'N';'O';'S'];rr=1.6+[1.2;1.7;1.55;1.52;1.8]; for i=1:length(types),r(i,1)=rr(strmatch(types{i}(1),rt));end
for i=1:length(simple),
    w=find(d2.residue==i);
    dat(i).p=d2.p(w,:)-repmat(mean(d2.p(w,:),1),length(w),1); dat(i).t=j(w); dat(i).r=r(dat(i).t);dat(i).name=simple{i};
end
    