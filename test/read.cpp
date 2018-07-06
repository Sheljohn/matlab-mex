
#include <iostream>
#include "../standalone.h"
#include "../redirect.h"

void mexFunction( int nargout, mxArray *out[],
                  int nargin, const mxArray *in[] )
{
    sa::cout_redirect();
    sa::MAT mfile("data.mat");
    
    sa::println("Opened file with the following fields:");
    for ( unsigned i=0; i < mfile.nfields(); ++i )
        std::cout << mfile.field_name(i) << std::endl;
}
