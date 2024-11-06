#include <toollib/pgm.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

#define PGM_PAGE_SIZE 1024

int main(void){
	pgm area = pgm_new();
	if(!area) FAIL("Failed to create new PGM.");

	for(size_t i = 0; i < PGM_PAGE_SIZE / sizeof(int); i++){
		int* data = pgm_alloc(area, sizeof(int));
		if(!data) FAIL("Failed to allocate an int.");
		*data = (int)i;
	}

	{
		pgm_Header* head = area;
		if(head->cap != PGM_PAGE_SIZE - sizeof(pgm_Header)) FAIL("Bad PGM_PAGE_SIZE.");
		if(!head->next) FAIL("Next was not allocated.");
		if(head->next->cap != PGM_PAGE_SIZE - sizeof(pgm_Header)) FAIL("Bad next PGM_PAGE_SIZE.");
		if(head->next->next) FAIL("Next next was allocated.");
	}

	pgm_clean(area);

	for(size_t i = 0; i < PGM_PAGE_SIZE / sizeof(int); i++){
		int* data = pgm_alloc(area, sizeof(int));
		if(!data) FAIL("Failed to allocate an int.");
		*data = (int)i;
	}

	{
		pgm_Header* head = area;
		if(head->cap != PGM_PAGE_SIZE - sizeof(pgm_Header)) FAIL("Bad PGM_PAGE_SIZE.");
		if(!head->next) FAIL("Next was not allocated.");
		if(head->next->cap != PGM_PAGE_SIZE - sizeof(pgm_Header)) FAIL("Bad next PGM_PAGE_SIZE.");
		if(head->next->next) FAIL("Next next was allocated.");
	}

	pgm_free(area);

	printf("OK\n");
	return 0;
}
