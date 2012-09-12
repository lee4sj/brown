function fnames = real_dirs(path)
%James Hays
%returns the names of the real (not '.' or '..') subdirectories in a path.
%works on windows or linux

%fnames is a cell array

files = dir(path);
real_dir_indicator = zeros(size(files,1),1);

for i = 1:size(files)
    if (files(i).isdir)
        if(strcmp(files(i).name, '.') || strcmp(files(i).name, '..'))
            real_dir_indicator(i) = 0;
        else
            real_dir_indicator(i) = 1;
        end
    end
end

counter = 0;
fnames = cell(sum(real_dir_indicator), 1);
for i = 1:size(files)
    if( real_dir_indicator(i) )
        counter = counter + 1;
        fnames{counter} = files(i).name;
    end
end