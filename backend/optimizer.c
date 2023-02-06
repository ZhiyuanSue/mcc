#include "optimizer.h"

bool optimizer(IR_MODULE* irm)
{
    if(!irm)
        goto error;
    
    if(!reg_alloc(irm))
        goto error;
    return true;
error:
    return false;
}
bool reg_alloc(IR_MODULE* irm)
{
    if(!irm)
        goto error;
    return true;
error:
    return false;
}