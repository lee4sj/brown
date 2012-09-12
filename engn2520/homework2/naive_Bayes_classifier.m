function [ output_  args ] = naive_Bayes_classifier( input_args )
%NAIVE_BAYES_CLASSIFIER Summary of this function goes here
%   Detailed explanation goes here


%prepare training examples
load('digits');

numclasses = 10;
feature_size = 28*28;

train{1} = train1;
train{2} = train2;
train{3}= train3;
train{4} = train4;
train{5} = train5;
train{6} = train6;
train{7} = train7;
train{8} = train8;
train{9} = train9;
train{10} = train0;

u = zeros(numclasses, feature_size);
var = zeros(numclasses, feature_size);

%find mean and variance for each digit
A = [];
B = [];
blank = ones(28, 28);
for i=1:10
    for j=1:size(train{i}, 1)
        u(i, :) = u(i, :) + train{i}(j, :);
    end
    u(i, :) = u(i, :) / size(train{i}, 1);
    var(i, :) = u(i, :) .* (1 - u(i, :));
    
    if (i <= 5)
        A = [A blank reshape(u(i, :), 28, 28)];
    else
        B = [B blank reshape(u(i, :), 28, 28)];
    end
end

%bound u
epsilon = .1;
for i=1:10
    for j=1:size(u(i,:), 2)
        if u(i,j) < epsilon
            u(i,j) = epsilon;
        end
        if u(i,j) > 1 - epsilon
            u(i, j) = epsilon;
        end
    end
end

bigblank = ones(28, 28*10);
figure, imshow([A; bigblank; B])

%test data
test{1} = test1;
test{2} = test2;
test{3}= test3;
test{4} = test4;
test{5} = test5;
test{6} = test6;
test{7} = test7;
test{8} = test8;
test{9} = test9;
test{10} = test0;

confusion = zeros(10, 10);

for l=1:10
    for k=1:size(test{l}, 1)
        a = zeros(numclasses, 1);
        for i=1:10
            for j=1:size(u, 2)
                x = test{l}(k, j);
                a(i) = a(i) + x*log(u(i, j)) + (1-x)*log(1-u(i,j));
            end
        end
        [val class] = max(a);
        confusion(l, class) = confusion(l, class) + 1;
    end
end

confusion



end

