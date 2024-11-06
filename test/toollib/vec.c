#include <toollib/vec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

int main(void){
	vec v1 = vec_new(4 * sizeof(int));
	if(!v1) FAIL("Failed to allocate new vec with 4*int cap.");

	vec v1b = v1;
	vec_push(&v1, 2 * sizeof(int));
	if(v1b != v1) FAIL("Reallocated even if not needed.");

	vec_free(v1);

	vec v = vec_new(0);
	if(!v) FAIL("Failed to allocate new vec.");

	int* arr1 = vec_push(&v, sizeof(int) * 10);
	if(!arr1) FAIL("Failed to push 10 elements.");

	for(size_t i = 0; i < 10; i++)
		arr1[i] = i % 5;

	for(size_t i = 0; i < 10; i++)
		if(arr1[i] != (int) i % 5) FAIL("Failed to fill arr1.");

	vec_pop(v, sizeof(int));
	if(vec_GET_HEAD(v)->size != sizeof(int) * 9) FAIL("Failed to pop from arr1.");

	for(size_t i = 0; i < 9; i++)
		if(arr1[i] != (int) i % 5) FAIL("Failed to pop from arr1 (bad value).");

	vec_delete(v, sizeof(int) * 4, sizeof(int));
	if(vec_GET_HEAD(v)->size != sizeof(int) * 8) FAIL("Failed to delete from arr1.");

	for(size_t i = 0; i < 8; i++)
		if(arr1[i] != (int) i % 4) FAIL("Failed to delete from arr1 (bad value).");

	vec_insert(&v, sizeof(int) * 4, sizeof(int) * 4);
	if(vec_GET_HEAD(v)->size != sizeof(int) * 12) FAIL("Failed to insert to arr1.");

	for(size_t i = 0; i < 4; i++)
		arr1[i + 4] = i;

	for(size_t i = 0; i < 12; i++)
		if(arr1[i] != (int) i % 4) FAIL("Failed to insert to arr1 (bad value).");

	vec_free(v);

	printf("OK\n");
	return 0;
}
