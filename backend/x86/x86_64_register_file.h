#ifndef _X86_64_REG_FILE_H_
#define _X86_64_REG_FILE_H_
#include "../register_file.h"
typedef struct{
    union{
        unsigned long long int rax;
        unsigned int eax;
        unsigned short ax;
        struct{
            unsigned char al;
            unsigned char ah;
        };
    };  /*rax*/
    union{
        unsigned long long int rbx;
        unsigned int ebx;
        unsigned short bx;
        struct{
            unsigned char bl;
            unsigned char bh;
        };
    };  /*rbx*/
    union{
        unsigned long long int rcx;
        unsigned int ecx;
        unsigned short cx;
        struct{
            unsigned char cl;
            unsigned char ch;
        };
    };  /*rcx*/
    union{
        unsigned long long int rdx;
        unsigned int edx;
        unsigned short dx;
        struct{
            unsigned char dl;
            unsigned char dh;
        };
    };  /*rdx*/
}X86_64_REG_FILE;
#endif