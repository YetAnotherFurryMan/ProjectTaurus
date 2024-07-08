#pragma once

#include <string>
#include <string_view>

#include <csv/csv.hpp>

namespace trsp{
	// name|strict|ext|exe|cmd|LibF|EexF|DebugF|lib
	struct Language{
		std::string m_Name;
		std::string m_Strict;
		std::string m_Extension;
		std::string m_Executable;
		std::string m_Cmd;
		std::string m_LibFlags;
		std::string m_ExeFlags;
		std::string m_DebugFlags;
		std::string m_Libraries;
		bool m_IsValid = false;
	
		Language() = default;
		Language(const std::string_view& name, const std::string_view& strict, const std::string_view& extension, 
				const std::string_view& executable,	const std::string_view& cmd, const std::string_view& libFlags, 
				const std::string_view& exeFlags, const std::string_view& debugFlags, const std::string_view& libraries):
			m_Name{name},
			m_Strict{strict},
			m_Extension{extension},
			m_Executable{executable},
			m_Cmd{cmd},
			m_LibFlags{libFlags},
			m_ExeFlags{exeFlags},
			m_DebugFlags{debugFlags},
			m_Libraries{libraries},
			m_IsValid{true}
		{}
	
		Language(csv::Row& row)
		{
			csv::decode(row, '|');
	
			if(row.m_Count < 9) 
				return;
	
			*this = Language(row.m_Values[0], row.m_Values[1], row.m_Values[2], row.m_Values[3], row.m_Values[4], row.m_Values[5], row.m_Values[6], row.m_Values[7], row.m_Values[8]);
		}
	
	
		std::string compile(const std::string_view& exe, const std::string_view& in, const std::string_view& out, 
				const std::string_view& includes, const std::string_view& flags){
			//$exe -c $in -o $out $includes -std=c17 -Wall -Wextra -Wpedantic $flags
			std::string cmd(m_Cmd);

#define REP(WHA, LEN) do{ \
	size_t pos = cmd.find("$"#WHA); \
	while(pos != std::string::npos){ \
		cmd.replace(pos, LEN, WHA); \
		pos = cmd.find("$"#WHA, pos + WHA.length()); \
	} \
} while(0)

			REP(includes, 9);
			REP(flags, 6);
			REP(exe, 4);
			REP(out, 4);
			REP(in, 3);

#undef REP

			return cmd;
		}
	};
}