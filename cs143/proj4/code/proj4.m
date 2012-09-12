% Sliding window face detection with SVMs. 
% All code by James Hays, except for pieces of evaluation code from Pascal
% VOC toolkit. Images from CMU+MIT face database, CalTech Web Face
% Database, and SUN scene database.

% Code structure:
% proj4.m
%  + load_crops.m
%  + crops2features.m   <-- Your code here!
%  <--- start hard negative mining / cascade building loop --->
%  + get_random_negatives.m <-- first iteration only
%    + rescale_max_size.m
%    + bboxes2crops.m
%  + get_hard_negatives.m  <-- Your code here!
%  + primal_svm.m
%  + report_accuracy.m
%  % adjust the classifier for the cascade <-- Your code here!
%  <--- end hard negative mining / cascade building loop --->
%  + run_detector.m
%    + get_detections_all_scales.m
%      + get_detections_single_scale.m
%        + get_img_crops.m
%    + non_max_supr.m
%  + evaluate_all_detections.m
%    + VOCap.m
%  + visualize_detections.m

% Other functions:
% test_primal_svm.m %Shows example usage of primal_svm.m for linear and
% non-linear SVM training and testing.
% kernel.m %Computes various types of kernel matrices. Use this to generate
% input to the non-linear SVM training and testing.

% Training and Testing data related functions:
% test_scenes/visualize_cmumit_database_landmarks.m
% test_scenes/visualize_cmumit_database_bboxes.m
% test_scenes/cmumit_database_points_to_bboxes.m %This function converts
% from the original MIT+CMU test set landmark points to Pascal VOC
% annotation format (bounding boxes).

% caltech_faces/caltech_database_points_to_crops.m %This function extracts
% training crops from the Caltech Web Face Database. The crops are
% intentionally large to contain most of the head, not just the face. The
% test_scene annotations are likewise scaled to contain most of the head.

% Format of the classifier. If you decide to use a cascaded non-linear
% classifier in the spirit of Viola-Jones, 'cascade' will be a N x 2 or N x
% 3 cell array, where N is the number of cascade stages and each row
% contains the classifier. In the case of a linear SVM the cascade will be
% N x 2, where the first entry, w, is a d x 1 weight vector and the second
% entry, b, is a scalar. d is the dimensionality of the feature. Together
% these parameters define a hyperplane decision boundary. In the case of a
% non-linear classifier the cascade will be N x 3, where the first entry is
% the nx1 coefficients to the support vectors, the second entry is a scalar
% offset, and the third entry is the support vectors (training data). If
% you are not using a cascade architecture, cascade will simply be a 1x2 or
% 1x3 cell array.

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

train_path_pos = '/course/cs143/data/proj4/caltech_faces/Caltech_CropFaces'; %Positive training examples

%this value must match the positive training data
patch_size = 36;

non_face_scn_path = '/course/cs143/data/proj4/train_non_face_scenes'; %We can mine random or hard negatives from here

test_scn_path = '/course/cs143/data/proj4/test_scenes/test_jpg'; %Test scenes


%% Load positive training examples and initial negative examples
% The positive examples will remain fixed, the negatives will be replaced
% with each iteration.

%load the raw image patches
crops_pos = load_crops(train_path_pos);

%convert those patches to the feature representation that the classifier
%will see. crop2features is initially an identify transformation, simply
%returning the raw patches.
all_feats_pos = crops2features(crops_pos);

% feats_pos_test = crops2features(crops_pos_test);
% feats_neg_test = crops2features(crops_neg_test);

global X; % X is a global variable which will be used by primal_svm()
global K; % K is a global variable used for non-linear svms.
global param; %struct with kernel parameters for non-linear svm.

% 'cascade' will contain the actual classifier (even if you don't build a
% cascade).
cascade   = cell(0,3); %we can dynamically resize this as we grow it. 
cur_stage    = 1; %stages of cascade, or stages of hard negative mining
total_stages = 7;
tp_rates = zeros(total_stages, 1); %training true positive rates
fp_rates = zeros(total_stages, 1); %training false positive rates
tn_rates = zeros(total_stages, 1); %training true negative rates
fn_rates = zeros(total_stages, 1); %training false negative rates
num_negative_examples = 1000; %also limits amount of positive training data used.

total_fp_rate = 1;

while(cur_stage <= total_stages)
%% step 1. Mine hard negatives by finding false positives with the cascade
% if this is the first iteration, get random negatives
    if(cur_stage == 1)
        crops_neg = get_random_negatives(non_face_scn_path, patch_size, num_negative_examples);
        feats_neg = crops2features(crops_neg);
    else
        crops_neg = get_hard_negatives(non_face_scn_path, cascade, patch_size, linear, num_negative_examples);
        % feats_neg = crops2features(crops_neg); 
        %you may want to combine the new negative examples with the
        %previous negative features rather than replace them. e.g.
        feats_neg = [feats_neg; crops2features(crops_neg)];
    end
    
%% step 2. Train Classifier and (optionally) add it to cascade.
    % ideally you would use all positive training data. This may not be
    % possible with the non-linear classifier (unless you use a cascade
    % architecture).
    fprintf('Randomly subsampling positive examples\n')
    feats_pos = all_feats_pos(randperm(size(all_feats_pos,1)) <= num_negative_examples,:);
    
    X = single([feats_pos; feats_neg]);
    label_vector = [ones(size(feats_pos,1),1); -1*ones(size(feats_neg,1),1)];

    linear = true;
    % see primal_svm.m and test_primal_svm.m for more info on SVM usage
	if(linear)
        lambda = 30 ; %If you change feature dimensionality / magnitude, this parameter might need to change.
        [w, b0 ] = primal_svm( linear, label_vector, lambda); 
        cascade{cur_stage,1} = w;
        cascade{cur_stage,2} = b0;
        
        %visualize the classifier. This function will only work for image
        %patch features.
        %visualize_cascade(cascade)
    else
        lambda = 1.0;
        param.type = 'rbf';
        param.sig = 1.0; 
        K = kernel(X, X, param);
        % figure(99)
        % imagesc(K) %should have a variety of values -- not nearly all 1's or 0's
        % pause
        [beta, b0] = primal_svm( linear, label_vector, lambda);
        cascade{1,1} = beta;
        cascade{1,2} = b0;
        cascade{1,3} = X;
    end
       

%% step 3. (if cascade) Adjust the classifier threshold to reach desired False Positive rate

    if(linear)
        confidences = X*cascade{1,1} + cascade{1,2};
    else
        confidences = K*cascade{1,1} + cascade{1,2};
    end
    
    %measure accuracy on the training data
    fprintf('Initial classifier performance on train data:\n')
    [tp_rates(cur_stage), fp_rates(cur_stage), tn_rates(cur_stage), fn_rates(cur_stage)]  = ...
        report_accuracy( confidences, label_vector );
    
    %visualize how well separated the positive and negative examples are at
    %training time.
    non_face_confs = confidences( label_vector < 0);
    face_confs     = confidences( label_vector > 0);
    figure; 
    plot(sort(face_confs), 'g'); hold on
    plot(sort(non_face_confs),'r'); 
    plot([0 size(non_face_confs,1)], [0 0], 'b');
    hold off;

    % %if building a cascade, we want each classifier to be more
    % conservative and have a high false positive rate and low false
    % negative rate
    if(cur_stage~=total_stages)
        cascade{cur_stage, 2} = cascade{cur_stage, 2};
        %re-examine the training errors. measure accuracy on the training data
        if(linear)
            confidences = X*cascade{cur_stage,1} + cascade{cur_stage,2};
        else
            confidences = K*cascade{cur_stage,1} + cascade{cur_stage,2};
        end
        fprintf('Adjusted classifier performance on train data:\n')
        [tp_rates(cur_stage), fp_rates(cur_stage), tn_rates(cur_stage), fn_rates(cur_stage)]  = ...
           report_accuracy( confidences, label_vector );
       
       threshhold = 0.000001;
       if (total_fp_rate < threshhold)
           break;
       end
    end
    
    cur_stage = cur_stage + 1;
end

cur_stage-1
cascade{cur_stage-1, 2} = cascade{cur_stage-1, 2} + 4;
save('cascade.mat', 'cascade')

%% Evaluate on test set.
% If your recall rate is low and your detector still has high precision at
% its highest recall point, you can improve your average precision by
% reducing the threshold for a positive detection. The easiest way to do
% this is to increase the constant offset in the classifier. (cascade
% column 2, by default).
[bboxes, confidences, image_ids] = run_detector(test_scn_path, cascade, patch_size, linear);

% Training statistics. Remember, testing accuracy is what really matters,
% but these statistics can still help diagnose problems.
for i = 1:total_stages
    fprintf('Stage %d. TPR: %.3f, FPR: %.3f, TNR: %.3f, FNR: %.3f\n', ...
        i, tp_rates(i), fp_rates(i), tn_rates(i), fn_rates(i))
end

figure(12)
evaluate_all_detections(bboxes, confidences, image_ids);

figure(13)
visualize_detections(bboxes, confidences, image_ids);


