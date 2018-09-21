function cmd = jmx( files, options, varargin )
%
% cmd = jmx( files, options, varargin )
%
% Compile a Mex file using the JMX library.
%
% files:    path to Mex file
%           or a cell starting with path to Mex file, followed by object files
%
% options + additional inputs: see jmx_compile for help
%
% See also: jmx_compile
%
% JH

    if nargin < 2, options=struct(); end

    args = varargin;
    here = fileparts(mfilename('fullpath'));
    
    incdir = fullfile(here,'inc');
    objfile = fullfile(incdir,'jmx.o');
    if exist(objfile,'file') ~= 2
        jmx_build();
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
    
    cmd = jmx_compile( files, options, args{:} );

end

function x=wrap_cell(x)
    if ~iscell(x), x={x}; end
end