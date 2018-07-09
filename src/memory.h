#ifndef JHM_MEMORY_H_INCLUDED
#define JHM_MEMORY_H_INCLUDED

//==================================================
// @title        memory.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include "common.h"

// ------------------------------------------------------------------------

namespace jhm {
    
    template <class T>
    struct AbstractMemory
    {
        T *data;
        index_t size;

        void clear()
        {
            data = nullptr;
            size = 0;
        }

        void assign( T* p, index_t n )
        {
            data = p;
            size = n;
        }

        inline T& operator[] ( index_t k ) const { return data[k]; }

        virtual void alloc( index_t n ) =0;
        virtual void free() =0;
    };
    
    // ------------------------------------------------------------------------
    
    template <class T>
    struct ReadOnlyMemory : public AbstractMemory<T>
    {
        void alloc( index_t n )
            { JHM_THROW( "Read-only memory cannot be allocated." ); }

        void free()
            { JHM_THROW( "Read-only memory cannot be freed." ); }
    };

    // ------------------------------------------------------------------------
    
    template <class T>
    struct MatlabMemory : public AbstractMemory<T>
    {
        void alloc( index_t n )
        {
            this->data = static_cast<T*>( mxCalloc( n, sizeof(T) ) ); 
            this->size = n;
        }

        void free()
            { mxFree(this->data); this->clear(); }
    };

    // ------------------------------------------------------------------------
    
    template <class T>
    struct CppMemory : public AbstractMemory<T>
    {
        void alloc( index_t n )
        {
            this->data = new T[n](); 
            this->size = n;
        }

        void free()
            { delete[] this->data; this->clear(); }
    };

}

#endif