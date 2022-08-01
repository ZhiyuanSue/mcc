#ifndef _PREPRO_H_
#define _PREPRO_H_
#include "../tools/MEM.h"
#include "../tools/mcc_error.h"
typedef struct m_file{
    size_t file_len;
    char* buf;
}M_FILE;
char* prepro(char filename[]);
char* load_file(char filename[]);
inline bool is_end_of_line(char* cp){
#ifdef _SRC_UNIX_
    return *cp=='\n';
#elif defined _SRC_WIN_
    return (*cp=='\n');
#elif defined _SRC_MAC_
    return *cp=='\r';
#endif
}

#endif