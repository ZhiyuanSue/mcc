#include "optimizer.h"

bool optimizer(IR_MODULE* irm)
{
    if(!irm)
        goto error;
    return true;
error:
    return false;
}