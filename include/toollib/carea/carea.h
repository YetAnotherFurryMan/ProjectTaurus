#ifndef _CAREA_H_
#define _CAREA_H_

#ifndef CAREA_PAGE_SIZE
#define CAREA_PAGE_SIZE 4 * 1024
#endif

#include <stddef.h>

typedef struct carea_Area{
	size_t m_Capacity;
	size_t m_Size;
	void* m_Data;
} carea_Area;

carea_Area carea_new(size_t cap);
void* carea_alloc(carea_Area* area, size_t n);

#endif // _CAREA_H_
