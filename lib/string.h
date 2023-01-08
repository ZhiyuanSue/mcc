/*
This is part of C11 standard library
and I copy and change some place from GCC's library
*/
#ifndef _STRING_H_
#define _STRING_H_
#ifndef NULL
#define NULL 0
#endif
typedef long unsigned int size_t;

extern void *memset(void *s, int c, size_t n);

extern int strcmp(const char *s1, const char *s2);

extern void *memcpy(void * restrict s1,
    const void * restrict s2, size_t n);
extern char *strcpy(char *s1,const char *s2);
extern size_t strlen(const char *s);
#endif