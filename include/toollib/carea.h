#ifndef _CAREA_H_
#define _CAREA_H_

#ifndef CAREA_PAGE_SIZE
#define CAREA_PAGE_SIZE 4 * 1024
#endif

#include <stddef.h>

struct carea_Header{
	size_t m_Size;
	size_t m_Capacity;
	struct carea_Header* m_Next;
	char m_Data[];
};

typedef struct carea_Header carea_Header;

void* carea_new();
void* carea_alloc(void* area, size_t n);
void carea_free(void* area);
void carea_freeHard(void* area);

#endif // _CAREA_H_
