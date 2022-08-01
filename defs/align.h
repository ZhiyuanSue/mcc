#ifndef _ALIGN_H_
#define _ALIGN_H_


#include "../lib/wordsize.h"
/*alignment,of course the size here means the bytes not bits*/
#ifndef _DEFALUT_ALIGN_SIZE_

#if __WORDSIZE==32
#define _DEFALUT_ALIGN_SIZE_ 4
#elif __WORDSIZE==64
#define _DEFALUT_ALIGN_SIZE_ 8
#endif
/*just like #pragma pack(x)*/
#endif
#ifndef _ALIGN_SIZE_
#define _ALIGN_SIZE_(x) ((x!=1&&x!=2&&x!=4&&x!=8&&x!=16)?_DEFALUT_ALIGN_SIZE_:x)
#endif

#define _MCC_ALIGN_PACK_ 8

#define MCC_ALIGN(addr,align)   \
         ( (uintptr_t)( ( (uintptr_t)addr + ((uintptr_t)(align-1)) ) &~( (uintptr_t)(align-1) ) ) )

#define OFFSET(str_name,member)   \
        ( (uintptr_t) ( &( ((str_name *)0)->member  ) ) )
inline bool  is_legal_align(unsigned long int align_size)
{
        if (align_size==0)
                return false;
        while(align_size%2==0)
        {
                align_size/=2;
        }
        if(align_size!=1)
                return false;
        else
                return true;
}

#ifndef _BIT_FIELD_COMPRESS_
#define _BIT_FIELD_COMPRESS_ 1 /*default is use compress*/
#endif
#endif