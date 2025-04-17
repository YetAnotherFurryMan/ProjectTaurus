#include <trs/horn.h>

#include <ctype.h>

#include <trs/utils/lex_char.h>

bool s_isValidId(char c){
	return (c != '\'' && c != '(' && c != ')' && c != '.' && c >= '!' && c <= '/') || (c >= ':' && c <= '@') || c == '_';
}

void horn_next(utils_State* state, utils_Token* token){
	utils_Token tok = (utils_Token){0};

	if(!state)
		goto ret;

	if(state->lookahead.type != HORN_TT_UKN){
		tok = state->lookahead;
		state->lookahead = (utils_Token){0};
		goto ret;
	}

	if(!state->cursor || !*state->cursor){
		tok.type = HORN_TT_EOF;
		goto ret;
	}

	while(*state->cursor && isspace(*state->cursor)){
		if(*state->cursor == '\n'){
			state->row = 0;
			state->column = 0;
		}
		state->cursor++;
		state->column++;
	}

	if(!*state->cursor){
		tok.type = HORN_TT_EOF;
		goto ret;
	}

#define XCASE(CHR, TKN) case CHR: { \
	tok.type = HORN_TT_##TKN;       \
	tok.begin = state->cursor;      \
	tok.end = ++state->cursor;      \
	state->column++;                \
} break;
	switch(*state->cursor){
		XCASE('(', LP)
		XCASE(')', RP)
		XCASE('{', LB)
		XCASE('}', RB)
		XCASE('[', LSB)
		XCASE(']', RSB)
		case '\'':
		{
			tok.begin = state->cursor;
			state->cursor++;
			state->column++;
				
			if(s_isValidId(*state->cursor) || isalpha(*state->cursor)){
				tok.type = HORN_TT_OBJ;

				while(s_isValidId(*state->cursor) || isalnum(*state->cursor)){
					state->cursor++;
					state->column++;
				}

				tok.end = state->cursor;
			} else {
				goto ret;
			}
		} break;
		case '\"':
		{
			tok.begin = state->cursor;
			state->cursor++;
			state->column++;

			int v = 0;
			while(v >= 0){
				v = s_nextChar(state, '\"');
			}

			if(v != -2)
				goto ret;

			tok.type = HORN_TT_STR;
			tok.end = ++state->cursor;
			state->column++;
		} break;
		default:
		{
			if(isdigit(*state->cursor)){
				tok.type = HORN_TT_INT;
				tok.begin = state->cursor;

				while(*state->cursor && isdigit(*state->cursor)){
					state->cursor++;
					state->column++;
				}

				tok.end = state->cursor;
			} else if(s_isValidId(*state->cursor) || isalpha(*state->cursor)){
				tok.type = HORN_TT_ID;
				tok.begin = state->cursor;

				while(s_isValidId(*state->cursor) || isalnum(*state->cursor)){
					state->cursor++;
					state->column++;
				}

				tok.end = state->cursor;
			} else {
				tok.begin = state->cursor;
				tok.end = ++state->cursor;
				state->cursor++;
			}
		} break;
	}
#undef XCASE

ret:
	if(token)
		*token = tok;
}

void horn_LH(utils_State* state, utils_Token* tok){
	if(state->lookahead.type == HORN_TT_UKN)
		horn_next(state, &state->lookahead);
	
	if(tok)
		*tok = state->lookahead;
}
