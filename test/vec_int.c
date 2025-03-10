#include <toollib/vec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

vec_GEN_FOR_TYPE(int)

int main(void){
	int* v = vec_new_int(0);
	if(!v) FAIL("Failed to allocate new vec.");

	int* arr1 = vec_pushN_int(&v, 10);
	if(!arr1) FAIL("Failed to push 10 elements.");

	for(size_t i = 0; i < 10; i++)
		arr1[i] = i % 5;

	for(size_t i = 0; i < 10; i++)
		if(arr1[i] != (int) i % 5) FAIL("Failed to fill arr1.");

	vec_pop_int(v);
	if(vec_GET_HEAD(v)->size != sizeof(int) * 9) FAIL("Failed to pop from arr1.");

	for(size_t i = 0; i < 9; i++)
		if(arr1[i] != (int) i % 5) FAIL("Failed to pop from arr1 (bad value).");

	vec_delete_int(v, 4);
	if(vec_GET_HEAD(v)->size != sizeof(int) * 8) FAIL("Failed to delete from arr1.");

	for(size_t i = 0; i < 8; i++)
		if(arr1[i] != (int) i % 4) FAIL("Failed to delete from arr1 (bad value).");

	vec_insertN_int(&v, 4, 4);
	if(vec_GET_HEAD(v)->size != sizeof(int) * 12) FAIL("Failed to insert to arr1.");

	for(size_t i = 0; i < 4; i++)
		arr1[i + 4] = i;

	for(size_t i = 0; i < 12; i++)
		if(arr1[i] != (int) i % 4) FAIL("Failed to insert to arr1 (bad value).");

	vec_free_int(v);

	printf("OK\n");
	return 0;
}
