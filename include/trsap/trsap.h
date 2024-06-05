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

typedef enum {
	TRSAP_ARG_STATUS_EOI, // End Of Input
	TRSAP_ARG_STATUS_OK,
	TRSAP_ARG_STATUS_OK_VALUE,
	TRSAP_ARG_STATUS_OK_VALUE2,
	TRSAP_ARG_STATUS_ERR_VALUE,
	TRSAP_ARG_STATUS_ERR_VALUE2
} trsap_ArgStatus;

typedef struct trsap_Desc trsap_Desc;
typedef struct trsap_Arg trsap_Arg;

struct trsap_Desc{
	char m_Short = 0;
	const char* m_Long = 0;
	trsap_ArgType m_Type = TRSAP_ARG_TYPE_FLAG;
};

struct trsap_Arg{
	size_t m_Id = 0;
	const char* m_Value = 0;
	const char* m_Value2 = 0;
	trsap_ArgStatus m_Status = TRSAP_ARG_STATUS_EOI;
};

const char* trsap_chop(size_t* argc, const char*** argv);

#endif // _TRSAP_H_
