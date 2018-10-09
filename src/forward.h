#ifndef JMX_FORWARD_H_INCLUDED
#define JMX_FORWARD_H_INCLUDED

namespace jmx {

    /**
     * The following definitions were forwarded from files:
     *      getters.h
     *      extractor.h
     *      creator.h
     * 
     * to allow mappable types (see mapping.h/Abstract) to implement
     * the Extractor / Creator interfaces. Since Struct is a mappable
     * type itself, the following functions/methods could not be implemented
     * before its full definition.
     * 
     * This is a bit of a mind-bending inheritance dependency; 
     * it's not pretty, but it works.
     */
    
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