function compile_lib(varargin)

    opt.mex = false;
    opt.cpp11 = true;
    opt.optimise = true;
    
    compile( 'src/main.cpp', opt, 'lib', 'ut', varargin{:} );
    movefile( 'src/main.o', 'inc/jh-mex.o' );
    
end