#include <trs/ast.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <toollib/carea/carea.h>

Token g_Lookahed = {0};
size_t g_LineNo = 0;

static inline void nextTokenStr(void* area, FILE* f, Token* tok){
	char c = getc(f); // Consume "

	size_t i = 0;
	size_t len = 32;
	char* str = malloc(32);
	if(!str){
		fprintf(stderr, "ERROR: Out of memory.\n");
		exit(1);
	}

	while(c != '\"' && !feof(f)){
		if(c == '\n')
			g_LineNo++;

		if(c == '\\'){
			c = getc(f);
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

		if(i >= len){
			len += 8;
			str = realloc(str, len);
			if(!str){
				fprintf(stderr, "ERROR: Out of memory.\n");
				exit(1);
			}
		}
	}

	str[i++] = 0;

	tok->m_Value = carea_alloc(area, i);
	if(!tok->m_Value){
		fprintf(stderr, "ERROR: Out of memory.\n");
		exit(1);
	}

	memcpy((void*)tok->m_Value, str, i);
	free(str);

	tok->m_Type = TOK_STR;
}

static inline void nextTokenId(void* area, FILE* f, Token* tok){
	char c = getc(f);
	
	size_t i = 0;
	size_t len = 32;
	char* str = malloc(32);
	if(!str){
		fprintf(stderr, "ERROR: Out of memory.\n");
		exit(1);
	}

	while(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') && !feof(f)){
		str[i++] = c;
		c = getc(f);

		if(i >= len){
			if(i >= 256){
				fprintf(stderr, "ERROR: Names bigger then 256 chars are not allowed.\n");
				// TODO: Print location.
				exit(1);
			}

			len += 8;
			str = realloc(str, len);
			if(!str){
				fprintf(stderr, "ERROR: Out of memory.\n");
				exit(1);
			}
		}
	}
	ungetc(c, f);

	str[i++] = 0;

	tok->m_Value = carea_alloc(area, i);
	if(!tok->m_Value){
		fprintf(stderr, "ERROR: Out of memory.\n");
		exit(1);
	}

	memcpy((void*)tok->m_Value, str, i);
	free(str);

	tok->m_Type = TOK_ID;
}

Token nextToken(void* area, FILE* f){
	if(g_Lookahed.m_Type != 0){
		Token tok = g_Lookahed;
		g_Lookahed.m_Type = 0;
		return tok;
	}

	Token tok = {0};
	if(feof(f)){
		tok.m_LineNo = g_LineNo;
		tok.m_Type = TOK_EOF;
		return tok;
	}

	char c = getc(f);
	while(isspace(c)){
		if(c == '\n')
			g_LineNo++;
		c = getc(f);
	}

	tok.m_LineNo = g_LineNo;
	
	if(feof(f)){
		tok.m_Type = TOK_EOF;
		return tok;
	}
	
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
		}

	if(tok.m_Type == TOK_ERR){
		if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'){
			ungetc(c, f);
			nextTokenId(area, f, &tok);
		} else if(c == '\"'){
			nextTokenStr(area, f, &tok);
		}
	}

	return tok;
}
