#include <toollib/carea.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

#define ARR_S \
	(128 / sizeof(int))

int main(void){
	void* area = carea_new();
	if(!area) FAIL("Failed to allocate area bytes.");

	int* arr = carea_alloc(area, ARR_S * sizeof(int));
	if(!arr) FAIL("Failed to allocate int array.");
	
	for(size_t i = 0; i < ARR_S; i++)
		arr[i] = i;

	int* arr2 = carea_alloc(area, ARR_S * sizeof(int));
	if(!arr2) FAIL("Failed to allocate int array 2.");
	
	for(size_t i = 0; i < ARR_S; i++)
		arr2[i] = i;

	carea_free(area);

	int* arr3 = carea_alloc(area, ARR_S * sizeof(int));
	if(!arr3) FAIL("Failed to allocate int array.");
	if(arr3 != arr) FAIL("Failed to reproduce arr1 ptr.");
	
	int* arr4 = carea_alloc(area, ARR_S * sizeof(int));
	if(!arr4) FAIL("Failed to allocate int array 2.");
	if(arr4 != arr2) FAIL("Failed to reproduce arr2 ptr.");

	carea_freeHard(area);

	printf("OK\n");
	return 0;
}
