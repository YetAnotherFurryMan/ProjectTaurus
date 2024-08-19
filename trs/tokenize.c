#include <trs/ast.h>

#include <string.h>
#include <stdlib.h>

#include <toollib/carea/carea.h>

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
		if(c == '\\'){
			c = getc(f);
			switch(c){
				case '\"':
				case '\'':
					break;
				case 'n':
					c = '\n';
					break;
				case 't':
					c = '\t';
					break;
				// TODO: \v etc
			}
			// TODO \x \000
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
	Token tok = {0};
	if(feof(f)){
		tok.m_Type = TOK_EOF;
		return tok;
	}

	char c = getc(f);
	switch(c){
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
