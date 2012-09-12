function output_image = rescale_max_size(input_image, max_allowed_size, verbose)
%if all image dimensions are less than max_allowed_size, this function does
%nothing.  Otherwise it will scale the image down, uniformly, so that the
%largest image dimension has size max_allowed_size

%works for color or grayscale

%by James Hays

if ~exist('verbose', 'var')
    verbose = 0;
end

is_double = 0;
if(isa(input_image, 'double'))
    input_image = single(input_image);
    is_double = 1;
end

%why am I using round?  matlab's resize seems to be based on floor.
if(size(input_image,1) > max_allowed_size || size(input_image,2) > max_allowed_size)
    %1 if 1 is larger, 2 if 2 is larger

    new_size = round(max_allowed_size * [size(input_image,1)  size(input_image,2)] / ...
                                    (max(size(input_image,1), size(input_image,2))));

%     if(verbose)
%         fprintf('  Resizing image from %dx%d to %dx%d (using openCV)\n',...
%             size(input_image,2), size(input_image,1), ...
%             new_size(2), new_size(1));
%     end
%     output_image = cvlib_mex('resize', input_image, new_size);
    if(verbose)
        fprintf('  rescale_max_size: using slow built in image resize\n');
    end
    output_image = imresize(input_image, new_size, 'bicubic');

else
    if(verbose)
        fprintf('  Image does not need to be rescaled (%dx%d)\n',size(input_image,2), size(input_image,1));
    end
    output_image = input_image;
end

%to preserve the type of the input_image
if(is_double)
    output_image = double(output_image);
end
