function cmd = compile_jhm( files, options, varargin )

    if nargin < 2, options=struct(); end

    args = varargin;
    here = fileparts(mfilename('fullpath'));
    
    incdir = fullfile(here,'inc');
    objfile = fullfile(incdir,'jh-mex.o');
    if exist(objfile,'file') ~= 2
        compile_lib();
    end
    
    files = wrap_cell(files);
    files{end+1} = objfile;
    
    [~,k] = ismember({'inc','incpath','ipath'},args);
    if any(k)
        k = nonzeros(k);
        ipath = args{ k(1)+1 };
        ipath = wrap_cell(ipath);
        ipath{end+1} = incdir;
        args{ k(1)+1 } = ipath;
    else
        args{end+1} = 'inc';
        args{end+1} = incdir;
    end
    
    cmd = compile( files, options, args{:} );

end

function x=wrap_cell(x)
    if ~iscell(x), x={x}; end
end