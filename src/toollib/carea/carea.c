#include <toollib/carea/carea.h>

#include <malloc.h>
#include <string.h>

void* carea_new(){
	carea_Header* data = malloc(CAREA_PAGE_SIZE);
	if(!data)
		return NULL;

	data->m_Size = sizeof(carea_Header);
	data->m_Capacity = CAREA_PAGE_SIZE;
	data->m_Next = 0;

	return data;
}

void* carea_alloc(void* area, size_t n){
	carea_Header* head = area;

	if(head->m_Capacity - head->m_Size >= n){
		char* data = head->m_Data + head->m_Size;
		head->m_Size += n;
		return data;
	}

	if(head->m_Next)
		return carea_alloc(head->m_Next, n);

	n += sizeof(carea_Header);

	size_t page = n > CAREA_PAGE_SIZE ? n : CAREA_PAGE_SIZE;
	carea_Header* next = malloc(page);
	if(!next)
		return NULL;

	next->m_Size = n;
	next->m_Capacity = page;
	next->m_Next = 0;
	head->m_Next = next;

	return next->m_Data + sizeof(carea_Header);
}

void carea_free(void* area){
	carea_Header* head = area;
	while(head){
		head->m_Size = sizeof(carea_Header);
		head = head->m_Next;
	}
}

void carea_freeHard(void* area){
	carea_Header* head = area;
	while(head){
		void* next = head->m_Next;
		free(head);
		head = next;
	}
}

