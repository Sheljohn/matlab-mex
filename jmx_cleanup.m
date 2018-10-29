function files = jmx_cleanup( folder, type )
%
% files = jmx_cleanup( folder, type=all )
%
% Remove compiled files from specified folder.
% Note that this method is not recursive; subfolders are not searched.
%
% Type can be:
%
%   mex     Remove all files with extension mexext()
%   obj     Remove all files with extension .o
%   all     Combine options mex and obj
%
% JH

    if nargin < 2, type='all'; end

    switch lower(type)
        
        case 'all'
            jmx_cleanup( folder, 'mex' );
            jmx_cleanup( folder, 'obj' );
            return;
            
        case 'mex'
            files = dk.fs.list_ext( folder, mexext );
            print = @(x) dk.info('[jmx_cleanup] Removing mex-file "%s"',x);
            
        case 'obj'
            files = dk.fs.list_ext( folder, 'o' );
            print = @(x) dk.info('[jmx_cleanup] Removing object-file "%s"',x);
        
        otherwise
            error( 'Unknown source type "%s".', type );
            
    end
    
    for i = 1:length(files)
        f = fullfile( folder, files{i} ); 
        print(f); delete(f);
    end

end