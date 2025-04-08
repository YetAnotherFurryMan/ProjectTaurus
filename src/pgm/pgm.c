#include <toollib/pgm.h>

#include <stdlib.h>

#ifndef PGM_PAGE_SIZE
#	define PGM_PAGE_SIZE (1024*4)
#endif

pgm_Header* pgm_page(size_t size){
	// We need fit this header into memory
	if(size < sizeof(pgm_Header))
		size += sizeof(pgm_Header);

	// We need to presume memory aligment
	size += size % sizeof(uintptr_t);

	pgm_Header* page = malloc(size + sizeof(pgm_Header));
	if(!page)
		return NULL;
	
	page->size = (sizeof(pgm_Header) + sizeof(pgm_Header) % sizeof(uintptr_t)) / sizeof(uintptr_t);
	page->cap = size / sizeof(uintptr_t);
	page->next = NULL;
	page->data = (uintptr_t*)page;
	return page;
}

void pgm_clean(pgm* p){
	pgm_Header* head = p->begin;

	while(head){
		head->size = sizeof(pgm_Header) + (sizeof(pgm_Header) % sizeof(uintptr_t));
		head = head->next;
	}
}

void* pgm_alloc(pgm* p, size_t size){
	pgm_Header* head = p->begin;

	while(head){
		if(head->cap - head->size >= size){
			void* data = head->data + head->size;
			head->size += (size + size % sizeof(uintptr_t)) / sizeof(uintptr_t);
			return data;
		}

		head = head->next;
	}

	size_t page_cap = size + sizeof(pgm_Header);
	if(page_cap < PGM_PAGE_SIZE)
		page_cap = PGM_PAGE_SIZE;

	pgm_Header* next = pgm_page(page_cap);
	if(!next)
		return NULL;

	if(!p->begin){
		p->begin = p->end = next;
	} else{
		p->end->next = next;
		p->end = next;
	}

	void* data = next->data + next->size;
	next->size += (size + size % sizeof(uintptr_t)) / sizeof(uintptr_t);
	return data;
}

void pgm_free(pgm* p){
	pgm_Header* head = p->begin;

	while(head){
		pgm_Header* next = head->next;
		free(head);
		head = next;
	}
}

