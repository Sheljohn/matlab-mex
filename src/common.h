#ifndef JHM_COMMON_H_INCLUDED
#define JHM_COMMON_H_INCLUDED

//==================================================
// @title        common.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include "mex.h"
#include "mat.h"

#include <cstdio>
#include <cstdlib>

#include <string>
#include <utility>
#include <stdexcept>

// ------------------------------------------------------------------------

// Assertions
static char jhm_msgbuf[JHM_MSGBUF_SIZE];

#define JHM_THROW( msg, args... ) \
	{ sprintf( jhm_msgbuf, "::JH-MEX-Exception:: " msg "\n", ##args ); throw std::runtime_error(jhm_msgbuf); }
#define JHM_WARN( msg, args... ) \
    { mexPrintf( "::JH-MEX-Warning:: " msg "\n", ##args ); }

// exception by default
#define JHM_REJECT( cdt, msg, args... ) { if (cdt) JHM_THROW(msg,##args) }
#define JHM_ASSERT( cdt, msg, args... ) JHM_REJECT(!(cdt),msg,##args)

// warning
#define JHM_WREJECT( cdt, msg, args... ) { if (cdt) JHM_WARN(msg,##args) }
#define JHM_WASSERT( cdt, msg, args... ) JHM_WREJECT(!(cdt),msg,##args)

// return value
#define JHM_WREJECT_R( cdt, val, msg, args... ) { if (cdt) { JHM_WARN(msg,##args) return (val); } }
#define JHM_WASSERT_R( cdt, val, msg, args... ) JHM_WREJECT_R(!(cdt),val,msg,##args)

// return false
#define JHM_WREJECT_RF( cdt, msg, args... ) JHM_WREJECT_R((cdt),false,msg,##args)
#define JHM_WASSERT_RF( cdt, msg, args... ) JHM_WASSERT_R((cdt),false,msg,##args)

namespace jhm {
    
    // Convenient printing functions
    template <typename ...Args>
    inline void print( const std::string& fmt, Args&&... args ) {
        mexPrintf( fmt.c_str(), std::forward<Args>(args)... );
    }

    template <typename ...Args>
    void println( std::string fmt, Args&&... args )
    {
        fmt += "\n"; 
        mexPrintf( fmt.c_str(), std::forward<Args>(args)... );
    }
}

// ------------------------------------------------------------------------

// Detect keyboard interruptions with utIsInterruptPending()
// NOTE: requires compiling with -lut
#ifdef __cplusplus
    extern "C" bool utIsInterruptPending();
#else
    extern bool utIsInterruptPending();
#endif

namespace jhm {

    // More intuitive alias
    inline bool check_interruption() {
        return utIsInterruptPending();
    }

}

// ------------------------------------------------------------------------

// type definitions / utils
#include "types.h"

#endif