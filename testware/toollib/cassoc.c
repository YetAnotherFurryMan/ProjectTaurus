#include <toollib/cvec/cvec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

int main(void){
	if(cassoc_new(0, 10)) FAIL("Element cannot be of size 0.");
	if(cassoc_new(10, 0)) FAIL("Amount cannot be 0.");

	int* vec = cassoc_new(sizeof(int), 10);

	cassoc_free(vec);

	printf("OK\n");
	return 0;
}
