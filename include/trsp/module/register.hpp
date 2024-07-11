#pragma once

#include <set>
#include <string>
#include <string_view>

#include <trsap/trsap.hpp>

#include "../Name.hpp"
#include "../Module.hpp"

namespace trsp{
	enum class ModuleRawArg{
		NAME,
		LANGUAGE,
		MODULE,
		PROJECT,
		EXE,
		LIB
	};

	struct ModuleRaw{
		std::string_view m_Name;
		std::string_view m_Project;
		std::set<std::string> m_Languages;
		std::set<std::string_view> m_Modules;
		ModuleType m_Type = ModuleType::DEFAULT;
		std::string m_Linker;

		bool putArg(const trs::ap::Arg& arg, ModuleRawArg type);
		bool good();

		inline Module getModule(){
			return Module(m_Name, m_Languages, m_Type, m_Linker);
		}

		inline Name getName(){
			return Name(m_Name, NameType::MODULE);
		}

		inline std::string getConfigPath(const std::string& file){
			if(m_Project.empty())
				return "trsp.config/" + file;
			return std::string(m_Project) + "/trsp.config/" + file;
		}
	};
}
