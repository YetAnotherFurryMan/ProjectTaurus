#include <toollib/cvec.h>

#include <stdio.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

int main(void){
	if(cassoc_new(0, 10)) FAIL("Element cannot be of size 0.");
	if(cassoc_new(10, 0)) FAIL("Amount cannot be 0.");

	int* vec = cassoc_new(sizeof(int), 10);

	char buff[4] = {0};
	for(int i = 0; i < 10; i++){
		buff[0] = buff[1] = i + '0';
		buff[2] = i + 'a';
		cassoc_set(vec, buff, &i);
	}

	if(cvec_length(vec) != 10) FAIL("Length or set do not work...");

	for(int i = 0; i < 10; i++){
		buff[0] = buff[1] = i + '0';
		buff[2] = i + 'a';

		if(*((int*)cassoc_get(vec, buff)) != i) FAIL("Failed to set.");
	}

	cassoc_free(vec);

	printf("OK\n");
	return 0;
}
