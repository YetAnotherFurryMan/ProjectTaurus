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

	enum class ArgStatus {
		EOI        = TRSAP_ARG_STATUS_EOI,
		OK         = TRSAP_ARG_STATUS_OK,
		OK_VALUE   = TRSAP_ARG_STATUS_OK_VALUE,
		OK_VALUE2  = TRSAP_ARG_STATUS_OK_VALUE2,
		ERR_VALUE  = TRSAP_ARG_STATUS_ERR_VALUE,
		ERR_VALUE2 = TRSAP_ARG_STATUS_ERR_VALUE2
	};

	struct Desc{
		char m_Short = 0;
		const char* m_Long = 0;
		ArgType m_Type = ArgType::FLAG;
	};

	struct Arg{
		size_t m_Id = 0;
		const char* m_Value = 0;
		const char* m_Value2 = 0;
		ArgStatus m_Status = ArgStatus::EOI;
	};

	inline const char* chop(size_t* argc, const char*** argv){
		return trsap_chop(argc, argv);
	}
}
