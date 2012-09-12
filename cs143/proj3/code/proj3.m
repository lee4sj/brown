%Stencil code prepared by Sam Birch and James Hays for CS 143, Brown

%{
First set up paths to VLFeat functions. 
See http://www.vlfeat.org/mdoc/mdoc.html for VLFeat Matlab documentation
%}
switch computer %see 'help computer' for explanation of this
    case 'GLNX86'
        addpath vlfeat/toolbox/mex/mexglx
    case 'GLNXA64'
        addpath vlfeat/toolbox/mex/mexa64
    case 'MACI'
        addpath vlfeat/toolbox/mex/mexmaci
    case 'MACI64'
        addpath vlfeat/toolbox/mex/mexmaci64
    case 'PCWIN'
        addpath vlfeat/toolbox/mex/mexw32
    case 'PCWIN64'
        addpath vlfeat/toolbox/mex/mexw64
end


%% Step 1: Establish a vocabulary of visual words through feature clustering.
% this step only needs to be performed once, unless you change the
% parameters of the vocabulary

data_path = '../data/';%'/course/cs143/data/proj3/'; %change if you want to work with a local copy
labels = real_dirs( fullfile( data_path, 'train')); %Each of these directories is a category
num_classes = length(labels); %number of scene categories.
num_train_per_class = 100; %number of training examples per class to use. Max is 100.
training_images = cell(num_classes * num_train_per_class, 1);   %This will contain the path of each training image
ctr = 1;

for i=1:num_classes
   images = dir( fullfile(data_path, 'train', labels{i}, '*.jpg') );
   for j=1:num_train_per_class
       training_images{ctr} = fullfile(data_path, 'train', labels{i}, images(j).name);
       ctr = ctr + 1;
   end
end

% You implement this function! (see build_vocabulary.m)
vocab_size = 400;
vocab = build_vocabulary(training_images, vocab_size);

% if you want to speed up debugging you can save and load variables like
% this:
save('vocab.mat', 'vocab') %also 'help load'
% load('vocab.mat', 'vocab');
size(vocab),

%% Step 2: Convert all training images into the histogram representation.

train_data = zeros(num_classes * num_train_per_class, vocab_size);
label_data = cell( num_classes * num_train_per_class, 1);
ctr = 1;

for i=1:num_classes
   disp(labels{i});
   images = dir( fullfile(data_path, 'train', labels{i}, '*.jpg') );
   for j=1:num_train_per_class
       image = fullfile(data_path, 'train', labels{i}, images(j).name);
       % You implement make_hist()! (See make_hist.m)
       histogram = make_hist(image, vocab, vocab_size);
       train_data(ctr,:) = histogram;
       label_data{ctr,1} = labels{i};
       ctr = ctr + 1;
   end
end

save('hist.mat', 'train_data');
save('label.mat', 'label_data');

% load('hist.mat', 'train_data');
% load('label.mat', 'labe_data');


%% Step 3: Learn a set of one-vs-all classifiers (SVMs) from the training histograms

models = cell(length(labels),2); %this will store the parameters of the SVM for each one-vs-all classifier

global X; % X is a global variable which will be used by primal_svm()
X = train_data; 

for class_idex=1:num_classes
    cur_label_vector = strcmp( labels{class_idex}, label_data ); %indicates which training instances are positive for this classifier
    cur_label_vector = cur_label_vector * 2 - 1; %transform from [0,1] to [-1, 1];
    
    %%%%%%%  SVM Training function %%%%%%%
    % you can try different parameters and non-linear kernels. Using
    % non-linear kernels will require changes in Step 4, as well.
    % see primal_svm.m and test_primal_svm.m for more info on SVM usage
    linear = 1;
    lambda = 0.1;
    [w,   b0 ] = primal_svm( linear, cur_label_vector, lambda); 
    
    models{class_idex}{1} = w; %store the learned model for each 1 vs all classifier.
    models{class_idex}{2} = b0;
    
    %if memory becomes problematic, you can integrate steps 3 and 4 and not
    %retain the learned models. Memory won't be an issue with linear SVMs
    %which have a compact model.
end

%% Step 4: Classify each test image and build a confusion matrix

% Use the models you just trained to classify the test data (classify
% an image with the classifier that gave the highest confidence.) Use this
% data to construct a confusion matrix: for each row (representing a class)
% give the frequency that an image was classified in each of the
% categories (as such, an identity matrix is the ideal case.) You should
% expect diagonals roughly 60-95% (depending on the class).

confusion_matrix = zeros(num_classes, num_classes);

for i=1:num_classes
   disp(labels{i});
   images = dir( fullfile(data_path, 'test', labels{i}, '*.jpg') );
   for j=1:num_train_per_class %this is the number of test images. It doesn't need to equal the number of training images.
      image = fullfile(data_path, 'test', labels{i}, images(j).name);
      histogram = make_hist(image, vocab, vocab_size);
      confidences = zeros(num_classes,1);
      for k=1:num_classes
         confidences(k) = histogram'*models{k}{1} + models{k}{2};
      end
      [conf, ind] = max(confidences);
      confusion_matrix(i, ind) = confusion_matrix(i, ind) + 1;
      if(ind == i)
          %correct classification
      else
          %incorrect classification
      end
   end
end
    

%% Step 5: Visualization and scoring

% If we wanted to do this properly we would train and test on many (at
% least 10) random splits of the data. You are not required to do so
% for this project, although if you want to win the "best performance"
% conteset you will need to.

fig_handle = figure; 
imagesc(confusion_matrix); 
axis_handle = get(fig_handle, 'CurrentAxes');
set(axis_handle, 'XTick', 1:15)
set(axis_handle, 'XTickLabel', ...  %hard coded label order may be wrong on some systems
      {'Sub', 'For', 'IC', 'OC', 'TB', ...
       'Bed', 'Kit', 'Sto', 'Cst', 'HW', ...
       'Mnt', 'St', 'Off', 'Ind', 'LR'})
set(axis_handle, 'YTick', 1:15)
set(axis_handle, 'YTickLabel', ...
      {'Suburb', 'Forest', 'InsideCity', 'OpenCountry', 'TallBuilding', ...
       'Bedroom', 'Kitchen', 'Store', 'Coast', 'Highway', ...
       'Mountain', 'Street', 'Office', 'Industrial', 'Livingroom'})

%if you change the relative amounts of training and testing data, the
%denominator needs to change here.
accuracy = mean(diag(confusion_matrix)) / num_train_per_class

% Interpreting your performance:
%  accuracy  =   0 -> Your code is broken (probably not the classifier's
%                     fault! A classifier would have to be amazing to
%                     perform this badly)
%  accuracy ~= .07 -> Your performance is chance. Something is broken.
%  accuracy >= .60 -> You've gotten things roughly correct.
%  accuracy >= .70 -> You've got the base pipeline correct and tuned
%                     parameters very well.
%  accuracy >= .80 -> You've added in spatial information somehow or you've
%                     added additional, complementary image features. This
%                     represents state of the art in Lazebnik et al 2006.
%  accuracy -> .85 -> You've done something really amazing. This is the
%                     state of the art in the SUN database paper from
%                     fusing many features. Don't trust this number unless
%                     you actually measure many random splits.
%  accuracy -> .96 -> You can beat a human at this task.




