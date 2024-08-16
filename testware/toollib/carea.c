#include <toollib/carea/carea.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

#define ARR_S \
	(128 / sizeof(int))

int main(void){
	carea_Area area = carea_new(128);
	if(!area.m_Capacity) FAIL("Failed to allocate 128 bytes.");

	int* arr = carea_alloc(&area, ARR_S * sizeof(int));
	if(!arr) FAIL("Failed to allocate int array.");
	if(((carea_Area*)area.m_Data)[-1].m_Data) FAIL("New page was allocated.");
	
	for(size_t i = 0; i < ARR_S; i++)
		arr[i] = i;

	int* arr2 = carea_alloc(&area, ARR_S * sizeof(int));
	if(!arr2) FAIL("Failed to allocate int array 2.");
	
	for(size_t i = 0; i < ARR_S; i++)
		arr2[i] = i;

	if(!((carea_Area*)area.m_Data)[-1].m_Data) FAIL("New page was not allocated.");

	printf("OK\n");
	return 0;
}
