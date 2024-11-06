#ifndef _PGM_H_
#define _PGM_H_

#include <stddef.h>

typedef void* pgm;
typedef struct pgm_Header pgm_Header;

struct pgm_Header{
	size_t size;
	size_t cap;
	pgm_Header* next;
	char data[];
};

pgm pgm_new(void);
void* pgm_alloc(pgm p, size_t size);
void pgm_clean(pgm p);
void pgm_free(pgm p);

#define pgm_allocT(P, T) ((T*) pgm_alloc(P, sizeof(T)))
#define pgm_allocTN(P, T, N) ((T*) pgm_alloc(P, sizeof(T) * N))

#endif //_PGM_H_
