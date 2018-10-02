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

#include "common.h"
#include "redirect.h"
#include "makers.h"
#include "setters.h"
#include "sequence.h"
#include "mapping.h"
#include "getters.h"

#endif