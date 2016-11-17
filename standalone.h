#ifndef STANDALONE_H_INCLUDED
#define STANDALONE_H_INCLUDED

//==================================================
// @title        standalone.h
// @author       Jonathan Hadida
// @contact      Jhadida [at] fmrib.ox.ac.uk
//==================================================

#include "mex.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// ------------------------------------------------------------------------

// Assertions
static char sa_msg_[2047];

#define SA_THROW( msg, args... ) \
	{ sprintf( sa_msg_, "::DRAYN:: " msg "\n", ##args ); throw std::runtime_error(sa_msg_); }

#define SA_REJECT( cdt, msg, args... ) { if (cdt) SA_THROW(msg,##args) }
#define SA_ASSERT( cdt, msg, args... ) SA_REJECT(!(cdt),msg,##args)

// Detect keyboard interruptions with utIsInterruptPending()
#ifdef __cplusplus
    extern "C" bool utIsInterruptPending();
#else
    extern bool utIsInterruptPending();
#endif

// ------------------------------------------------------------------------

// Typedefs
using index_t = mwIndex;
using integ_t = mwSignedIndex;
using real_t  = double;

/**
 * Basic containers.
 */
template <class T, class I = index_t>
struct Vector
{
    T *data;
    I len;

    Vector()
        { clear(); }
    Vector( T *data_, I len_ )
        { set(data_,len_); }

    inline void clear()
        { data = NULL; len = 0; }
	inline void free()
		{ delete[] data; clear(); }

    inline void set( T *data_, I len_ )
        { data = data_; len = len_; }

    inline T& operator[] ( I k ) const
        { return data[k]; }
};

template <class T, class I = index_t>
struct Matrix
{
    T *data;
    I nrows, ncols;

    Matrix()
        { clear(); }
    Matrix( T *data_, I nrows_, I ncols_ )
        { set(data_,nrows_,ncols_); }

    inline void clear()
        { data = NULL; nrows = ncols = 0; }
	inline void free()
		{ delete[] data; clear(); }

    inline void set( T *data_, I nrows_, I ncols_ )
        { data = data_; nrows = nrows_; ncols = ncols_; }

    inline T& operator() ( I r, I c ) const
	    { return data[ r + nrows*c ]; }
};

template <class T, class I = index_t>
struct Volume
{
    T *data;
    I nr, nc, ns;

    Volume()
        { clear(); }
    Volume( T *data_, I nr_, I nc_, I ns_ )
        { set(data_,nr_,nc_,ns_); }

    inline void clear()
        { data = NULL; nr = nc = ns = 0; }
	inline void free()
		{ delete[] data; clear(); }

    inline void set( T *data_, I nr_, I nc_, I ns_ )
        { data = data_; nr = nr_; nc = nc_; ns = ns_; }

    inline T& operator() ( I r, I c, I s ) const
	    { return data[ r + nr*c + nr*nc*s ]; }
};

/**
 * Basic extraction methods.
 */
template <class T>
Vector<T> get_vector( const mxArray *ms )
{
	SA_ASSERT( mxIsNumeric(ms) && !mxIsComplex(ms), "Bad input type." );
	SA_ASSERT( mxGetNumberOfDimensions(ms)==2, "Not a vector." );

	index_t nr = mxGetM(ms);
	index_t nc = mxGetN(ms);

	if ( nr < nc )
	{
		SA_ASSERT( (nr==1) && (nc>1), "Not a vector." );
		return Vector<T>( static_cast<T*>(mxGetData(ms)), nc );
	}
	else
	{
		SA_ASSERT( (nc==1) && (nr>1), "Not a vector." );
		return Vector<T>( static_cast<T*>(mxGetData(ms)), nr );
	}
}

template <class T>
Matrix<T> get_matrix( const mxArray *ms )
{
	SA_ASSERT( mxIsNumeric(ms) && !mxIsComplex(ms), "Bad input type." );
	SA_ASSERT( mxGetNumberOfDimensions(ms)==2, "Not a matrix." );
	return Matrix<T>( static_cast<T*>(mxGetData(ms)), mxGetM(ms), mxGetN(ms) );
}

template <class T>
Volume<T> get_volume( const mxArray *ms )
{
	SA_ASSERT( mxIsNumeric(ms) && !mxIsComplex(ms), "Bad input type." );
	SA_ASSERT( mxGetNumberOfDimensions(ms)==3, "Not a volume." );
    const index_t *size = mxGetDimensions(ms);
	return Volume<T>( static_cast<T*>(mxGetData(ms)), size[0], size[1], size[2] );
}

template <class T>
T get_scalar( const mxArray *ms )
{
	SA_ASSERT( ms, "Null pointer in input." );
	SA_ASSERT( (mxIsNumeric(ms) || mxIsLogical(ms)) && !mxIsComplex(ms) && (mxGetNumberOfElements(ms) == 1),
		"Input should be a numeric scalar." );

	return static_cast<T>(mxGetScalar(ms));
}

template <class T>
T get_scalar( const mxArray *ms, const T& default_val )
{
	return (ms) ? get_scalar<T>(ms) : default_val;
}

/**
 * Basic creation methods.
 */
template <class T>
inline mxArray* make_scalar( const T& val )
{
    return mxCreateDoubleScalar(static_cast<double>(val));
}

inline mxArray* make_matrix( index_t nr, index_t nc, mxClassID classid=mxDOUBLE_CLASS )
{
    return mxCreateNumericMatrix( nr, nc, classid, mxREAL );
}

inline mxArray* make_vector( index_t len, bool column=false, mxClassID classid=mxDOUBLE_CLASS )
{
    if (column)
        return make_matrix( len, 1, classid );
    else
        return make_matrix( 1, len, classid );
}

inline mxArray* make_volume( index_t nr, index_t nc, index_t ns, mxClassID classid=mxDOUBLE_CLASS )
{
    index_t size[3] = {nr,nc,ns};
    return mxCreateNumericArray( 3, size, classid, mxREAL );
}

#endif
