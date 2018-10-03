function p = jmx_incpath(varargin)
    p = fileparts(mfilename('fullpath'));
    p = fullfile(p, 'inc', varargin{:});
end