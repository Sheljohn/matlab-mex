
#include <iostream>
#include "jh-mex.h"

void mexFunction( int nargout, mxArray *out[],
                  int nargin, const mxArray *in[] )
{
    jhm::cout_redirect();
    jhm::MAT mfile("data.mat");
    
    jhm::println("Opened file with the following fields:");
    for ( unsigned i=0; i < mfile.nfields(); ++i )
        std::cout << mfile.get_name(i) << std::endl;
}
