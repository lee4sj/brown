%%setup

d=loaddat;
d2=loadprotein(d,{'nasn','leu','tyr','ile','gln','trp','leu','lys','asp','gly','gly','cpro'},'b');
p=loadphys;
p2=applyphys(d2, p, 'i');

out=proteins(d2.p,d2.edges,d2.types,p2);
