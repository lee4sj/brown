function [h]=plot_eval_multidir(dirs,colors,names)
% plot evaluation results.
% Pablo Arbelaez <arbelaez@eecs.berkeley.edu>
% modified by geoff to plot from multiple dirs

numdir=length(dirs);
if (numdir~=length(colors))
    error('number of dirs and colors must be the same...');
end

if (numdir+4~=length(names))
    error('number of names wrong...');
end

h=open('isoF3.fig');
hold on;

for ii=1:numdir
    
    evalDir=dirs{ii};
    col=colors{ii};

    fwrite(2,sprintf('\n%s\n',evalDir));

    if exist(fullfile(evalDir,'eval_bdry_thr.txt'),'file'),

        prvals = dlmread(fullfile(evalDir,'eval_bdry_thr.txt')); % thresh,r,p,f
        f=find(prvals(:,2)>=0.01);
        prvals = prvals(f,:);


        evalRes = dlmread(fullfile(evalDir,'eval_bdry.txt'));
        if size(prvals,1)>1,
            plot(prvals(1:end,2),prvals(1:end,3),col,'LineWidth',3);
        else
            plot(evalRes(2),evalRes(3),'o','MarkerFaceColor',col,'MarkerEdgeColor',col,'MarkerSize',8);
        end


        fprintf('Boundary\n');
        fprintf('ODS: F( %1.2f, %1.2f ) = %1.2f   [th = %1.2f]\n',evalRes(2:4),evalRes(1));
        fprintf('OIS: F( %1.2f, %1.2f ) = %1.2f\n',evalRes(5:7));
        fprintf('Area_PR = %1.2f\n\n',evalRes(8));
    end

    if exist(fullfile(evalDir,'eval_cover.txt'),'file'),
        evalRes = dlmread(fullfile(evalDir,'eval_cover.txt'));
        fprintf('Region\n');
        fprintf('GT covering: ODS = %1.2f [th = %1.2f]. OIS = %1.2f. Best = %1.2f\n',evalRes(2),evalRes(1),evalRes(3:4));
        evalRes = dlmread(fullfile(evalDir,'eval_RI_VOI.txt'));
        fprintf('Rand Index: ODS = %1.2f [th = %1.2f]. OIS = %1.2f.\n',evalRes(2),evalRes(1),evalRes(3));
        fprintf('Var. Info.: ODS = %1.2f [th = %1.2f]. OIS = %1.2f.\n',evalRes(5),evalRes(4),evalRes(6));
    end
end
legend('','',names,'Location','SouthWest');

