#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_
typedef unsigned int NMSP;
#define NMSP_UNKNOWN 0
#define NMSP_DEFAULT 1
#define NMSP_SU_TAG 2
NMSP GET_NEW_NAMESPACE(void);
void RESET_NAMESPACE(void);
#endif