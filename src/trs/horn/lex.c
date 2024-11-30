#include <trs/horn.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

horn_Token g_horn_lookahead = {0};

void horn_next(horn_Token* tok, const char* src){
	static const char* s = NULL;
	if(src){
		s = src;
		g_horn_lookahead = (horn_Token){0};
	}

	if(g_horn_lookahead.type != HORN_TT_UKN){
		*tok = g_horn_lookahead;
		g_horn_lookahead = (horn_Token){0};
		fprintf(stderr, "INFO: Lookahead(%s): %s\n", horn_TokenTypeToString(tok->type), tok->text);
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

	switch(*s){
		case '=':
			tt = HORN_TT_OP_EQ;
			break;
		case '+':
			tt = HORN_TT_OP_PLUS;
			break;
		case '*':
			tt = HORN_TT_OP_MUL;
			break;
		case ';':
			tt = HORN_TT_EOE;
			break;
		case '(':
			tt = HORN_TT_LP;
			break;
		case ')':
			tt = HORN_TT_RP;
			break;
		default:
		{
			if(isdigit(*s)){
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

	if(*s)
		s++;
	else 
		s = NULL;

ret:
	fprintf(stderr, "INFO: Token(%s): %s\n", horn_TokenTypeToString(tt), text);
	tok->type = tt;
	tok->text = text;
}

void horn_LH(horn_Token* tok, const char* src){
	if(g_horn_lookahead.type == HORN_TT_UKN)
		horn_next(&g_horn_lookahead, src);
	*tok = g_horn_lookahead;
}
