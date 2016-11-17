## Writing C++ Mex files

It is a pain to work with Mex in C++ without a minimal set of wrapper classes. 
Most algorithms you will write in Mex (for acceleration I presume) will only deal with scalars, vectors, matrices and volumes.

I provide a header file `standalone.h` which contains a minimal set of definitions to make it easier to write Mex programs in C++.
I also added a few useful things to allow assertions (throwing exceptions) and keyboard interruptions from the Matlab console.

## Documentation

> TO-DO: Note that the documentation is currently under construction.

Please refer to the [wiki](https://github.com/Sheljohn/matlab-mex/wiki).

## Usage

An example file `test.cpp`

```c++
#include "standalone.h"

void mexFunction(	int nargout, mxArray *out[],
					int nargin, const mxArray *in[] )
{
    mexPrintf("Hello World!\n");
}
```

Compile and run with Matlab:

```
>> mex -lut test.cpp
Building with 'Xcode Clang++'.
MEX completed successfully.
>> test
Hello World!
```

**Note:** you _must_ include the `-lut` into your compiler flags, this is to allow keyboard interruptions using the function `utIsInterruptPending()` in your code.
