#include <toollib/pgm.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); print_pgm(&area); return 1; }

#define PGM_PAGE_SIZE (1024*4)

void print_pgm(pgm* p){
	pgm_Header* head = p->begin;
	while(head){
		printf("PGM{.size = %lu; .cap = %lu; .next = %p; .data = %p;}\n", head->size, head->cap, (void*)head->next, (void*)head->data);
		head = head->next;
	}
}

int main(void){
	pgm area = {0};

	for(size_t i = 0; i < PGM_PAGE_SIZE / sizeof(uintptr_t); i++){
		int* data = pgm_alloc(&area, sizeof(int));
		if(!data) FAIL("(1) Failed to allocate an int.");
		*data = (int)i;
	}

	{
		pgm_Header* head = area.begin;
		if(head->cap != PGM_PAGE_SIZE / sizeof(uintptr_t)) FAIL("(1) Bad PGM_PAGE_SIZE.");
		if(!head->next) FAIL("(1) Next was not allocated.");
		if(head->next->cap != PGM_PAGE_SIZE / sizeof(uintptr_t)) FAIL("(1) Bad next PGM_PAGE_SIZE.");
		if(head->next->next) FAIL("(1) Next next was allocated.");
	}

	pgm_clean(&area);

	for(size_t i = 0; i < PGM_PAGE_SIZE / sizeof(uintptr_t); i++){
		int* data = pgm_alloc(&area, sizeof(int));
		if(!data) FAIL("(2) Failed to allocate an int.");
		*data = (int)i;
	}

	{
		pgm_Header* head = area.begin;
		if(head->cap != PGM_PAGE_SIZE / sizeof(uintptr_t)) FAIL("(2) Bad PGM_PAGE_SIZE.");
		if(!head->next) FAIL("(2) Next was not allocated.");
		if(head->next->cap != PGM_PAGE_SIZE / sizeof(uintptr_t)) FAIL("(2) Bad next PGM_PAGE_SIZE.");
		if(head->next->next) FAIL("(2)  Next next was allocated.");
	}

	pgm_free(&area);

	printf("OK\n");
	return 0;
}
