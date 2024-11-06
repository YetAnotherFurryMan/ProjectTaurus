#include <trs/parser.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

trs_Token g_trs_lexLookahead = {0};

void trs_lexNext(trs_Token* tok, const char* src){
	static const char* s = NULL;
	if(src){
		s = src;
		g_trs_lexLookahead = (trs_Token){0};
	}

	if(g_trs_lexLookahead.type != TRS_TT_UKN){
		*tok = g_trs_lexLookahead;
		g_trs_lexLookahead = (trs_Token){0};
		fprintf(stderr, "INFO: Lookahead(%s): %s\n", trs_TokenTypeToString(tok->type), tok->text);
		return;
	}

	if(!s || !*s){
		*tok = (trs_Token){TRS_TT_EOF, NULL};
		return;
	}

	while(*s && isspace(*s))
		s++;

	if(!*s){
		s = NULL;
		*tok = (trs_Token){TRS_TT_EOF, NULL};
		return;
	}

	trs_TokenType tt = TRS_TT_UKN;
	char* text = NULL;

	switch(*s){
		case '=':
			tt = TRS_TT_OP_EQ;
			break;
		case '+':
			tt = TRS_TT_OP_PLUS;
			break;
		case '*':
			tt = TRS_TT_OP_MUL;
			break;
		case ';':
			tt = TRS_TT_EOE;
			break;
		case '(':
			tt = TRS_TT_LB;
			break;
		case ')':
			tt = TRS_TT_RB;
			break;
		default:
		{
			if(isdigit(*s)){
				tt = TRS_TT_INT;

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
				tt = TRS_TT_ID;

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
	fprintf(stderr, "INFO: Token(%s): %s\n", trs_TokenTypeToString(tt), text);
	tok->type = tt;
	tok->text = text;
}

void trs_lexLH(trs_Token* tok, const char* src){
	if(g_trs_lexLookahead.type == TRS_TT_UKN)
		trs_lexNext(&g_trs_lexLookahead, src);
	*tok = g_trs_lexLookahead;
}
