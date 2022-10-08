#ifndef _MCC_MAIN_H_
#define _MCC_MAIN_H_

#include "../tools/MEM.h"
#include "../tools/mcc_error.h"
#include "../tools/ARG.h"
#include "../tools/EBNF.h"
#include "../frontend/preprocessor.h"
#include "../frontend/lex.h"
#include "../frontend/parser.h"
#include "../frontend/semantics.h"
#include "../frontend/search_mem.h"
#include "../IR/trans.h"
#include "../backend/backend.h"

int mcc_main(char* filename);

#endif