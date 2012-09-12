function visualize_cascade(cascade)
%creates figures visualizing each level of the cascade. 
%only works for patches or normalized patch features.
%only works for square patches

num_stages = size(cascade,1);

%only works for square patches
patch_size = sqrt( length( cascade{1,1} ));
fig_size = ceil(sqrt(num_stages));
figure(11)
close(11)
figure(11)

for i = 1:size(cascade,1)
    subplot(fig_size, fig_size, i, 'replace')
    w = cascade{i,1};
    fprintf('classifier mean: %f, std: %f. mean magnitude %f\n', mean(w), std(w), mean(abs(w)))
    cur_classifier_img = reshape(w, patch_size, patch_size);
    cur_classifier_img = cur_classifier_img - min(w);
    cur_classifier_img = cur_classifier_img / max(w);
    imshow(cur_classifier_img, 'InitialMagnification', 500)
end