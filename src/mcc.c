#include "mcc_main.h"

int main(int argc,char* argv[])
{
    /*main*/
    if(arg(argc,argv))
    {
        mcc_main(argv[1],argv[2]);
    }
    return 0;
}