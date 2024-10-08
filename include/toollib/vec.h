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

#define vec_GEN_FOR_TYPE(T)                                                                                                              \
	static inline T*   vec_new_##T(size_t n)                          { return vec_new(n * sizeof(T)); }                                 \
	static inline T*   vec_push_##T(T** v)                            { return vec_push((vec*) v, sizeof(T)); }                          \
	static inline T*   vec_pushN_##T(T** v, size_t n)                 { return vec_push((vec*) v, sizeof(T) * n); }                      \
	static inline void vec_pop_##T(T* v)                              { vec_pop(v, sizeof(T)); }                                         \
	static inline void vec_popN_##T(T* v, size_t n)                   { vec_pop(v, sizeof(T) * n); }                                     \
	static inline T*   vec_insert_##T(T** v, size_t index)            { return vec_insert((vec*) v, index * sizeof(T), sizeof(T)); }     \
	static inline T*   vec_insertN_##T(T** v, size_t index, size_t n) { return vec_insert((vec*) v, index * sizeof(T), sizeof(T) * n); } \
	static inline void vec_delete_##T(T* v, size_t index)             { vec_delete(v, index * sizeof(T), sizeof(T)); }                   \
	static inline void vec_deleteN_##T(T* v, size_t index, size_t n)  { vec_delete(v, index * sizeof(T), sizeof(T) * n); }               \
	static inline void vec_free_##T(T* v)                             { vec_free(v); }

#endif //_VEC_H_
