#include <trs/horn.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

horn_Token g_horn_lookahead = {0};

static int horn_nextChar(const char** str, char q){
	const char* s = *str;
	int ret = -1;

	if(*s == '\\'){
		if(!*(++s)) goto ret;
		switch(*s){
			case 't':
			{
				ret = 9;
			} break;
			case 'n':
			{
				ret = 10;
			} break;
			case 'r':
			{
				ret = 13;
			} break;
			case '\'':
			case '\"':
			{
				ret = *s;
			} break;
			case 'x':
			{
				if(!*(++s)) goto ret;
				if(isdigit(*(++s))) ret = *s - '0';
				else if(*s >= 'A' && *s <= 'F') ret = *s - 'A' + 10;
				else if(*s >= 'a' && *s <= 'f') ret = *s - 'a' + 10;
				else goto ret;

				if(!*(++s)){
					ret = -1;
					goto ret;
				}

				ret *= 16;
				if(isdigit(*(++s))) ret += *s - '0';
				else if(*s >= 'A' && *s <= 'F') ret += *s - 'A' + 10;
				else if(*s >= 'a' && *s <= 'f') ret += *s - 'a' + 10;
				else{
					ret = -1;
					goto ret;
				}
			} break;
			default:
			{
				if(isdigit(*s)){
					ret = *s - '0';

					if(!isdigit(*(++s))) goto ret;
					ret *= 8;
					ret += *s - '0';

					if(!isdigit(*(++s))) goto ret;
					ret *= 8;
					ret += *s - '0';
				}
			}
		}
	} else if(*s != q){
		ret = *(s++);
	}

ret:
	if(ret)	*str = s;
	return ret;
}

void horn_next(horn_Token* tok, const char* src){
	static const char* s = NULL;
	if(src){
		s = src;
		g_horn_lookahead = (horn_Token){0};
	}

	if(g_horn_lookahead.type != HORN_TT_UKN){
		*tok = g_horn_lookahead;
		g_horn_lookahead = (horn_Token){0};
		return;
	}

	if(!s || !*s){
		*tok = (horn_Token){HORN_TT_EOF, NULL};
		return;
	}

	while(*s && isspace(*s))
		s++;

	if(!*s){
		s = NULL;
		*tok = (horn_Token){HORN_TT_EOF, NULL};
		return;
	}

	horn_TokenType tt = HORN_TT_UKN;
	char* text = NULL;

	// TODO: ==, !=, >=, <=, &&, ||
#define XCASE(CHR, TKN) case CHR: tt = HORN_TT_##TKN; break;
	switch(*s){
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
		default:
		{
			if(*s == '\''){
				s++;
				int v = horn_nextChar(&s, '\'');
				if(v < 0 || *s != '\'') goto ret;
				text = malloc(2);
				text[0] = (char)v;
				text[1] = 0;
				tt = HORN_TT_CHAR;
			} else if(isdigit(*s)){
				tt = HORN_TT_INT;

				size_t i = 1;
				while(s[i] && isdigit(s[i]))
					i++;

				text = malloc((i + 1) * sizeof(char));
				if(!text)
					goto ret;

				memcpy(text, s, i);
				text[i] = 0;
				
				s += i - 1;
			} else if(*s == '_' || isalpha(*s)){
				tt = HORN_TT_ID;

				size_t i = 1;
				while(s[i] == '_' || isalnum(s[i]))
					i++;
				
				text = malloc((i + 1) * sizeof(char));
				if(!text)
					goto ret;
				
				memcpy(text, s, i);
				text[i] = 0;
				
				s += i - 1;
			}
		} break;
	}
#undef XCASE

	if(*s)
		s++;
	else 
		s = NULL;

ret:
	tok->type = tt;
	tok->text = text;
}

void horn_LH(horn_Token* tok, const char* src){
	if(g_horn_lookahead.type == HORN_TT_UKN)
		horn_next(&g_horn_lookahead, src);
	*tok = g_horn_lookahead;
}
