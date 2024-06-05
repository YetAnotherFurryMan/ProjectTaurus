#ifndef _TRSAP_H_
#define _TRSAP_H_

#include <stddef.h>

typedef enum {
	TRSAP_ARG_TYPE_FLAG = 0,
	TRSAP_ARG_TYPE_VALUE,
	TRSAP_ARG_TYPE_VALUE_OPTIONAL,
	TRSAP_ARG_TYPE_VALUE2,
	TRSAP_ARG_TYPE_VALUE2_OPTIONAL
} trsap_ArgType;

const char* trsap_chop(size_t* argc, const char*** argv);

#endif // _TRSAP_H_
