#include <toollib/pgm.h>

#include <stdlib.h>

#ifndef PGM_PAGE_SIZE
#	define PGM_PAGE_SIZE (1024*4)
#endif

#define PGM_SIZE(BTS) (((BTS) / sizeof(pgm_unit)) + (((BTS) % sizeof(pgm_unit) != 0)?1:0))

pgm_Header* pgm_page(size_t size){
	// We need fit this header into memory
	if(size < sizeof(pgm_Header))
		size += sizeof(pgm_Header);

	// We need to presume memory aligment
	size += sizeof(pgm_unit) - (size % sizeof(pgm_unit));

	pgm_Header* page = malloc(size);
	if(!page)
		return NULL;
	
	page->size = PGM_SIZE(sizeof(pgm_Header));
	page->cap = size / sizeof(pgm_unit);
	page->next = NULL;
	page->data = (pgm_unit*)page;
	return page;
}

void pgm_clean(pgm* p){
	pgm_Header* head = p->begin;

	while(head){
		head->size = PGM_SIZE(sizeof(pgm_Header));
		head = head->next;
	}
}

void* pgm_alloc(pgm* p, size_t size){
	pgm_Header* head = p->begin;

	while(head){
		if(head->cap - head->size >= size){
			void* data = head->data + head->size;
			head->size += PGM_SIZE(size);
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
	next->size += PGM_SIZE(size);
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

