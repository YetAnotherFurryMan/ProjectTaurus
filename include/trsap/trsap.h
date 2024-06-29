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
	TRSAP_ARG_STATUS_EOI = 0, // End Of Input
	TRSAP_ARG_STATUS_OK,
	TRSAP_ARG_STATUS_ERR_SHORT,
	TRSAP_ARG_STATUS_ERR_LONG,
	TRSAP_ARG_STATUS_ERR_VALUE,
	TRSAP_ARG_STATUS_ERR_VALUE2
} trsap_ArgStatus;

typedef struct trsap_Desc trsap_Desc;
typedef struct trsap_Arg trsap_Arg;

struct trsap_Desc{
	char m_Short;
	const char* m_Long;
	trsap_ArgType m_Type;
};

struct trsap_Arg{
	int m_Id;
	const char* m_Value;
	const char* m_Value2;
	size_t m_ValueLen;
	size_t m_Value2Len;
	trsap_ArgStatus m_Status;
};

const char* trsap_chop(int* argc, const char*** argv);
trsap_Arg trsap_next(size_t descc, trsap_Desc* descv, int* argc, const char*** argv);

inline const char* trsap_chop2(int* argc, const char* const** argv){
	return trsap_chop(argc, (const char***) argv);
}

inline trsap_Arg trsap_next2(size_t descc, trsap_Desc* descv, int* argc, const char* const** argv){
	return trsap_next(descc, descv, argc, (const char***) argv);
}

#endif // _TRSAP_H_
