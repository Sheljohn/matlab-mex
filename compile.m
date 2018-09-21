function cmd = compile( files, options, varargin )
%
% cmd = compile( files, options, ... )
%
% Compile C++ files using Mex.
% 
% ** files
%  
%   Either a string or a cell of strings.
%   If a file depends on other files, then needs to be a cellstr with object files last.
%
%
% ** options
%
%   mex         true   -c         Whether the target source file is a Mex-file.
%                                 I.e. it should have a mexFunction() instead of a main().
%
%   mock        false  -n         Dry-run mode (will not actually compile target files if true).
%   cpp11       true              Set appropriate compiler flags for the C++11 standard.
%
%   index32     false             Newer versions of Matlab use 64-bits indices (-largeArrayDims).
%                                 Set to true to use 32-bits legacy indexing (-compatibleArrayDims).
%
%   outdir      pwd    -outdir    The folder in which to put the compiled object.
%   outfile     ''     -ouput     The name of the compiled file.
%   mexopts     ''     -f         Path to an .xml file with custom Mex options.
%
% See the documentation of mex for the following options:
%
%   optimise    false  -O
%   debug       false  -g
%   verbose     false  -v
%   silent      false  -silent
%
%
% ** settings
% 
%   flag                CXXFLAGS
%   def                 -D
%   undef               -U
%   lib                 -l
%   lpath               -L
%   ipath               -I
%
% JH

    % process inputs
    if nargin < 2 || isempty(options), options = struct(); end

    files = wrap_cell(files);
    filetest = @(f) ismember( exist(f,'file'), [2,7] );
    
    assert( iscellstr(files), 'Files ($1) should be a string or cell-string.' );
    assert( all(cellfun( filetest, files )), 'One or several files not found.' );
    
    T = parse_options(options, fileparts(files{1})); % default output dir with target file
    S = parse_settings(varargin{:});
    
    
    % apply side-effects
    if T.cpp11
        if isfield(S,'flag')
            S.flag{end+1} = '-std=c++11';
        else
            S.flag = {'-std=c++11'};
        end
    end
    [F,D,U,l,L,I] = process_settings(S);
    
    % build command
    cmd = {};
    
    if T.index32
        cmd{end+1} = '-compatibleArrayDims';
    else
        cmd{end+1} = '-largeArrayDims';
    end
    
    if ~T.mex,      cmd{end+1} = '-c'; end
    if T.optimise,  cmd{end+1} = '-O'; end
    if T.debug,     cmd{end+1} = '-g'; end
    if T.mock,      cmd{end+1} = '-n'; end
    if T.verbose,   cmd{end+1} = '-v'; end
    if T.silent,    cmd{end+1} = '-silent'; end
    
    if ~isempty(T.mexopts)
        assert( filetest(T.mexopts), 'Mex options file not found.' );
        cmd{end+1} = '-f';
        cmd{end+1} = T.mexopts;
    end
    if ~isempty(T.outdir)
        assert( filetest(T.outdir), 'Output folder not found.' );
        cmd{end+1} = '-outdir';
        cmd{end+1} = T.outdir;
    end
    if ~isempty(T.outfile)
        cmd{end+1} = '-output';
        cmd{end+1} = T.outfile;
    end
    
    cmd{end+1} = F;
    cmd{end+1} = D;
    cmd{end+1} = U;
    cmd{end+1} = l;
    cmd{end+1} = L;
    cmd{end+1} = I;
    
    cmd = horzcat( cmd, files );
    disp(['mex ' strjoin(cmd)]);
    mex(cmd{:});
    
end

function out = parse_options(in,filedir)

    % set defaults
    out.outdir  = filedir;
    out.outfile = '';
    out.mexopts = '';
    
    out.mex = true;
    out.mock = false;
    
    out.cpp11 = true;
    out.index32 = false;
    
    out.optimise = false;
    out.verbose = false;
    out.silent = false;
    out.debug = false;
    
    % overwrite defaults
    f = fieldnames(in);
    n = numel(f);
    for i = 1:n
        out.(f{i}) = in.(f{i});
    end
    
    % deal with spelling variants
    if isfield(out,'optimize')
        assert( ~isfield(in,'optimise'), 'Conflicting spelling of "optimise".' );
        out.optimise = out.optimize;
    end

end

function c = wrap_cell(varargin)
    if nargin == 1 && iscell(varargin{1})
        c = varargin{1};
    else
        c = varargin;
    end
end

function s = parse_settings(varargin)

    assert( mod(nargin,2) == 0, 'Inputs should be Name/Value pairs.' )
    
    n = nargin/2;
    s = struct();
    
    for i = 1:n
        name  = varargin{2*i-1};
        value = wrap_cell(varargin{2*i});
        assert( iscellstr(value) && ~isempty(value), 'Expected a non-empty cell of strings.' );
        
        switch lower(name)
            case {'flag'}
                s.flag = value;
            case {'lnk','link'}
                s.link = value;
            case {'def','define'}
                s.def = value;
            case {'undef','undefine'}
                s.undef = value;
            case {'lib','library'}
                s.lib = value;
            case {'lpath','ldpath'}
                s.lpath = value;
            case {'ipath','inc','incpath'}
                s.ipath = value;
            otherwise
                error('Unknown setting: %s', name);
        end
    end
    
end

function [flag, def, undef, lib, lpath, ipath] = process_settings(s)
    
    prefix = @(c,p) cellfun( @(s) [p s], c, 'UniformOutput', false );

    if isfield(s,'flag')
        flag = ['CXXFLAGS="$CXXFLAGS ' strjoin(s.flag) '"'];
    else
        flag = '';
    end
    
    if isfield(s,'def')
        def = strjoin(prefix( s.def, '-D' ));
    else
        def = '';
    end
    
    if isfield(s,'undef')
        undef = strjoin(prefix( s.undef, '-U' ));
    else
        undef = '';
    end
    
    if isfield(s,'lib')
        lib = strjoin(prefix( s.lib, '-l' ));
    else
        lib = '';
    end
    
    if isfield(s,'lpath')
        lpath = strjoin(prefix( s.lpath, '-L' ));
    else
        lpath = '';
    end
    
    if isfield(s,'ipath')
        ipath = strjoin(prefix( s.ipath, '-I' ));
    else
        ipath = '';
    end

end