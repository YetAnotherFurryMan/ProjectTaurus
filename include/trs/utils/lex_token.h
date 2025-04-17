#ifndef _UTILS_LEX_TOKEN_H_
#define _UTILS_LEX_TOKEN_H_

#include <stdlib.h>

typedef struct utils_Token utils_Token;

struct utils_Token{
	size_t type;
	const char* begin;
	const char* end;
};

#endif //_UTILS_LEX_TOKEN_H_
