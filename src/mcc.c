#include "mcc_main.h"

int main(int argc,char* argv[])
{
    /*main*/
    if(arg(argc,argv))
    {
        mcc_main(argv[1]);
    }
    return 0;
}