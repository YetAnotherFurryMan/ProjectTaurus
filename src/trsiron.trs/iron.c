#include <trs/iron.h>

#include <trs/error.h>

bool iron_load(horn_Instance* inst, const char* src){
	if(!inst){
		LOGENL(EIDX_HORN_BAD_INSTANCE, NULL);
		return true;
	}

	if(!src){
		LOGENL(EIDX_INTERNAL_BAD_DATA_SRC, "src");
		return true;
	}

	// TODO: Create a header like taurus.h and declare the tokens and state there. Stop using horn.
	// TODO: Clean up horn, it is no longer reponsible for parsing or tokenizing taurus.
	// TODO: Parser
	
	return false;
}
