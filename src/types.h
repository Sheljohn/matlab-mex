#ifndef JHM_TYPES_H_INCLUDED
#define JHM_TYPES_H_INCLUDED

//==================================================
// @title        types.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include <cstdint>
#include <type_traits>
#include <initializer_list>

// ------------------------------------------------------------------------

namespace jhm {

    // map Matlab types
    using index_t = mwIndex;
    using integ_t = mwSignedIndex;
    using real_t  = double;

    // short alias for initializer lists
    template <class T>
    using inilst = std::initializer_list<T>;
    
    // ----------  =====  ----------
    
    template <int C>
    struct mex2cpp
    {
        typedef void type;
    };

    template <> struct mex2cpp<mxLOGICAL_CLASS> { typedef bool type; };
    template <> struct mex2cpp<   mxINT8_CLASS> { typedef int8_t type; };
    template <> struct mex2cpp<  mxUINT8_CLASS> { typedef uint8_t type; };
    template <> struct mex2cpp<  mxINT16_CLASS> { typedef int16_t type; };
    template <> struct mex2cpp< mxUINT16_CLASS> { typedef uint16_t type; };
    template <> struct mex2cpp<  mxINT32_CLASS> { typedef int32_t type; };
    template <> struct mex2cpp< mxUINT32_CLASS> { typedef uint32_t type; };
    template <> struct mex2cpp<  mxINT64_CLASS> { typedef int64_t type; };
    template <> struct mex2cpp< mxUINT64_CLASS> { typedef uint64_t type; };
    template <> struct mex2cpp< mxSINGLE_CLASS> { typedef float type; };
    template <> struct mex2cpp< mxDOUBLE_CLASS> { typedef double type; };
    
    // ----------  =====  ----------
    
    // see: https://stackoverflow.com/a/43205818/472610

    template <int M, class T, class = T>
    struct is_compatible : std::false_type
    {};

    template <int M, class T>
    struct is_compatible<M, T, decltype(static_cast<T>( std::declval< mex2cpp<M>::type >() ))> : std::true_type
    {};

    template <class T>
    inline bool isCompatible( const mxArray *ms ) {
        
        switch (mxGetClassID(ms))
        {
            case mxLOGICAL_CLASS:
                return std::is_same<bool,T>::value;

            case mxINT8_CLASS:
                return std::is_same<int8_t,T>::value;
            case mxINT16_CLASS:
                return std::is_same<int16_t,T>::value;
            case mxINT32_CLASS:
                return std::is_same<int32_t,T>::value;
            case mxINT64_CLASS:
                return std::is_same<int64_t,T>::value;

            case mxUINT8_CLASS:
                return std::is_same<uint8_t,T>::value;
            case mxUINT16_CLASS:
                return std::is_same<uint16_t,T>::value;
            case mxUINT32_CLASS:
                return std::is_same<uint32_t,T>::value;
            case mxUINT64_CLASS:
                return std::is_same<uint64_t,T>::value;

            case mxSINGLE_CLASS:
                return std::is_same<float,T>::value;
            case mxDOUBLE_CLASS:
                return std::is_same<double,T>::value;

            default:
                JHM_WARN( "Unknown class name: %s", mxGetClassName(ms) );
                return false;
        }
    }
    
    // ----------  =====  ----------
    
    inline bool isNumberLike( const mxArray *ms ) {
        return (mxIsNumeric(ms) || mxIsLogical(ms)) && !mxIsComplex(ms);
    }

}

#endif