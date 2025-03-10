#include <toollib/pgm.h>

#include <stdlib.h>

#ifndef PGM_PAGE_SIZE
#	define PGM_PAGE_SIZE 1024
#endif

pgm pgm_new(void){
	pgm_Header* head = malloc(PGM_PAGE_SIZE);
	if(!head)
		return NULL;
	
	head->size = 0;
	head->cap = PGM_PAGE_SIZE - sizeof(pgm_Header);
	head->next = NULL;

	return head;
}

void pgm_clean(pgm p){
	pgm_Header* head = p;

	while(head){
		head->size = 0;
		head = head->next;
	}
}

void* pgm_alloc(pgm p, size_t size){
	pgm_Header* head = p;

	if(!head)
		return NULL;

	while(1){
		if(head->cap - head->size >= size){
			void* data = head->data + head->size;
			head->size += size;
			return data;
		}

		if(head->next)
			head = head->next;
		else
			break;
	}

	size_t page_cap = size + sizeof(pgm_Header);
	if(page_cap < PGM_PAGE_SIZE)
		page_cap = PGM_PAGE_SIZE;

	pgm_Header* next = malloc(page_cap);
	if(!next)
		return NULL;

	next->size = size;
	next->cap = page_cap - sizeof(pgm_Header);
	next->next = NULL;

	head->next = next;

	return next->data;
}

void pgm_free(pgm p){
	pgm_Header* head = p;

	while(head){
		pgm_Header* next = head->next;
		free(head);
		head = next;
	}
}

