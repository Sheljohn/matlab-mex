#ifndef JHM_GETTERS_H_INCLUDED
#define JHM_GETTERS_H_INCLUDED

//==================================================
// @title        getters.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include "common.h"
#include "sequence.h"
#include "mapping.h"

#include <string>

// ------------------------------------------------------------------------

namespace jhm {    

    template <class T>
    T get_scalar( const mxArray *ms )
    {
        JHM_ASSERT( ms, "Null pointer." );
        JHM_ASSERT( isNumberLike(ms) && (mxGetNumberOfElements(ms) == 1),
            "Input should be a numeric scalar." );

        return static_cast<T>(mxGetScalar(ms));
    }

    template <class T>
    T get_scalar( const mxArray *ms, const T& default_val ) {
        return (ms) ? get_scalar<T>(ms) : default_val;
    }

    std::string get_string( const mxArray *ms );

    inline std::string get_string( const mxArray *ms, const std::string& default_val ) {
        return (ms) ? get_string(ms) : default_val;
    }

    template <class T>
    Vector<T> get_vector( const mxArray *ms )
    {
        JHM_ASSERT( ms, "Null pointer." );
        JHM_ASSERT( isNumberLike(ms), "Bad input type." );
        JHM_ASSERT( isCompatible<T>(ms), "Incompatible types." );
        JHM_ASSERT( mxGetNumberOfDimensions(ms)==2, "Not a vector." );

        index_t nr = mxGetM(ms);
        index_t nc = mxGetN(ms);

        if ( nr < nc )
        {
            JHM_ASSERT( (nr==1) && (nc>1), "Not a vector." );
            return Vector<T>( static_cast<T*>(mxGetData(ms)), nc );
        }
        else
        {
            JHM_ASSERT( (nc==1) && (nr>1), "Not a vector." );
            return Vector<T>( static_cast<T*>(mxGetData(ms)), nr );
        }
    }

    template <class T>
    Matrix<T> get_matrix( const mxArray *ms )
    {
        JHM_ASSERT( ms, "Null pointer." );
        JHM_ASSERT( isNumberLike(ms), "Bad input type." );
        JHM_ASSERT( isCompatible<T>(ms), "Incompatible types." );
        JHM_ASSERT( mxGetNumberOfDimensions(ms)==2, "Not a matrix." );
        return Matrix<T>( static_cast<T*>(mxGetData(ms)), mxGetM(ms), mxGetN(ms) );
    }

    template <class T>
    Volume<T> get_volume( const mxArray *ms )
    {
        JHM_ASSERT( ms, "Null pointer." );
        JHM_ASSERT( isNumberLike(ms), "Bad input type." );
        JHM_ASSERT( isCompatible<T>(ms), "Incompatible types." );
        JHM_ASSERT( mxGetNumberOfDimensions(ms)==3, "Not a volume." );
        const index_t *size = mxGetDimensions(ms);
        return Volume<T>( static_cast<T*>(mxGetData(ms)), size[0], size[1], size[2] );
    }

    inline Cell get_cell( const mxArray *ms ) {
        return Cell(ms);
    }

    inline Struct get_struct( const mxArray *ms, index_t index=0 ) {
        return Struct(ms, index);
    }

}

#endif