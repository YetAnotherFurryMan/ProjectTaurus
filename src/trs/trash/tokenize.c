#include <trs/ast.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <toollib/carea/carea.h>

#include <trs/error.h>
#include <trs/state.h>

Token g_Lookahed = {0};

// TODO: Fix s->m_ColNo to point at the begining not the end of the token

static inline void nextTokenStr(State* s, FILE* f, Token* tok){
	void* area = s->m_Area;
	char c = getc(f); // Consume "
	s->m_ColNo++;

	size_t i = 0;
	size_t len = 32;
	char* str = malloc(32);
	if(!str)
		yell(FE_OUT_OF_MEMORY);

	while(c != '\"' && !feof(f)){
		if(c == '\n'){
			s->m_LineNo++;
			s->m_ColNo = 0;
		}

		if(c == '\\'){
			c = getc(f);
			s->m_ColNo++;
			switch(c){
				case '\"':
				case '\'':
				case '\\':
					break;
				case 't':
					c = '\t'; // 0x09
					break;
				case 'n':
					c = '\n'; // 0x0a
					break;
				case 'v':
					c = '\v'; // 0x0b
					break;
				case 'f':
					c = '\f'; // 0x0c
					break;
				case 'r':
					c = '\r'; // 0x0d
					break;
			}
			// TODO \x00 \000
		}

		str[i++] = c;
		c = getc(f);
		s->m_ColNo++;

		if(i >= len){
			len += 8;
			str = realloc(str, len);
			if(!str)
				yell(FE_OUT_OF_MEMORY);
		}
	}

	str[i++] = 0;

	tok->m_Value = carea_alloc(area, i);
	if(!tok->m_Value)
		yell(FE_OUT_OF_MEMORY);

	memcpy((void*)tok->m_Value, str, i);
	free(str);

	tok->m_Type = TOK_STR;
}

static inline void nextTokenNum(State* s, FILE* f, Token* tok){
	char c = getc(f);
	s->m_ColNo++;

	size_t i = 0;
	char* str = malloc(21);
	if(!str)
		yell(FE_OUT_OF_MEMORY);


	while(c >= '0' && c <= '9'){
		str[i++] = c;
		c = fgetc(f);
		s->m_ColNo++;

		if(i > 20)
			yellp(E_UNPARSABLE_NUMBER, s->m_FileName, s->m_LineNo, s->m_ColNo);
	}
	ungetc(c ,f);

	str[i++] = 0;

	tok->m_Value = carea_alloc(s->m_Area, i);
	if(!tok->m_Value)
		yell(FE_OUT_OF_MEMORY);


	memcpy((void*)tok->m_Value, str, i);
	free(str);

	tok->m_Type = TOK_NUM;
}

static inline void nextTokenId(State* s, FILE* f, Token* tok){
	char c = getc(f);
	s->m_ColNo++;
	
	size_t i = 0;
	size_t len = 32;
	char* str = malloc(32);
	if(!str)
		yell(FE_OUT_OF_MEMORY);


	while(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') && !feof(f)){
		str[i++] = c;
		c = getc(f);
		s->m_ColNo++;

		if(i >= len){
			if(i >= 256){
				fprintf(stderr, "ERROR: Names bigger then 256 chars are not allowed.\n");
				// TODO: Print location.
				exit(1);
			}

			len += 8;
			str = realloc(str, len);
			if(!str)
				yell(FE_OUT_OF_MEMORY);

		}
	}
	ungetc(c, f);
	s->m_ColNo--;

	str[i++] = 0;

	tok->m_Value = carea_alloc(s->m_Area, i);
	if(!tok->m_Value)
		yell(FE_OUT_OF_MEMORY);


	memcpy((void*)tok->m_Value, str, i);
	free(str);

	tok->m_Type = TOK_ID;
}

Token nextToken(State* s, FILE* f){
	if(g_Lookahed.m_Type != 0){
		Token tok = g_Lookahed;
		g_Lookahed.m_Type = 0;
		return tok;
	}

	Token tok = {0};
	if(feof(f)){
		tok.m_Type = TOK_EOF;
		return tok;
	}

	char c = getc(f);
	while(isspace(c)){
		if(c == '\n'){
			s->m_LineNo++;
			s->m_ColNo = 0;
		}

		c = getc(f);
		s->m_ColNo++;
	}

	if(feof(f)){
		tok.m_Type = TOK_EOF;
		return tok;
	}

	bool nextEq = false;

	switch(c){
		case ';':
			tok.m_Type = TOK_SEMICOLON;
			tok.m_Value = ";";
			break;
		case ',':
			tok.m_Type = TOK_COMMA;
			tok.m_Value = ",";
			break;
		case '(':
			tok.m_Type = TOK_PRENTICE_L;
			tok.m_Value = "(";
			break;
		case ')':
			tok.m_Type = TOK_PRENTICE_R;
			tok.m_Value = ")";
			break;
		case '=':
			tok.m_Type = TOK_OP_EQ;
			tok.m_Value = "=";
			nextEq = true;
			break;
	}

	if(tok.m_Type == TOK_ERR){
		if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'){
			ungetc(c, f);
			s->m_ColNo--;
			nextTokenId(s, f, &tok);
		} else if(c >= '0' && c <= '9'){
			ungetc(c, f);
			s->m_ColNo--;
			nextTokenNum(s, f, &tok);
		} else if(c == '\"'){
			nextTokenStr(s, f, &tok);
		}
	} else if(nextEq){
		c = getc(f);
		s->m_ColNo++;
		if(c == '='){
			// TODO
		} else{
			ungetc(c, f);
			s->m_ColNo--;
		}
	}

	return tok;
}
