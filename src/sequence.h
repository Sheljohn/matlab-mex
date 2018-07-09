#ifndef JHM_SEQUENCE_H_INCLUDED
#define JHM_SEQUENCE_H_INCLUDED

//==================================================
// @title        sequence.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include <algorithm>

#include "common.h"
#include "setters.h"

// ------------------------------------------------------------------------

namespace jhm {

    template <class T>
    struct AbstractSequence
    {
        T *data;

        virtual void clear() =0;

        inline T& operator[] ( index_t k ) const { return data[k]; }

        inline void free() { delete[] data; clear(); }
        inline void mxfree() { mxFree(data); clear(); }

        inline void alloc( index_t n ) { data = new T[n](); }
        
        void mxalloc( index_t n, bool persist=false ) 
        { 
            data = static_cast<T*>( mxCalloc( n, sizeof(T) ) ); 
            if ( persist ) mexMakeMemoryPersistent(data);
        }
    };
    
    // ----------  =====  ----------
    
    template <class T>
    struct Vector : public AbstractSequence<T>
    {
        index_t n;

        Vector()
            { clear(); }
        Vector( T *p, index_t len )
            { set(p,len); }

        inline void clear()
            { this->data = nullptr; n = 0; }
        inline void set( T *ptr, index_t len )
            { this->data = ptr; n = len; }
    };

    // ----------  =====  ----------

    template <class T>
    struct Matrix : public AbstractSequence<T>
    {
        index_t nr, nc;

        Matrix()
            { clear(); }
        Matrix( T *ptr, index_t nrows, index_t ncols )
            { set(ptr,nrows,ncols); }

        inline void clear()
            { this->data = nullptr; nr = nc = 0; }
        inline void set( T *ptr, index_t nrows, index_t ncols )
            { this->data = ptr; nr = nrows; nc = ncols; }

        inline T& operator() ( index_t r, index_t c ) const
            { return this->data[ r + nr*c ]; }
    };

    // ----------  =====  ----------

    template <class T>
    struct Volume : public AbstractSequence<T>
    {
        index_t nr, nc, ns;

        Volume()
            { clear(); }
        Volume( T *ptr, index_t nrows, index_t ncols, index_t nslices )
            { set(ptr,nrows,ncols,nslices); }

        inline void clear()
            { this->data = nullptr; nr = nc = ns = 0; }
        inline void set( T *ptr, index_t nrows, index_t ncols, index_t nslices )
            { this->data = ptr; nr = nrows; nc = ncols; ns = nslices; }

        inline T& operator() ( index_t r, index_t c, index_t s ) const
            { return this->data[ r + nr*c + nr*nc*s ]; }
    };

    // ----------  =====  ----------
    
    class Cell 
    {
    public:

        Cell()
            { clear(); }
        Cell( const mxArray *ms )
            { wrap(ms); }

        inline void clear()
            { mcell = nullptr; }

        void wrap( const mxArray *ms );
        inline const mxArray* mx() const { return mcell; }
            
        inline index_t  numel () const { return mxGetNumberOfElements(mcell); }
        inline bool     empty () const { return numel() == 0; }
        inline bool     valid () const { return mcell && mxIsCell(mcell); }
        inline operator bool  () const { return valid() && !empty(); }

        inline mxArray* operator[] ( index_t index ) const { return get_value(index); }
        inline mxArray* get_value  ( index_t index ) const { return mxGetCell(mcell, index); }

        inline int set_value( index_t index, mxArray *value ) const {
            return set_cell( const_cast<mxArray*>(mcell), index, value );
        }

    private: 

        const mxArray *mcell;
    };

}

#endif