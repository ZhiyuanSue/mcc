#ifndef _ERRON_H_
#define _ERRON_H_

/*in C11 standard ,only three macro is defined
but I use glibc, so I have to set the number accordance with the gcc
If you try to use your own lib, just reset it*/
#define EDOM 33 /* Mathargument out of domain of func */
#define ERANGE 34 /*Math result not representable */

#define EILSEQ 84 /*Illegal byte sequence */

extern int *__errno_location(void);
#ifndef errno
#define errno (*__errno_location())
#endif
#endif