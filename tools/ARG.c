#include "ARG.h"
bool arg(int argc,char* argv[])
{
    /*deal with the shell argument part*/
    /*for now, I just use a source and a target file name*/
    if(argc!=3){
        printf("mcc:wrong argument number\n");     //it's not a internal error
        exit(1);
        goto no_comply;
    }
    return true;
no_comply:
    return false;
}