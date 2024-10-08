#ifndef _VEC_H_
#define _VEC_H_

#include <stddef.h>

typedef void* vec;
typedef struct vec_Header vec_Header;

struct vec_Header{
	size_t size;
	size_t cap;
	char data[];
};

vec vec_new(size_t cap);
void* vec_push(vec* v, size_t size);
void vec_pop(vec v, size_t size);
void* vec_insert(vec* v, size_t offset, size_t size);
void vec_delete(vec v, size_t offset, size_t size);
void vec_free(vec v);

#define vec_GET_HEAD(v) (&((vec_Header*)v)[-1])

#endif //_VEC_H_
