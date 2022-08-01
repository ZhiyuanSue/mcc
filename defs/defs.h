#ifndef _DEFS_H_
#define _DEFS_H_

/*--platform of mcc running--*/
/*#ifndef _UNIX_
#define _UNIX_
*/
/*
#ifndef _WIN_
#define _WIN_
*/

#ifndef _MAC_
#define _MAC_

#endif

/*--platform of source file--*/
/*
#ifndef _SRC_UNIX_
#define _SRC_UNIX_
*/
#ifndef _SRC_WIN_
#define _SRC_WIN_
/*
#ifndef _SRC_MAC_
#define _SRC_MAC_
*/
#endif


/*--Address bit size--*/
/*it will affect the sizeof(pointer)*/
#ifndef __ADD_SIZE
/*#define __ADD_SIZE 16*/
#define __ADD_SIZE 32
/*#define __ADD_SIZE 64*/
#endif

/*--the char type equals to unsigned or signed--*/
#ifndef _FSIGN
#define _FSIGN 0    /*0 means char equals to signed char,1 means unsigned char*/
#endif

/*--Is _Complex type can be support?--*/
#ifndef _CPLX_SUPPORT
#define _CPLX_SUPPORT 1
#endif

/*--Is _Atomic type can be support?--*/
#ifndef __ATOMIC_SUPPORT
#define __ATOMIC_SUPPORT 1
#endif

/*--test choice--*/
#ifndef _TEST_
#define _TEST_

#ifndef _TEST_LEXER_
//#define _TEST_LEXER_
#endif

#ifndef _TEST_PARSER_
//#define _TEST_PARSER_
#endif

#ifndef _TEST_AST_
#define _TEST_AST_
#endif

#ifndef _TEST_SEMANTICS_
#define _TEST_SEMANTICS_
#endif

#ifndef _TEST_SYMBOL_TABLE_
#define _TEST_SYMBOL_TABLE_
#endif

#endif

/*--memory pool choice, DEFAULT_MEM means use system malloc
    otherwise is memory pool
--*/
#ifndef DEFAULT_MEM
#define DEFAULT_MEM 0   /*0-system malloc,1-memory pool*/
#endif

/*--is a cross complier--*/
#ifndef CROSS_CMPL
//#define CROSS_CMPL
#endif

/*--other definations--*/
#define BUFSIZE 8193    /*4095 character in one logical line*/
#define INF 0x3f3f3f3f
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)>(b))?(b):(a))
#endif

/*#include <stdio.h>*/
#include "../lib/stdio.h"
/*#include <string.h>*/
#include "../lib/string.h"
/*#include <stdlib.h>*/
#include "../lib/stdlib.h"
/*#include <stdbool.h>*/
#include "../lib/stdbool.h"
/*#include <stdint.h>*/
#include "../lib/stdint.h"
/*#include <float.h>*/
#include "../lib/float.h"
/*#include <stddef.h>*/
#include "../lib/stddef.h"


/*define the data model*/
#if __WORDSIZE==32
#define ILP32 0
#define LP32 1
#elif __WORDSIZE==64
#define LP64 1
#define LLP64 0
#endif

#include "align.h"

#endif