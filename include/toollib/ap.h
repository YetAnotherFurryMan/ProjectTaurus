#ifndef _TOOLLIB_AP_H_
#define _TOOLLIB_AP_H_

#include <stddef.h>

typedef enum {
	AP_ARG_TYPE_FLAG = 0,
	AP_ARG_TYPE_VALUE,
	AP_ARG_TYPE_VALUE_OPTIONAL,
	AP_ARG_TYPE_VALUE2,
	AP_ARG_TYPE_VALUE2_OPTIONAL
} ap_ArgType;

typedef enum {
	AP_ARG_STATUS_EOI = 0, // End Of Input
	AP_ARG_STATUS_OK,
	AP_ARG_STATUS_ERR_SHORT,
	AP_ARG_STATUS_ERR_LONG,
	AP_ARG_STATUS_ERR_VALUE,
	AP_ARG_STATUS_ERR_VALUE2
} ap_ArgStatus;

typedef struct ap_Desc ap_Desc;
typedef struct ap_Arg ap_Arg;

struct ap_Desc{
	char m_Short;
	const char* m_Long;
	ap_ArgType m_Type;
};

struct ap_Arg{
	int m_Id;
	const char* m_Value;
	const char* m_Value2;
	size_t m_ValueLen;
	size_t m_Value2Len;
	ap_ArgStatus m_Status;
};

const char* ap_chop(int* argc, const char*** argv);
ap_Arg ap_next(size_t descc, ap_Desc* descv, int* argc, const char*** argv);

inline const char* ap_chop2(int* argc, const char* const** argv){
	return ap_chop(argc, (const char***) argv);
}

inline ap_Arg ap_next2(size_t descc, ap_Desc* descv, int* argc, const char* const** argv){
	return ap_next(descc, descv, argc, (const char***) argv);
}

#endif // _AP_H_
