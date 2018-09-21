#ifndef JMX_SEQUENCE_H_INCLUDED
#define JMX_SEQUENCE_H_INCLUDED

//==================================================
// @title        sequence.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include <algorithm>

#include "common.h"
#include "memory.h"
#include "setters.h"

// ------------------------------------------------------------------------

namespace jmx {

    template <class T, class M >
    struct Container
    {
        using value_type = typename M::value_type;

        M mem;
        virtual void clear() =0;
        virtual index_t ndims() const =0;

        inline index_t numel() const { return mem.size; }
        inline void free() { mem.free(); clear(); }
        inline value_type& operator[] ( index_t k ) const { return mem.data[k]; }
    };
    
    // ----------  =====  ----------
    
    template <class T, class M = CppMemory<T> >
    struct Vector : public Container<T,M>
    {
        index_t n;

        Vector()
            { clear(); }
        Vector( index_t len )
            { alloc(len); }
        Vector( T *ptr, index_t len )
            { assign(ptr,len); }

        inline index_t ndims() const { return 1; }

        inline void clear()
            { this->mem.clear(); n = 0; }
        inline void assign( T *ptr, index_t len )
            { this->mem.assign(ptr,len); n=len; }
        inline void alloc( index_t len )
            { this->mem.alloc(len); n = len; }
    };

    // ----------  =====  ----------

    template <class T, class M = CppMemory<T> >
    struct Matrix : public Container<T,M>
    {
        using value_type = typename M::value_type;
        index_t nr, nc;

        Matrix()
            { clear(); }
        Matrix( index_t nrows, index_t ncols )
            { alloc(nrows,ncols); }
        Matrix( T *ptr, index_t nrows, index_t ncols )
            { assign(ptr,nrows,ncols); }

        inline index_t ndims() const { return 2; }

        inline void clear()
            { this->mem.clear(); nr = nc = 0; }
        inline void assign( T *ptr, index_t nrows, index_t ncols )
            { this->mem.assign(ptr,nrows*ncols); nr=nrows; nc=ncols; }
        inline void alloc( index_t nrows, index_t ncols )
            { this->mem.alloc(nrows*ncols); nr=nrows; nc=ncols; }

        inline value_type& operator() ( index_t r, index_t c ) const
            { return this->mem[ r + nr*c ]; }
    };

    // ----------  =====  ----------

    template <class T, class M = CppMemory<T> >
    struct Volume : public Container<T,M>
    {
        using value_type = typename M::value_type;
        index_t nr, nc, ns;

        Volume()
            { clear(); }
        Volume( index_t nrows, index_t ncols, index_t nslices )
            { alloc(nrows,ncols,nslices); }
        Volume( T *ptr, index_t nrows, index_t ncols, index_t nslices )
            { assign(ptr,nrows,ncols,nslices); }

        inline index_t ndims() const { return 3; }

        inline void clear()
            { this->mem.clear(); nr = nc = ns = 0; }
        inline void assign( T *ptr, index_t nrows, index_t ncols, index_t nslices )
            { this->mem.assign(ptr,nrows*ncols*nslices); nr=nrows; nc=ncols; ns=nslices; }
        inline void alloc( index_t nrows, index_t ncols, index_t nslices )
            { this->mem.alloc(nrows*ncols*nslices); nr=nrows; nc=ncols; ns=nslices; }

        inline value_type& operator() ( index_t r, index_t c, index_t s ) const
            { return this->mem[ r + nr*c + nr*nc*s ]; }
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
