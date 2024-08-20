#include <toollib/cvec/cvec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

int main(void){
	if(cvec_new(0, 10)) FAIL("Element cannot be of size 0.");
	if(cvec_new(10, 0)) FAIL("Amount cannot be 0.");

	int* vec = cvec_new(sizeof(int), 10);
	if(!vec) FAIL("Failed to allocate memeory for int[10].");
	if(cvec_length(vec) != 0) FAIL("Length != 0.");
	if(cvec_size(vec) != 10 * sizeof(int)) FAIL("Size != sizeof(int[10]).");

	cvec_addArrayN(vec, NULL, 10);

	if(cvec_length(vec) != 10) FAIL("Length != 10.");
	if(cvec_size(vec) != 10 * sizeof(int)) FAIL("Size != sizeof(int[10]).");

	for(size_t i = 0; i < 10; i++)
		vec[i] = i;

	if(cvec_length(vec) != 10) FAIL("Length != 10.");
	if(cvec_size(vec) != 10 * sizeof(int)) FAIL("Size != sizeof(int[10]).");
	
	for(int i = 0; i < 10; i++)
		if(vec[i] != i) FAIL("Failed to fill int[10].");

	if(cvec_needsRealloc(vec, 0)) FAIL("int[10] needs realloc to fit 0 additional elements.");
	if(!cvec_needsRealloc(vec, 10)) FAIL("int[10] does not need to be realloced for 10 additional ints.");

	vec = cvec_ensure(vec, 10);
	if(!vec) FAIL("Failed to reallocate int[10] to int[19].");

	if(cvec_length(vec) != 10) FAIL("Length != 10.");
	if(cvec_size(vec) < 19 * sizeof(int)) FAIL("int[19] do not fit 19 ints.");

	if(cvec_needsRealloc(vec, 10)) FAIL("int[19] needs to be realloced for 20 ints.");
	if(!cvec_needsRealloc(vec, 20)) FAIL("int[19] does not need to be realloced for 20 ints.");

	for(int i = 0; i < 10; i++)
		cvec_addN(vec, &i);

	for(int i = 0; i < 20; i++)
		if(vec[i] != i % 10) FAIL("Failed to fill by cvec_add");

	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	cvec_addArrayN(vec, arr, 10);

	for(int i = 0; i < 20; i++)
		if(vec[i] != i % 10) FAIL("Failed to fill by cvec_addArray");

	for(size_t i = 0; i < 10; i++)
		if(vec[i + 20] != arr[i]) FAIL("Failed to fill with cvec_addArray");

	if(cvec_length(vec) != 30) FAIL("Length != 30");

	cvec_insertN(vec, 1, arr);

	if(cvec_length(vec) != 31) FAIL("Length != 31");
	if(vec[0] != 0) FAIL("Failed to insert element.");
	if(vec[1] != arr[0]) FAIL("Failed to insert element.");

	for(int i = 1; i < 20; i++)
		if(vec[i + 1] != i % 10) FAIL("Failed to insert #1");

	for(size_t i = 0; i < 10; i++)
		if(vec[i + 21] != arr[i]) FAIL("Failed to insert #2");

	cvec_insertArrayN(vec, 21, arr, 10);

	if(cvec_length(vec) != 41) FAIL("Length != 41");
	if(vec[0] != 0) FAIL("Failed to insert array.");
	if(vec[1] != arr[0]) FAIL("Failed to insert array.");

	for(int i = 1; i < 20; i++)
		if(vec[i + 1] != i % 10) FAIL("Failed to insert array #1");

	for(size_t i = 0; i < 10; i++)
		if(vec[i + 21] != arr[i]) FAIL("Failed to insert array #2");

	for(size_t i = 0; i < 10; i++)
		if(vec[i + 31] != arr[i]) FAIL("Failed to insert array #3");

	cvec_free(vec);

	printf("OK\n");
	return 0;
}
