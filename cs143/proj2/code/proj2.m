%=================================
% starting code for proj2: pb-lite
% general pipeline:
% - set up paths/parameters
% - for each test image, compute features, calculate pb
% - do evaluation
%
% the starting code finds pb by thresholding
% edge detection using sobel and canny edge detection
% your goal is to beat the baselines by using
% fancier image representations, and possibly
% attempt some learning-based approach, such as 
% using an SVM.
%
% more details can be found on the proj2 website.
%==================================



%=============================================================
% functions you would need to modify:
% 1. create_filter_bank.m
% 2. gen_oriented_masks.m
% 3. pb_lite.m
% 4. texton_img.m
% 5. get_gradient.m
% it is recommended you approach them in the above order
%=============================================================




%%
clear all;close all;

%% 
%=== set up ===
addpath(genpath('./bench/'));
testdir='../data/testset/'; %contains only 10 test images
%testdir='/course/cs143/asgn/proj2/BSDS500/BSDS500/data/images/test/'; %contains only 10 test images

files=dir([testdir,'*.jpg']); %all jpgs

groundtruthdir='/course/cs143/asgn/proj2/BSDS500/BSDS500/data/groundTruth/test/'; %need this for benchmark

%create dirs to store pb results
baselinedir1='../data/sobel_baseline/'; %thresholded sobel or canny edge
baselinedir2='../data/canny_baseline/'; %thresholded sobel or canny edge
mypbdir='../data/mypb/';         %your implementation
if (~exist(baselinedir1,'dir')) mkdir(baselinedir1); end
if (~exist(baselinedir2,'dir')) mkdir(baselinedir2); end
if (~exist(mypbdir,'dir')) mkdir(mypbdir); end


%%
close all;
%================================
% compute pb using thresholded 
% sobel responses
%================================
sobel_pb_stack=cell(1,length(files));
thresh=.08:.02:.3;
for f=1:length(files)
    fprintf('computing sobel baseline #%d out of %d\n',f,length(files));
    [a b]=strtok(files(f).name,'.');
    im=rgb2gray(im2double(imread(fullfile(testdir,files(f).name))));
    pb=sobel_pb(im,thresh);
    figure(1);imshow(pb);
    imwrite(pb,[baselinedir1,a,'.png'],'png'); %sobel thresholded pb
    sobel_pb_stack{f}=pb;
end


%%
close all;
%================================
% compute pb using thresholded 
% canny responses
%================================
canny_pb_stack=cell(1,length(files));
thresh=.1:.1:.7;
sigma=1:1:4;
for f=1:length(files)
    fprintf('computing canny baseline #%d out of %d\n',f,length(files));
    [a b]=strtok(files(f).name,'.');
    im=rgb2gray(im2double(imread(fullfile(testdir,files(f).name))));
    pb=canny_pb(im,thresh,sigma);
    figure(1);imshow(pb);
    imwrite(pb,[baselinedir2,a,'.png'],'png'); %sobel thresholded pb
    canny_pb_stack{f}=pb;
end



%%
%==========================
% set params for pb-lite
%==========================
close all;
k=64;                                     %num of clusters for textons
fb_norient=16;                            %num of orientations for the filter bank
orientations=linspace(0,360,fb_norient+1);%actual orientations
orientations(end)=[];                     
fb_sigmas=[1,2];                          %scales for filter bank (sigma for a gaussian kernel)
%===============================================================
[fb,fb_h]=create_filter_bank(orientations,fb_sigmas); %creat your own filter bank
print(fb_h,'-dpng','fb.png');





g_orients=orientations(1:fb_norient/2);      %num of orientations for gradient computation
g_radii=[5,10,20];                           %scales for gradient computation (radius of a disc)
%===============================================================
%           generate masks for each orientations/scales
[masks,masks_h]=gen_oriented_masks(g_orients,g_radii); %these will be helpful when we do gradients
print(masks_h,'-dpng','masks.png');

%%
close all;
%==========================
% compute pb-lite
%==========================
for f=1:length(files)
    fprintf('computing pb-lite #%d out of %d\n',f,length(files));
    [a b]=strtok(files(f).name,'.');
    im=im2double(imread(fullfile(testdir,files(f).name)));
    %=============================================================
    %
    %               your implementation of pb-lite
    %
    [pb]=pb_lite(im,k,fb,masks,sobel_pb_stack{f},canny_pb_stack{f});
    %==============================================================
    figure(1);imshow(pb);
    imwrite(pb,[mypbdir,a,'.png'],'png');
    %print(h,'-dpng',[num2str(f),'.png']); %save visualization
end

%%
%==========================
% evaluation againt human
% annotations
%==========================


%note:
% when running the following code, you only need to evaluate
% a particular type of pb (baseline or pb-lite) IF you modify
% the saved images or the way you compute them.
%
%warning:
% the evaluation might be a little slow (2~3 seconds per image)


%%
%get results from baseline 1
%run only when the baseline 1 images change

baseline1tmpdir = '../data/baseline1tmp/';
if (~exist(baseline1tmpdir,'dir')) mkdir(baseline1tmpdir); end
nthresh =10;
tic;
boundaryBench(testdir, groundtruthdir, baselinedir1, baseline1tmpdir, nthresh);
toc;


%%
%get results from baseline 2
%run only when the baseline 2 images change

baseline2tmpdir = '../data/baseline2tmp/';
if (~exist(baseline2tmpdir,'dir')) mkdir(baseline2tmpdir); end
nthresh =10;
tic;
boundaryBench(testdir, groundtruthdir, baselinedir2, baseline2tmpdir, nthresh);
toc;



%%
%get results from mypb
%run only when the pb images change
mypbtmpdir = '../data/mypbtmp/';
if (~exist(mypbtmpdir,'dir')) mkdir(mypbtmpdir); end
nthresh =10;
tic;
boundaryBench(testdir, groundtruthdir, mypbdir, mypbtmpdir, nthresh);
toc;


%%
%========== plot the PR curve =============
close all;
%number of colors and dirs must be the same
dirs{1}=baseline1tmpdir;
dirs{2}=baseline2tmpdir;
dirs{3}=mypbtmpdir;
colors={'g','m','k'};

%number of names should be 4+number of dirs
%these will appear in the legend of the plot
%the first four are precomputed and not evaluated on the fly
names={'human','contours','canny','gpb','baseline1(sobel)','baseline2(canny)','pb-lite'};
h=plot_eval_multidir(dirs,colors,names);
print(h,'-dpng','PR_curve.png');

%note the dotted lines are copied from figure 17 in the 2011 PAMI paper
%they are tuned/optimized to perform well than the baselines 
%given in the above code.