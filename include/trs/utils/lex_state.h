#ifndef _UTILS_LEX_STATE_H_
#define _UTILS_LEX_STATE_H_

#include <stdlib.h>

#include "lex_token.h"

typedef struct utils_State utils_State;

struct utils_State{
	utils_Token lookahead;
	const char* src;
	const char* cursor;
	size_t row;
	size_t column;
};

static inline void utils_resetState(utils_State* state, const char* src){
	state->lookahead.type = 0;
	state->lookahead.begin = NULL;
	state->lookahead.end = NULL;
	state->src = src;
	state->cursor = src;
	state->row = 0;
	state->column = 0;
}

#endif //_UTILS_LEX_STATE_H_
