#ifndef _IRON_H_
#define _IRON_H_

#include <trs/horn.h>

typedef bool(*iron_load_t)(horn_Instance*, const char*);

bool iron_load(horn_Instance* inst, const char* src);

#ifdef IRON_LEX

#include <trs/utils/lex_state.h>

typedef void(*iron_LH_t)(utils_State*, const char*);
typedef bool(*iron_next_t)(utils_State*, const char*);

void iron_LH(utils_State* state, utils_Token* tok);
void iron_next(utils_State* state, utils_Token* tok);

#endif // IRON_LEX

#endif // _IRON_H_
