
#include "jh-mex.h"

// include other header files defining functions
#include "example-lib.h"

// ------------------------------------------------------------------------

void mexFunction( int nargout, mxArray *out[],
                  int nargin, const mxArray *in[] )
{
    // deal with inputs
    JHM_ASSERT( nargin >= 1, "This function requires at least one input!" );
    jhm::Vector<double> x = jhm::get_vector(in[0]);

    // call functions from included scripts
    // simple_function(x);

    // create outputs
}