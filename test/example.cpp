
#include "jh-mex.h"
using namespace jmx;

// ------------------------------------------------------------------------

void mexFunction( int nargout, mxArray *out[],
                  int nargin, const mxArray *in[] )
{
    // deal with inputs
    JMX_ASSERT( nargin >= 1, "This function requires a vector in input" );
    auto x = get_vector<double>(in[0]);

    cout_redirect(true);
    println("Hello there :)");

    // deal with outputs
    if (nargout >= 1) {
        out[0] = make_matrix( 1, 2 );
        auto out0 = get_matrix_rw<double>(out[0]);
        out0[0] = 1;
        out0[1] = -1;
    }
}