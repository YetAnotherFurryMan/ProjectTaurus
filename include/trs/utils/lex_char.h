#ifndef _UTILS_LEX_CHAR_H_
#define _UTILS_LEX_CHAR_H_

#include "lex_state.h"

#include <ctype.h>

static int s_nextChar(utils_State* state, char q){
	size_t col = state->column;

	if(*state->cursor == '\\'){
		state->cursor++;
		state->column++;
		
		if(!*state->cursor)
			return -1;

		
		switch(*state->cursor){
			case 't':
			case 'n':
			case 'r':
			case '\'':
			case '\"':
			{
				state->cursor++;
				state->column++;
			} break;
			case 'x':
			{
				state->cursor++;
				state->column++;

				if(!*state->cursor ||
				   !(isdigit(*state->cursor) ||
					(*state->cursor >= 'A' && *state->cursor <= 'F') ||
					(*state->cursor >= 'a' && *state->cursor <= 'f'))) return -1;

				state->cursor++;
				state->column++;

				if(!*state->cursor ||
				   !(isdigit(*state->cursor) ||
					(*state->cursor >= 'A' && *state->cursor <= 'F') ||
					(*state->cursor >= 'a' && *state->cursor <= 'f'))) return -1;

				state->cursor++;
				state->column++;
			} break;
			default:
			{
				if(isdigit(*state->cursor)){
					state->cursor++;
					state->column++;

					if(isdigit(*state->cursor)){
						state->cursor++;
						state->column++;
					}

					if(isdigit(*state->cursor)){
						state->cursor++;
						state->column++;
					}
				} else{
					return -1;
				}
			}
		}
	} else if(*state->cursor != q){
		state->cursor++;
		state->column++;
	} else{
		return -2;
	}

	return state->column - col;
}

#endif //_UTILS_LEX_CHAR_H_
