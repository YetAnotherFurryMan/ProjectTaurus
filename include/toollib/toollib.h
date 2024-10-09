#ifndef _TOOLLIB_H_
#define _TOOLLIB_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static inline uint64_t tl_hash(const char* str){
	// DJB2
	unsigned long h = 5381;

	char c;
	while((c = *str)){
		h = (h << 5) + h + c; // h * 33 * c
		str++;
	}

	return (uint64_t)(h % -1L);
}

static inline char* tl_strcpy(const char* s){
	size_t len = strlen(s) + 1;
	char* cpy = (char*)malloc(len);
	if(!cpy)
		return NULL;

	memcpy(cpy, s, len);
	return cpy;
}

#endif // _TOOLLIB_H_
