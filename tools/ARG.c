#include "ARG.h"
void arg(int argc,char* argv[])
{
    //deal with the shell argument part
    if(argc<2){
        printf("mcc:need more argument\n");     //it's not a internal error
        exit(1);
    }
}