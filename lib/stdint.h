/*
    For this file, I copyed some code from microsoft and GNU C project,
    Thanks for that.
    But I changed at some places to fit mcc
    And the following is the data model of different system
    -----------------------------------------------------------------------------
    |   Data type   |   ILP32   |   LP32    |   ILP64   |   LP64    |   LLP64   |
    -----------------------------------------------------------------------------
    |   char        |   8       |   8       |   8       |   8       |   8       |
    -----------------------------------------------------------------------------
    |   short       |   16      |   16      |   16      |   16      |   16      |
    -----------------------------------------------------------------------------
    |   int         |   32      |   32      |   64      |   32      |   32      |
    -----------------------------------------------------------------------------
    |   long        |   32      |   32      |   64      |   64      |   32      |
    -----------------------------------------------------------------------------
    |   long long   |   64      |   64      |   64      |   64      |   64      |
    -----------------------------------------------------------------------------
    |   point       |   32      |   32      |   64      |   64      |   64      |
    -----------------------------------------------------------------------------
    And next is the data size of the gcc I used to complie mcc:
    char:8
    short:16
    int:32
    long:64
    long long:64
    point:64
    it means LP64 model——as always defined in Unix 64 system
    and in win64,it always LLP64 
*/
#ifndef _STDINT_H_
#define _STDINT_H_
#include "wordsize.h"
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int_least8_t;
typedef short              int_least16_t;
typedef int                int_least32_t;
typedef long long          int_least64_t;
typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char        int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef long long          int_fast64_t;
typedef unsigned char      uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

#define INT8_MIN         (-127 - 1)
#define INT16_MIN        (-32767 - 1)
#define INT32_MIN        (-2147483647 - 1)
#define INT64_MIN        (-9223372036854775807 - 1)
#define INT8_MAX         127
#define INT16_MAX        32767
#define INT32_MAX        2147483647
#define INT64_MAX        9223372036854775807ll
#define UINT8_MAX        0xffu
#define UINT16_MAX       0xffffu
#define UINT32_MAX       0xfffffffful
#define UINT64_MAX       0xffffffffffffffffull

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX

#if __WORDSIZE == 64
    typedef unsigned long int uintptr_t;
    #define INTPTR_MIN   INT64_MIN
    #define INTPTR_MAX   INT64_MAX
    #define UINTPTR_MAX  UINT64_MAX
#else
    typedef unsigned int uintptr_t;
    #define INTPTR_MIN   INT32_MIN
    #define INTPTR_MAX   INT32_MAX
    #define UINTPTR_MAX  UINT32_MAX
#endif

#define INTMAX_MIN       INT64_MIN
#define INTMAX_MAX       INT64_MAX
#define UINTMAX_MAX      UINT64_MAX

#define PTRDIFF_MIN      INTPTR_MIN
#define PTRDIFF_MAX      INTPTR_MAX

#ifndef SIZE_MAX
    #define SIZE_MAX     UINTPTR_MAX
#endif

#define SIG_ATOMIC_MIN   INT32_MIN
#define SIG_ATOMIC_MAX   INT32_MAX

#define WCHAR_MIN        0x0000
#define WCHAR_MAX        0xffff

#define WINT_MIN         0x0000
#define WINT_MAX         0xffff

#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   (x)
#define INT64_C(x)   (x ## LL)

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  (x ## U)
#define UINT64_C(x)  (x ## ULL)

#define INTMAX_C(x)  INT64_C(x)
#define UINTMAX_C(x) UINT64_C(x)

#endif