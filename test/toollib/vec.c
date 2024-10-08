#include <toollib/vec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

int main(void){
	vec v1 = vec_new(4 * sizeof(int));
	if(!v1) FAIL("Failed to allocate new vec with 4*int cap.");

	vec v1b = v1;
	vec_push(&v1, 2 * sizeof(int));
	if(v1b != v1) FAIL("Reallocated even if not needed.");

	vec_push(&v1, 4 * sizeof(int));
	vec_free(v1);

	vec v = vec_new(0);
	if(!v) FAIL("Failed to allocate new vec.");

	int* arr1 = vec_push(&v, sizeof(int) * 10);
	if(!arr1) FAIL("Failed to push 10 elements.");

	for(size_t i = 0; i < 10; i++)
		arr1[i] = i % 5;

	for(size_t i = 0; i < 10; i++)
		if(arr1[i] != i % 5) FAIL("Failed to fill arr1.");

	vec_free(v);

	printf("OK\n");
	return 0;
}
