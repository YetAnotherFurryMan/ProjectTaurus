#ifndef _ERROR_H_
#define _ERROR_H_

// TODO: Make a way to redirect or disable phisical logging

#include <stddef.h>

extern const size_t g_errorMax;
extern const char* g_errorStrings[];

#define EIDX_ERROR_LIST_X                                           \
	X(INTERNAL_BAD_EIDX, "Internal error: Bad error index: %d")     \
	X(INTERNAL_BAD_DATA_SRC, "Internal error: Bad data source: %s") \
	X(FAILED_TO_OPEN, "Failed to open: %s")                         \
	X(UNKNOWN_EXT, "Unknown extension: %s")                         \
	X(OUT_OF_MEM_FILE, "Out of memory while reading file: %s")      \
	X(OUT_OF_MEM, "Out of memory.")                                 \
	X(UNEXPECTED, "Unexpected %s")                                  \
	X(EXPECTED_GOT, "Expected %s but got %s")                       \
	X(EXPECTED_GOT_LEN, "Expected %s but got %*s")                  \
	X(HORN_UNKNOWN_CMD, "Unknown command: %s")                      \
	X(HORN_UNKNOWN_CMD_LEN, "Unknown command: %*s\n")               \
	X(HORN_UNEXPECTED_EXP, "Unexpected expression %s")              \
	X(HORN_EXPECTED, "Expected %s")                                 \
	X(HORN_BAD_INSTANCE, "Internal Error: Bad horn instance.")      \
	X(CG_UNEXPECTED, "Unexpected %s")

typedef enum {
	EIDX_UNKNOWN = 0,
#define X(NME, MSG) EIDX_##NME,
	EIDX_ERROR_LIST_X
#undef X
} error_EIDX;

typedef struct error_Location error_Location;

struct error_Location{
	const char* filename;
	size_t lineno;
	size_t colno;
};

#define error_logErrorNL(IDX, ...) error_logError((error_Location){0}, IDX, __VA_ARGS__)
#define error_logWarningNL(IDX, ...) error_logWarning((error_Location){0}, IDX, __VA_ARGS__)
#define error_logInfoNL(IDX, ...) error_logInfo((error_Location){0}, IDX, __VA_ARGS__)

void error_logError(error_Location loc, error_EIDX errno, ...);
void error_logWarning(error_Location loc, error_EIDX errno, ...);
void error_logInfo(error_Location loc, error_EIDX errno, ...);

#define LOGE(LOC, IDX, ...) error_logError(LOC, IDX, __VA_ARGS__)
#define LOGW(LOC, IDX, ...) error_logWarning(LOC, IDX, __VA_ARGS__)
#define LOGI(LOC, IDX, ...) error_logInfo(LOC, IDX, __VA_ARGS__)

#define LOGENL(IDX, ...) error_logError((error_Location){0}, IDX, __VA_ARGS__)
#define LOGWNL(IDX, ...) error_logWarning((error_Location){0}, IDX, __VA_ARGS__)
#define LOGINL(IDX, ...) error_logInfo((error_Location){0}, IDX, __VA_ARGS__)

#endif // _ERROR_H_
