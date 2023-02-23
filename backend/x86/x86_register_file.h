#ifndef _X86_REG_FILE_H_
#define _X86_REG_FILE_H_
#include "../register_file.h"
typedef struct{
    union{
        unsigned int eax;
        unsigned short ax;
        struct{
            unsigned char al;
            unsigned char ah;
        };
    };  /*eax*/
    union{
        unsigned int ebx;
        unsigned short bx;
        struct{
            unsigned char bl;
            unsigned char bh;
        };
    };  /*ebx*/
    union{
        unsigned int ecx;
        unsigned short cx;
        struct{
            unsigned char cl;
            unsigned char ch;
        };
    };  /*ecx*/
    union{
        unsigned int edx;
        unsigned short dx;
        struct{
            unsigned char dl;
            unsigned char dh;
        };
    };  /*edx*/
}X86_32_REG_FILE;

#endif