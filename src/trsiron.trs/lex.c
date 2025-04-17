#define IRON_LEX
#include <trs/iron.h>

#include <trs/taurus.h>
#include <trs/utils/lex_char.h>

#include <ctype.h>

void iron_next(utils_State* state, utils_Token* token){
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

	// TODO: ==, !=, >=, <=, &&, ||
#define XCASE(CHR, TKN) case CHR: { \
	tok.type = TRS_TT_##TKN;        \
	tok.begin = state->cursor;      \
	tok.end = ++state->cursor;      \
	state->column++;                \
} break;
	switch(*state->cursor){
		XCASE('=', OP_EQ)
		XCASE('+', OP_PLUS)
		XCASE('-', OP_MINUS)
		XCASE('*', OP_MUL)
		XCASE('/', OP_DIV)
		XCASE('%', OP_MOD)
		XCASE('!', OP_LOGICAL_NOT)
		XCASE('>', OP_LOGICAL_GT)
		XCASE('<', OP_LOGICAL_LT)
		XCASE('~', OP_BINARY_NOT)
		XCASE('&', OP_BINARY_AND)
		XCASE('^', OP_BINARY_XOR)
		XCASE('|', OP_BINARY_OR)
		XCASE(';', EOE)
		XCASE(':', COLON)
		XCASE('(', LP)
		XCASE(')', RP)
		XCASE('{', LB)
		XCASE('}', RB)
		XCASE('[', LSB)
		XCASE(']', RSB)
		XCASE(',', COMMA)
		case '\'':
		{
			tok.begin = state->cursor;
			state->cursor++;
			state->column++;
				
			if(s_nextChar(state, '\'') < 0 || *state->cursor != '\'')
				goto ret;

			tok.type = TRS_TT_CHAR;
			tok.end = ++state->cursor;
			state->column++;
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

			tok.type = TRS_TT_STR;
			tok.end = ++state->cursor;
			state->column++;
		} break;
		default:
		{
			if(isdigit(*state->cursor)){
				tok.type = TRS_TT_INT;
				tok.begin = state->cursor;

				while(*state->cursor && isdigit(*state->cursor)){
					state->cursor++;
					state->column++;
				}

				tok.end = state->cursor;
			} else if(*state->cursor == '_' || isalpha(*state->cursor)){
				tok.type = TRS_TT_ID;
				tok.begin = state->cursor;

				while(*state->cursor == '_' || isalnum(*state->cursor)){
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

void iron_LH(utils_State* state, utils_Token* tok){
	if(state->lookahead.type == TRS_TT_UKN)
		iron_next(state, &state->lookahead);
	
	if(tok)
		*tok = state->lookahead;
}
