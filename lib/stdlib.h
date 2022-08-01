/*
This is part of C11 standard library
and I copy and change some place from GCC's library
*/
#ifndef _STDLIB_H_
#define _STDLIB_H_
#include "errno.h"
#include "stddef.h"
#ifndef NULL
#define NULL 0
#endif
#define MB_CUR_MAX      (__ctype_get_mb_cur_max ())
extern size_t __ctype_get_mb_cur_max (void);

typedef long unsigned int size_t;

extern void *malloc(size_t size);

extern void free(void *ptr);

extern _Noreturn void exit(int status);

extern long int strtol(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern long long int strtoll(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern unsigned long int strtoul(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern unsigned long long int strtoull(
    const char * restrict nptr,
    char ** restrict endptr,
    int base);

extern double strtod(const char * restrict nptr,
    char ** restrict endptr);

extern float strtof(const char * restrict nptr,
    char ** restrict endptr);
    
extern long double strtold(const char * restrict nptr,
    char ** restrict endptr);

extern int mbtowc(wchar_t *pwc, const char *str, size_t n);

extern int mblen (const char *s, size_t n);
#endif