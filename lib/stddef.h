#ifndef _UCHAR_H
#define _UCHAR_H        1
#include "stdint.h"
/* Define the 16-bit and 32-bit character types.  */
#define __WCHAR_TYPE__ int
typedef __WCHAR_TYPE__ wchar_t;
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

#ifndef __WINT_TYPE__
# define __WINT_TYPE__ unsigned int
#endif

typedef long int ptrdiff_t;
/* Conversion state information.  */
typedef struct
{
  int count;
  union
  {
    __WINT_TYPE__ wch;
    char wchb[4];
  } value;		/* Value so far.  */
} mbstate_t;
/*I copyed this from gcc,but don't know what it means,emmm,it just works*/

/* Write char16_t representation of multibyte character pointed
   to by S to PC16.  */
extern size_t mbrtoc16 (char16_t *restrict pc16,
                        const char *restrict s, size_t n,
                        mbstate_t *restrict p);

/* Write multibyte representation of char16_t C16 to S.  */
extern size_t c16rtomb (char *restrict s, char16_t c16,
                        mbstate_t *restrict ps);

/* Write char32_t representation of multibyte character pointed
   to by S to PC32.  */
extern size_t mbrtoc32 (char32_t *restrict pc32,
                        const char *restrict s, size_t n,
                        mbstate_t *restrict p);

/* Write multibyte representation of char32_t C32 to S.  */
extern size_t c32rtomb (char *restrict s, char32_t c32,
                        mbstate_t *restrict ps);


#endif