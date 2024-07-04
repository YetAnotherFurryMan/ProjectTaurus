#pragma once

#include <set>
#include <string_view>

#include <csv/csv.hpp>

namespace trsp{
	enum class ModuleType{
		DEFAULT, EXE, LIB
	};

	struct Module{
		std::string_view m_Name;
		std::set<std::string_view> m_Languages;
		ModuleType m_Type;
		bool m_IsValid = false;

		Module() = default;
		Module(const std::string_view& name, std::set<std::string_view>& languages, ModuleType type):
			m_Name{name}, m_Languages{languages}, m_Type{type}, m_IsValid{true}
		{}
	};

	struct ModuleRow{
		Module m_Module;
		csv::Row m_Row;
		csv::Row m_Languages;

		ModuleRow(csv::Row& row):
			m_Row{row},
			m_Languages{(row.m_Count >= 3)?csv::csv_parseRow(m_Row.m_Values[2], ';'):csv::Row(0, nullptr)}
		{
			if(m_Row.m_Count < 3) 
				return;

			std::set<std::string_view> langs;
			for(size_t i = 0; i < m_Languages.m_Count; i++)
				langs.emplace(m_Languages.m_Values[i]);
			m_Module = Module(m_Row.m_Values[0], langs, (ModuleType)std::atoi(m_Row.m_Values[1]));
		}
	};
}
