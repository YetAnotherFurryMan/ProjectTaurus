#include <toollib/carea/carea.h>

#include <malloc.h>
#include <string.h>

carea_Area carea_new(size_t cap){
	carea_Area area = {0};

	carea_Area* data = malloc(cap + sizeof(carea_Area));
	if(!data)
		return area;

	memset(data, 0, sizeof(carea_Area));
	area.m_Data = data + 1;
	area.m_Capacity = cap;

	return area;
}

void* carea_alloc(carea_Area* area, size_t n){
	if(area->m_Capacity - area->m_Size >= n){
		area->m_Size += n;
		return ((char*)area->m_Data) + n + area->m_Size;
	}

	carea_Area* a = &((carea_Area*)area->m_Data)[-1];
	if(!a->m_Data){
		size_t nn = area->m_Capacity + (2 * n);
		if(nn > CAREA_PAGE_SIZE - sizeof(carea_Area)) nn = CAREA_PAGE_SIZE - sizeof(carea_Area);
		if(nn < n) nn = n;

		carea_Area* data = malloc(nn + sizeof(carea_Area));
		if(!data)
			return 0;

		memset(data, 0, sizeof(carea_Area));
		a->m_Data = data + 1;
		a->m_Capacity = nn;
		a->m_Size = n;
		return a->m_Data;
	}

	return carea_alloc(a, n);
}

