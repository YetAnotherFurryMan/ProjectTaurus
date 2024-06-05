#pragma once

namespace trsap {
	extern "C" {
		#include "trsap.h"
	}

	inline const char* chop(size_t* argc, const char*** argv){
		return trsap_chop(argc, argv);
	}
}
