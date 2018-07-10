
# Getting Started

Getting started with Matlab-Mex.

 - Sharing memory and variables
 - Mex or not Mex?
 - Using this library
 - Projects with multiple files

---

## Sharing memory and variables

Variables require memory, memory is managed internally by Matlab.

As far as I understand, Mex allows you to access Matlab's memory directly.

You should never, ever modify an input variable; create a new one to store any processing instead.

---

## Mex or not Mex?

Mex functions are callable from Matlab and MUST define a `mexFunction`

```cpp
Show template
```

Non-Mex code (e.g. libraries) can also be compiled, but cannot be called directly from the Matlab console.

---

## Using this library

A typical Mex-file would be:

```cpp
#include "jh-mex.h"
#include "example-lib.h" // other headers from this project ...

// ------------------------------------------------------------------------

void mexFunction( int nargout, mxArray *out[],
                  int nargin, const mxArray *in[] ) // required by Matlab
{
    // deal with inputs
    JHM_ASSERT( nargin >= 1, "This function requires at least one input!" );
    jhm::Vector<double> x = jhm::get_vector(in[0]);

    // call functions from included scripts
    // simple_function(x);

    // deal with outputs
    if (nargout >= 1) {
        out[0] = jhm::make_matrix( 23, 42 );
        jhm::Matrix<double> out0 = get_matrix(out[0]);
    }
}
```

---

## Projects with multiple files

Most projects will have multiple files.

Compile each file independently (most likely not Mex), specify linked objects with the compiler.

Then compile the main Mex file at the end, specifying all linked objects.
