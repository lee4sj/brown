function d=loaddat;
%d=loaddat;d2=loadproteinalpha(d,{'nasn','leu','tyr','ile','gln','trp','leu
%','lys','asp','gly','gly','pro','ser','ser','gly','arg','pro','pro','pro','cser'});p=loadphys;p2=applyphys(d2,p);
prefixes={'','',''};
names1={'amino12.in','aminont12.in','aminoct12.in'};prefixes={'','N','C'};
%names1={'all_amino03.in','all_aminont03.in','all_aminoct03.in'};
%names1={'all_amino94.in','all_aminont94.in','all_aminoct94.in'};prefixes={'','N','C'};%above is better!
d=[];
for ii=1:length(names1)
    f=fopen(names1{ii},'rt');
    junk=fgetl(f);junk=fgetl(f);
    while 1,
        r=[];
        pos=length(d)+1;
        desc=fgetl(f); if strncmp(desc,'STOP',4), break; end; junk=fgetl(f);
        d{pos}.name=[prefixes{ii} fscanf(f,'%s',1)]; junk=fgetl(f); junk=fgetl(f);
        junk=fscanf(f,'%f\n',1); if junk~=0, error('cutoff not 0!'); end
%        while 1,
            i=1;
            while 1,
                temp=fgetl(f); if length(temp), temp=textscan(temp,'%d %s %s %c %d %d %d %f %f %f %f\n',1);else break; end
                if ~length(temp{1}), break; end
                r=[r;temp];
                i=i+1;
            end; i=i-1;
            p=zeros(i,3);
            p(2,1)=r{2,8};
            p(3,1)=p(2,1)-r{3,8}*cosd(r{3,9});
            p(3,2)=r{3,8}*sind(r{3,9});
            for j=4:i,
                vb=p(r{j,6},:)-p(r{j,5},:); vb=vb/sqrt(sum(vb.^2));
                v90=-cross(vb,p(r{j,7},:)-p(r{j,6},:)); v90=v90/sqrt(sum(v90.^2));
                v0=-cross(v90,vb);
                p(j,:)=p(r{j,5},:)+r{j,8}*(vb*cosd(r{j,9})+sind(r{j,9})*(cosd(r{j,10})*v0+sind(r{j,10})*v90));
            end
            edges=[(5:i)' cell2mat(r(5:end,5))];
            names=cellfun(@(x) x{1},r(:,2),'UniformOutput',0);
            types=cellfun(@(x) x{1},r(:,3),'UniformOutput',0);
            while 1,
                junk=fgetl(f);
                if length(junk)>=4,
                    if strmatch(junk(1:4),'DONE'); break; end
                    if strmatch(junk(1:4),'LOOP')
                        while 1,
                            temp=fgetl(f); if ~all(temp==32), temp=textscan(temp,'%s %s\n',1); else break; end
                            edges=[edges;strmatch(temp{1},names) strmatch(temp{2},names)];
                        end
                    end
                end
            end
            d{pos}.names=names(4:end); d{pos}.types=types(4:end); d{pos}.edges=edges-3; d{pos}.p=p(4:end,:)-repmat(p(3,:),size(p,1)-3,1);
            d{pos}.charges=cell2mat(r(4:end,11));d{pos}.backbone=(cell2mat(r(4:end,4))=='M');
            d{pos}.desc=desc(1:max(find(desc~=32)));d{pos}.struc=cell2mat(r(4:end,5:7))-3;
%        end
    end
    fclose(f);
end