#include <toollib/assoc.h>

#include <stdio.h>
#include <string.h>

#define FAIL(MSG) { printf("%s\nFAILED\n", MSG); return 1; }

#define DEPTH 10000

assoc_GEN_FOR_TYPE(int)

int main(void){
	assoc a = assoc_new_int(0);
	if(!a) FAIL("Failed to allocate new assoc.");

	int* val1 = assoc_set_int(&a, "val1");
	if(!val1) FAIL("Failed to set val1.");
	*val1 = 1;

	{
		assoc_Header* head = a;
		if(head->keys[0].data != val1) FAIL("val1's data do not point to val1.");
		if(strcmp(head->keys[0].key, "val1") != 0) FAIL("val1 is not first.");
	}

	if(assoc_valueAt_int(a, 0) != val1 || strcmp(assoc_keyAt_int(a, 0)->key, "val1") != 0) FAIL("Failed to assoc_XAt.");

	int* val2 = assoc_set_int(&a, "val2");
	if(!val2) FAIL("Failed to set val2.");
	*val2 = 2;

	{
		assoc_Header* head = a;
		if(head->count != 2) FAIL("Bad count.");
	}

	assoc_Key_int* val1k = assoc_find_int(a, "val1");
	if(!val1k) FAIL("Failed to find val1.");
	if(strcmp(val1k->key, "val1") != 0) FAIL("Founded val1 is not val1.");

	assoc_Key_int* val2k = assoc_find_int(a, "val2");
	if(!val2k) FAIL("Failed to find val2.");
	if(strcmp(val2k->key, "val2") != 0) FAIL("Founded val2 is not val2.");
	if(*val2k->data != *val2) FAIL("val2 != val2.");

	for(size_t i = 0; i < DEPTH; i++){
		char str[7] = {
			(char)(i % 10)           + '0', 
			(char)((i / 10) % 10)    + 'A', 
			(char)((i / 100) % 10)   + 'a', 
			(char)((i / 1000) % 10)  + '0', 
			(char)((i / 10000) % 10) + 'A',
			(char)(i / 100000)       + 'a',
			0
		};
		int* val = assoc_set_int(&a, str);
		*val = 1;
		for(size_t i = 0; i < 7; i++)
			*val *= str[i];
	}

	for(size_t i = 0; i < DEPTH; i++){
		int v = 1;
		char str[7] = {
			(char)(i % 10)           + '0', 
			(char)((i / 10) % 10)    + 'A', 
			(char)((i / 100) % 10)   + 'a', 
			(char)((i / 1000) % 10)  + '0', 
			(char)((i / 10000) % 10) + 'A',
			(char)(i / 100000)       + 'a',
			0
		};

		for(size_t i = 0; i < 7; i++)
			v *= str[i];
		
		assoc_Key_int* val = assoc_find_int(a, str);
		if(!val) FAIL("Failed to find.");
		if(strcmp(val->key, str) != 0) FAIL("Found bad val.");
		if(*val->data != v) FAIL("Found val with bad data.");
		if(*assoc_get_int(a, str) != v) FAIL("Failed to get.");
	}

	assoc_free_int(a);
	
	printf("OK\n");
	return 0;
}
