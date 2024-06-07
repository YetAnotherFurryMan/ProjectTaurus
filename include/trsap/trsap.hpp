#pragma once

#include <vector>

namespace trs::ap {
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
		ERR_SHORT  = TRSAP_ARG_STATUS_ERR_SHORT,
		ERR_LONG   = TRSAP_ARG_STATUS_ERR_LONG,
		ERR_VALUE  = TRSAP_ARG_STATUS_ERR_VALUE,
		ERR_VALUE2 = TRSAP_ARG_STATUS_ERR_VALUE2
	};

	struct Desc{
		char m_Short = 0;
		const char* m_Long = 0;
		ArgType m_Type = ArgType::FLAG;
	};

	struct Arg{
		int m_Id = 0;
		const char* m_Value = 0;
		const char* m_Value2 = 0;
		size_t m_ValueLen = 0;
		size_t m_Value2Len = 0;
		ArgStatus m_Status = ArgStatus::EOI;
	};

	inline const char* chop(int* argc, const char*** argv){
		return trsap_chop(argc, argv);
	}

	inline Arg next(size_t descc, Desc* descv, int* argc, const char*** argv){
		trsap_Arg arg = trsap_next(descc, (trsap_Desc*)descv, argc, argv);
		return *((Arg*)&arg);
	}

	inline const char* chop(int* argc, const char* const** argv){
		return trsap_chop2(argc, argv);
	}

	inline Arg next(size_t descc, Desc* descv, int* argc, const char* const** argv){
		trsap_Arg arg = trsap_next2(descc, (trsap_Desc*)descv, argc, argv);
		return *((Arg*)&arg);
	}


	inline std::vector<Arg> getAll(size_t descc, Desc* descv, int* argc, const char*** argv){
		std::vector<Arg> args;
		trsap_Arg arg = trsap_next(descc, (trsap_Desc*)descv, argc, argv);
		while(arg.m_Status == TRSAP_ARG_STATUS_OK){
			args.push_back(*((Arg*)&arg));
			arg = trsap_next(descc, (trsap_Desc*)descv, argc, argv);
		}
		if(arg.m_Status != TRSAP_ARG_STATUS_EOI)
			args.push_back(*((Arg*)&arg));
		return args;
	}
	
	inline std::vector<Arg> getAll(size_t descc, Desc* descv, int* argc, const char* const** argv){
		return getAll(descc, descv, argc, (const char***) argv);
	}
}
