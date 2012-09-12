function K = kernel(X1, X2, param)
%code by Antonio Torralba and James Hays, 2009

n1 = size(X1,1);
n2 = size(X2,1);

if(~exist('param', 'var'))
    fprintf('defaulting to rbf kernel\n')
    param.type = 'rbf';
end

switch param.type
    case 'linear'
        K = X1*X2';
        
    case 'rbf'
        norm1 = sum(X1.^2,2);
        norm2 = sum(X2.^2,2);
        dist = (repmat(norm1 ,1,size(X2,1)) + ...
            repmat(norm2',size(X1,1),1) - ...
            2*X1*X2');
        if(~isfield(param, 'sig'))
            param.sig=sqrt(mean(dist(:))/2);
        end
        K = exp(-0.5/param.sig^2 * dist);

    case 'dist'
        norm1 = sum(X1.^2,2);
        norm2 = sum(X2.^2,2);
        dist = (repmat(norm1 ,1,size(X2,1)) + ...
            repmat(norm2',size(X1,1),1) - ...
            2*X1*X2');
        K = param.maxdist - dist;
        
    case 'Lp'
        K = zeros(n1,n2);
        for n = 1:n1
            K(n,:) = real(sum(bsxfun(@minus, X2, X1(n,:)).^param.p,2));
        end
        K = exp(-0.5/param.sig^2 * K);

    case 'L1'
        %this is not guaranteed to generate a true kernel
        K = zeros(n1,n2);
        for n = 1:n1
            K(n,:) = real(sum(abs(bsxfun(@minus, X2, X1(n,:))),2));
        end
        K = max(max(K)) - K;
        
    case 'L1_exp'
        K = zeros(n1,n2);
        for n = 1:n1
            K(n,:) = real(sum(abs(bsxfun(@minus, X2, X1(n,:))),2));
        end

        if(~isfield(param, 'sig'))
            param.sig=mean(K(:))/2;
        end
        K = exp(-0.5/param.sig * K);

    case 'histintersection'
        
    case 'chisqr'
        %this is not guaranteed to generate a true kernel
        %requires mex'd chisqr implementation
        X1 = X1';
        X2 = X2';
        %X1 = X1./repmat(sum(X1,1)+eps, size(X1,1), 1); %normalize each column
        %X2 = X2./repmat(sum(X2,1)+eps, size(X2,1), 1); %normalize each column
        
        if(~isa(X1,'single') || ~isa(X2, 'single'))
            error('chisqr inputs must be single for speed reasons.\n')
        end

        K = chi2_mex_single(X2, X1);
        K = K';
        K = max(max(K)) - K;
%         figure
%         imagesc(K)
%         opts.disp = 0;
%         [v, d] = eigs(double(K),1,'SA', opts); %this gets the smallest eigenvector (most negative)
%         d
%         pause
    case 'chisqr_exp'
        X1 = X1';
        X2 = X2';
        %X1 = X1./repmat(sum(X1,1)+eps, size(X1,1), 1); %normalize each column
        %X2 = X2./repmat(sum(X2,1)+eps, size(X2,1), 1); %normalize each column
        
        if(~isa(X1,'single') || ~isa(X2, 'single'))
            error('chisqr inputs must be single for speed reasons.\n')
        end

        K = chi2_mex_single(X2, X1);
        K = K';
        if(~isfield(param, 'sig'))
            param.sig=mean(K(:));
        end
        K = exp(-0.5/param.sig * K);
        
    otherwise
        error('Unknown kernel');
end;

