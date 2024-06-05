#include <trsap/trsap.h>

const char* trsap_chop(size_t* argc, const char*** argv){
	if(!(*argc))
		return 0;

	const char* arg = **argv;
	(*argv)++;
	(*argc)--;
	return arg;
}

