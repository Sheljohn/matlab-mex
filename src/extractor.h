#ifndef JMX_EXTRACTOR_H_INCLUDED
#define JMX_EXTRACTOR_H_INCLUDED

//==================================================
// @title        extractor.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include <string>

// ------------------------------------------------------------------------

namespace jmx {
    
    template <class Key>
    struct Extractor
    {
        using key_t = Key;
        using ptr_t = const mxArray*;

        // methods to be defined
        virtual bool _extractor_valid_key( key_t k ) const =0;
        virtual ptr_t _extractor_get( key_t k ) const =0;

        // forward declaration of Struct extraction (see forward.h)
        Struct getstruct( index_t k, index_t i=0 ); 
        
        // ----------  =====  ----------
        
        // plain getters
        inline bool getbool( key_t k )         { return get_scalar<bool>(_extractor_get(k)); }
        inline std::string getstr( key_t k )   { return get_string(_extractor_get(k)); }
        inline Cell getcell( key_t k )         { return get_cell(_extractor_get(k)); }


        // templated getters
        template <class T = real_t>
        inline T getnum( key_t k )             { return get_scalar<T>(_extractor_get(k)); }

        template <class T = real_t>
        inline Vector_ro<T> getvec( key_t k )  { return get_vector<T>(_extractor_get(k)); }

        template <class T = real_t>
        inline Matrix_ro<T> getmat( key_t k )  { return get_matrix<T>(_extractor_get(k)); }

        template <class T = real_t>
        inline Volume_ro<T> getvol( key_t k )  { return get_volume<T>(_extractor_get(k)); }


        // getters with defaults
        template <class T = real_t>
        inline T getnum( key_t k, const T& val )
            { return _extractor_valid_key(k) ? getnum<T>(k) : val; }

        inline bool getbool( key_t k, bool val ) 
            { return _extractor_valid_key(k) ? getbool(k) : val; }
        inline std::string getstr( key_t k, const std::string& val )
            { return _extractor_valid_key(k) ? getstr(k) : val; }
    };

}

#endif
