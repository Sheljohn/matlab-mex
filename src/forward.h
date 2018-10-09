#ifndef JMX_FOO_H_INCLUDED
#define JMX_FOO_H_INCLUDED

namespace jmx {
    
    inline Struct get_struct( const mxArray *ms, index_t index ) {
        return Struct(ms, index);
    }

    template <class K>
    inline Struct Extractor<K>::getstruct( index_t k, index_t i ) 
        { return get_struct( _extractor_get(k), i ); }

    template <class K>
    inline Struct Creator<K>::mkstruct( key_t k, inilst<const char*> fields ) {
        return Struct( _creator_assign(k, make_struct( fields, 1, 1 )) );
    }

}

#endif