#pragma once

namespace trsap {
	extern "C" {
		#include "trsap.h"
	}

	enum class ArgType {
		FLAG             = TRSAP_ARG_TYPE_FLAG,
		VALUE            = TRSAP_ARG_TYPE_VALUE,
		VALUE_OPTIONAL   = TRSAP_ARG_TYPE_VALUE_OPTIONAL,
		VALUE2           = TRSAP_ARG_TYPE_VALUE2,
		VALUE2_OPTIONAL	 = TRSAP_ARG_TYPE_VALUE2_OPTIONAL	
	};

	inline const char* chop(size_t* argc, const char*** argv){
		return trsap_chop(argc, argv);
	}
}
