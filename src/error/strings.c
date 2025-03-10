#include <trs/error.h>

const char* g_errorStrings[] = {
	"Unexpected error.\n",
#define X(NME, MSG) MSG "\n",
	EIDX_ERROR_LIST_X
#undef X
};

const size_t g_errorMax = sizeof(g_errorStrings) / sizeof(const char*);
