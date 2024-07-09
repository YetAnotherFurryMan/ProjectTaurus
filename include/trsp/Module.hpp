#pragma once

#include <set>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

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
		Module(std::string_view name, std::set<std::string>& languages, ModuleType type):
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

		inline int write(std::string_view file_path){
			std::ifstream imods(file_path.data());
			if(!imods.good()){
			std::cerr << "Error: Failed to open file: " << file_path << std::endl 
					<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
				return -1;
			}

			while(true){
				auto row = csv::fgetrow(imods, '|');
				if(row.m_Count){
					Module mod(row);
					if(mod.m_Name == m_Name){
						std::cerr << "Error: Module with name \"" << m_Name << "\" already exists." << std::endl;
						imods.close();
						std::exit(-1);
					}
				} else break;
			}

			imods.close();

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
