#pragma once

#include <set>
#include <string>
#include <string_view>
#include <ostream>

#include <csv/csv.hpp>

namespace trsp{
	enum class ModuleType{
		DEFAULT, EXE, LIB
	};

	struct Module{
		std::string m_Name;
		std::set<std::string> m_Languages;
		ModuleType m_Type;
		bool m_IsValid = false;

		Module() = default;
		Module(const std::string_view& name, std::set<std::string>& languages, ModuleType type):
			m_Name{name}, m_Languages{languages}, m_Type{type}, m_IsValid{true}
		{}

		Module(csv::Row& row){
			if(row.m_Count < 3) 
				return;

			csv::Row langs_row = csv::parseRow(row.m_Values[2], ';');

			std::set<std::string> langs;
			for(size_t i = 0; i < langs_row.m_Count; i++)
				langs.emplace(langs_row.m_Values[i]);
			*this = Module(row.m_Values[0], langs, (ModuleType)std::atoi(row.m_Values[1]));
		}

	};
}

inline std::ostream& operator<<(std::ostream& s, const trsp::ModuleType t){
	switch(t){
		case trsp::ModuleType::EXE:
			return s << "Executable";
		case trsp::ModuleType::LIB:
			return s << "Library";
		default:
			return s << "DEFAULT";
	}
}
