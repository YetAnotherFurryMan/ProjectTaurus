#ifndef _UTILS_LEX_TOKEN_H_
#define _UTILS_LEX_TOKEN_H_

#include <stdlib.h>
#include <string.h>

#include <toollib/pgm.h>

typedef struct utils_Token utils_Token;

struct utils_Token{
	size_t type;
	const char* begin;
	const char* end;
};

inline static char* utils_strtok(pgm* pgm, const utils_Token* tok){
	if(!tok || !tok->begin || !tok->end)
		return NULL;

	size_t len = tok->end - tok->begin;

	char* text = 0;
	if(pgm)
		text = pgm_alloc(pgm, len + 1);
	else
		text = malloc(len + 1);

	if(!text)
		return NULL;

	memcpy(text, tok->begin, len);
	text[len] = 0;

	return text;
}

#endif //_UTILS_LEX_TOKEN_H_
