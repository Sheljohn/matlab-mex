#ifndef JMX_ARGS_H_INCLUDED
#define JMX_ARGS_H_INCLUDED

//==================================================
// @title        args.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include "common.h"
#include "getters.h"
#include "makers.h"

#include <string>
#include <vector>
#include <utility>

// ------------------------------------------------------------------------

namespace jmx {

    /**
     * Simple vector wrapper, which returns nullptr outside of its range.
     * This is to avoid segfaults in Mex functions.
     */
    struct _mxInput
    {
        using val_t = const mxArray*;
        val_t *ptr;
        const index_t len;

        _mxInput( val_t ptr[], index_t len )
            : ptr(ptr), len(len) {}
        
        inline val_t operator[] ( index_t k ) const
            { return (k < len) ? ptr[k] : nullptr; }
    };

    struct _mxOutput
    {
        using val_t = mxArray*;
        val_t *ptr;
        const index_t len;

        _mxOutput( val_t ptr[], index_t len )
            : ptr(ptr), len(len) 
        {
            for ( index_t k = 0; k < len; ++k )
                ptr[k] = nullptr;
        }
        
        inline val_t operator[] ( index_t k ) const
        { 
            JMX_ASSERT( k < len, "Failed to access uncollected output." ); 
            return ptr[k];
        }

        inline void assign( index_t k, val_t val )
        {
            JMX_ASSERT( k < len, "Failed to assign uncollected output." ); 
            JMX_ASSERT( !ptr[k], "Cannot overwrite existing output." ); 
            ptr[k] = val;
        }
    };
    
    // ----------  =====  ----------
    
    struct Arguments 
    {
        _mxOutput out;
        _mxInput in;

        Arguments( 
            int nargout, mxArray *out[],
            int nargin, const mxArray *in[]
        ) : in(in,nargin), out(out,nargout) {}

        inline void _checkout( index_t k ) { 
            JMX_ASSERT( k < out.len, "Failed to set uncollected output." ); 
            JMX_ASSERT( !out[k], "Cannot overwrite existing output." );
        }

        // plain getters
        inline bool getbool( index_t k )         { return get_scalar<bool>( in[k] ); }
        inline std::string getstr( index_t k )   { return get_string( in[k] ); }
        inline Cell getcell( index_t k )         { return get_cell( in[k] ); }

        inline Struct getstruct( index_t k, index_t i=0 ) 
            { return get_struct( in[k], i ); }


        // templated getters
        template <class T = real_t>
        inline T getnum( index_t k )             { return get_scalar<T>( in[k] ); }

        template <class T = real_t>
        inline Vector_ro<T> getvec( index_t k )  { return get_vector<T>( in[k] ); }

        template <class T = real_t>
        inline Matrix_ro<T> getmat( index_t k )  { return get_matrix<T>( in[k] ); }

        template <class T = real_t>
        inline Volume_ro<T> getvol( index_t k )  { return get_volume<T>( in[k] ); }


        // getters with defaults
        template <class T = real_t>
        inline T getnum( index_t k, const T& val )
            { return (k < in.len) ? getnum<T>(k) : val; }

        inline bool getbool( index_t k, bool val ) 
            { return (k < in.len) ? getbool(k) : val; }
        inline std::string getstr( index_t k, const std::string& val )
            { return (k < in.len) ? getstr(k) : val; }


        // void setters
        inline void mkbool( index_t k, bool val )
            { out.assign(k, make_logical(val)); }

        inline void mkstr( index_t k, const std::string& val )
            { out.assign(k, make_string(val)); }

        template <class T = real_t>
        inline void mknum( index_t k, const T& val )
            { out.assign(k, make_scalar<T>(val)); }


        // setters with access
        template <class T = real_t>
        inline Vector_mx<T> mkvec( index_t k, index_t len, bool col=false ) { 
            out.assign(k, make_vector( len, col, cpp2mex<T>::classid )); 
            return Vector_mx<T>( static_cast<T*>(mxGetData(out[k])), len );
        }

        template <class T = real_t>
        inline Matrix_mx<T> mkmat( index_t k, index_t nr, index_t nc ) {
            out.assign(k, make_matrix( nr, nc, cpp2mex<T>::classid )); 
            return Matrix_mx<T>( static_cast<T*>(mxGetData(out[k])), nr, nc );
        }

        template <class T = real_t>
        inline Volume_mx<T> mkmat( index_t k, index_t nr, index_t nc, index_t ns ) {
            out.assign(k, make_volume( nr, nc, cpp2mex<T>::classid )); 
            return Volume_mx<T>( static_cast<T*>(mxGetData(out[k])), nr, nc, ns );
        }

        inline Cell mkcell( index_t k, index_t len ) {
            out.assign(k, make_cell( len )); 
            return Cell( out[k] );
        }

        inline Struct mkstruct( index_t k, inilst<const char*> fields ) {
            out.assign(k, make_struct( fields, 1, 1 ));
            return Struct( out[k] );
        }

        inline mxArray* mkstructarr( index_t k, inilst<const char*> fields, index_t nr, index_t nc ) {
            out.assign(k, make_struct( fields, nr, nc ));
            return out[k];
        }
    };

}

#endif
