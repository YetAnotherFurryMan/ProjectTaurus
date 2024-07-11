#pragma once

#include <set>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

#include <csv/csv.hpp>

// TODO: Deps
// TODO: Modules
namespace trsp{
	enum class ModuleType{
		DEFAULT, EXE, LIB
	};

	struct Module{
		std::string m_Name;
		std::set<std::string> m_Languages;
		ModuleType m_Type;
		std::string m_Linker;
		bool m_IsValid = false;

		Module() = default;
		Module(std::string_view name, std::set<std::string>& languages, ModuleType type, std::string linker):
			m_Name{name}, m_Languages{languages}, m_Type{type}, m_Linker{linker}, m_IsValid{true}
		{}

		Module(csv::Row& row){
			if(row.m_Count < 4) 
				return;

			csv::Row langs_row = csv::parseRow(row.m_Values[2], ';');

			std::set<std::string> langs;
			for(size_t i = 0; i < langs_row.m_Count; i++)
				langs.emplace(langs_row.m_Values[i]);
			*this = Module(row.m_Values[0], langs, (ModuleType)std::atoi(row.m_Values[1]), row.m_Values[3]);
		}

		inline int write(std::string_view file_path){
			std::ofstream omods(file_path.data(), std::ios::app | std::ios::out);
			if(!omods.good()){
				std::cerr << "Error: Failed to open file: " << file_path << std::endl
					<< "       Permissions?" << std::endl;
				return -1;
			}

			omods << m_Name << "|" << (int)m_Type << "|";
			auto lang_it = m_Languages.begin();
			omods << *lang_it;
			for(lang_it++; lang_it != m_Languages.end(); lang_it++)
				omods << ";" << *lang_it;
			omods << "|" << m_Linker;
			omods << std::endl;

			omods.close();

			return 0;
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
