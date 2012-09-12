function [ output_args ] = linear_basis_regression( input_args )
%LINEAR_BASIS_REGRESSION


% get data
train_x = load('Xtrain');
train_y = load('Ytrain');

max_degree = 4; %the maximum polynomial degree of the basis functions

%build phi
phi = zeros(size(train_x, 1), max_degree + 1);
for i=0:max_degree
    for j=1:size(train_x, 1)
        phi(j, i+1) = train_x(j)^i;
    end
end

%build Moore-Penrose pseudo-inverse of phi and find w
mppi = (phi' * phi) \ phi';
w = mppi * train_y;

%find train error
e = 0;
for i=1:size(train_x, 1)
    phi_x = zeros(max_degree + 1, 1);
    for j=0:max_degree
        phi_x(j+1, 1) = train_x(i)^j;
    end
    e = e + (train_y(i) - w'*phi_x)^2;
end
train_error = (1/2) * e;
train_error

%find test error
test_x = load('Xtest');
test_y = load('Ytest');
e = 0;
for i=1:size(train_x, 1)
    phi_x = zeros(max_degree + 1, 1);
    for j=0:max_degree
        phi_x(j+1, 1) = test_x(i)^j;
    end
    e = e + (test_y(i) - w'*phi_x)^2;
end
test_error = (1/2) * e;
test_error

p_min = min(train_x);
p_max = max(train_x);
plot_x = zeros(40, 1);
for i=1:40
    plot_x(i, 1) = p_min + (i * (p_max - p_min)/40);
end
plot_x   

y = zeros(20, 1);
for i=1:40
    phi_x = zeros(max_degree + 1, 1);
    for j=0:max_degree
        phi_x(j+1, 1) = plot_x(i)^j;
    end
    y(i) = w'*phi_x;
end


figure(1), scatter(train_x, train_y)
hold all, plot(plot_x, y)

figure(2), scatter(test_x, test_y)
hold all, plot(plot_x, y)

figure(3), plot(plot_x, y)

end

