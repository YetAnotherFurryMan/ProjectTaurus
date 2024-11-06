#pragma once

#include <vector>

namespace toollib::ap {
	extern "C" {
		#include "ap.h"
	}

	enum class ArgType {
		FLAG             = AP_ARG_TYPE_FLAG,
		VALUE            = AP_ARG_TYPE_VALUE,
		VALUE_OPTIONAL   = AP_ARG_TYPE_VALUE_OPTIONAL,
		VALUE2           = AP_ARG_TYPE_VALUE2,
		VALUE2_OPTIONAL	 = AP_ARG_TYPE_VALUE2_OPTIONAL	
	};

	enum class ArgStatus {
		EOI        = AP_ARG_STATUS_EOI,
		OK         = AP_ARG_STATUS_OK,
		ERR_SHORT  = AP_ARG_STATUS_ERR_SHORT,
		ERR_LONG   = AP_ARG_STATUS_ERR_LONG,
		ERR_VALUE  = AP_ARG_STATUS_ERR_VALUE,
		ERR_VALUE2 = AP_ARG_STATUS_ERR_VALUE2
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
		return ap_chop(argc, argv);
	}

	inline Arg next(size_t descc, Desc* descv, int* argc, const char*** argv){
		ap_Arg arg = ap_next(descc, (ap_Desc*)descv, argc, argv);
		return *((Arg*)&arg);
	}

	inline const char* chop(int* argc, const char* const** argv){
		return ap_chop2(argc, argv);
	}

	inline Arg next(size_t descc, Desc* descv, int* argc, const char* const** argv){
		ap_Arg arg = ap_next2(descc, (ap_Desc*)descv, argc, argv);
		return *((Arg*)&arg);
	}

	inline std::vector<Arg> getAll(size_t descc, Desc* descv, int* argc, const char*** argv){
		std::vector<Arg> args;
		ap_Arg arg = ap_next(descc, (ap_Desc*)descv, argc, argv);
		while(arg.m_Status == AP_ARG_STATUS_OK){
			args.push_back(*((Arg*)&arg));
			arg = ap_next(descc, (ap_Desc*)descv, argc, argv);
		}
		if(arg.m_Status != AP_ARG_STATUS_EOI)
			args.push_back(*((Arg*)&arg));
		return args;
	}
	
	inline std::vector<Arg> getAll(size_t descc, Desc* descv, int* argc, const char* const** argv){
		return getAll(descc, descv, argc, (const char***) argv);
	}
}
