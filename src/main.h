#ifndef JMX_H_INCLUDED
#define JMX_H_INCLUDED

//==================================================
// @title        J.H. Mex Library
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#ifndef JMX_MSGBUF_SIZE
#define JMX_MSGBUF_SIZE 2047
#endif

#ifndef JMX_64BIT
    #ifndef JMX_32BIT
        #define JMX_64BIT
    #endif
#endif

// common stuff
#include "common.h"

#include "redirect.h"
#include "makers.h"
#include "setters.h"

// sequence containers
#include "sequence.h"

// forward declaration of Struct
// Allows Abstract mapping to implement creator/extractor interfaces.
namespace jmx { struct Struct; }
#include "getters.h"
#include "creator.h"
#include "extractor.h"

// definition of Struct, and forward definitions
#include "mapping.h"
#include "forward.h"
#include "args.h"

#endif