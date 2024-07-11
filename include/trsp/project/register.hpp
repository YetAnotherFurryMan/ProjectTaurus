#pragma once

#include <set>
#include <string>
#include <string_view>

#include <trsap/trsap.hpp>

#include "../Name.hpp"
#include "../Project.hpp"

namespace trsp{
	enum class ProjectRawArg{
		NAME
	};

	struct ProjectRaw{
		std::string_view m_Name;

		bool putArg(const trs::ap::Arg& arg, ProjectRawArg type);
		bool good();

		inline Project getProject(){
			return Project(m_Name);
		}

		inline Name getName(){
			return Name(m_Name, NameType::PROJECT);
		}
	};
}
