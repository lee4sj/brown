function p=loadphys
f=fopen('parm10mod12sb.dat','rt');
%f=fopen('parm99mod03.dat','rt');
%f=fopen('parm99_papermod.dat','rt');%this is used in the trpcage thing; above choice is better!
junk=fgetl(f);
tt=0; p.types=[]; p.weight=[]; p.polarizability=[];
while 1,
    temp=fgetl(f); if length(temp), temp=textscan(temp,'%s %f %f',1); else break; end
    if ~length(temp{1}), break; end
    tt=tt+1; p.types=[p.types; temp{1}];
    p.weight=[p.weight; temp{2}];
    if ~length(temp{3}), temp{3}=-inf; end, p.polarizability=[p.polarizability;temp{3}];
end
temp=fgetl(f); temp=textscan(temp,'%s');
p.hydrophilic=[]; for i=1:length(temp{1}), p.hydrophilic=[p.hydrophilic;strmatch(temp{1}{i},p.types,'exact')]; end

p.bonds=[];
while 1,
    temp=fgetl(f); if length(temp), temp=textscan(temp,'%[^-]-%s %f %f',1); else break; end
    if ~length(temp{1}), break; end
    p.bonds=[p.bonds;strmatch(temp{1},p.types,'exact') strmatch(temp{2},p.types,'exact') temp{3} temp{4}];
end

p.angles=[];
while 1,
    temp=fgetl(f); if length(temp), temp=textscan(temp,'%[^-]-%[^-]-%s %f %f',1); else break; end
    if ~length(temp{1}), break; end
    p.angles=[p.angles;strmatch(temp{1},p.types,'exact') strmatch(temp{2},p.types,'exact') strmatch(temp{3},p.types,'exact') temp{4} temp{5}];
end

p.dihedrals=[]; tx=[{'X'};p.types];
while 1,
    temp=fgetl(f); if length(temp), temp=textscan(temp,'%[^-]-%[^-]-%[^-]-%s %f %f %f %f',1); else break; end
    if ~length(temp{1}), break; end
    p.dihedrals=[p.dihedrals;strmatch(temp{1},tx,'exact') strmatch(temp{2},tx,'exact') strmatch(temp{3},tx,'exact') strmatch(temp{4},tx,'exact') temp{5} temp{6} temp{7} temp{8}];
end; p.dihedrals(:,1:4)=p.dihedrals(:,1:4)-1;

p.improper=[];
while 1,
    temp=fgetl(f); if length(temp), temp=textscan(temp,'%[^-]-%[^-]-%[^-]-%s %f %f %f',1); else break; end
    if ~length(temp{1}), break; end
    p.improper=[p.improper;strmatch(temp{1},tx,'exact') strmatch(temp{2},tx,'exact') strmatch(temp{3},tx,'exact') strmatch(temp{4},tx,'exact') temp{5} temp{6} temp{7}];
end; p.improper(:,1:4)=p.improper(:,1:4)-1;

fgetl(f);fgetl(f); %hardcoded; ignoring 10-12
equiv=1:size(p.types,1);
while 1,
    temp=fgetl(f); if ~isempty(temp), temp=textscan(temp,'%s'); else break; end
    if isempty(temp{1}) break; end
    j=strmatch(temp{1}{1},p.types,'exact');
    for i=2:length(temp{1}),
        equiv(strmatch(temp{1}{i},p.types,'exact'))=j;
    end
end
junk=fgetl(f); temp=textscan(junk,'%s %s',1); if ~strcmp(temp{2},'RE'); error('Couldn''t find RE!'); end
p.VDW=zeros(size(p.types,1),2);
while 1,
    temp=fgetl(f); if ~isempty(temp), temp=textscan(temp,'%s %f %f',1); else break; end
    if isempty(temp{1}) break; end
    p.VDW(strmatch(temp{1},p.types,'exact'),:)=[temp{2} temp{3}];
end
p.VDW=p.VDW(equiv,:);
fclose(f);