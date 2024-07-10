#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <fstream>

#include <csv/csv.hpp>

namespace trsp{
	enum class NameType{
		RESERVED = 0,
		MODULE,
		PROJECT,
		LANGUAGE
	};

	struct Name{
		std::string m_Name;
		NameType m_Type;
		bool m_IsValid = false;

		Name() = default;
		Name(std::string_view name, NameType type):
			m_Name{name},
			m_Type{type},
			m_IsValid{true}
		{}

		Name(csv::Row& row){
			if(row.m_Count < 2) 
				return;

			*this = Name(row.m_Values[0], (NameType)atoi(row.m_Values[1]));
		}

		inline int write(std::string_view file_path){
			std::ofstream onames(file_path.data(), std::ios::app | std::ios::out);
			if(!onames.good()){
				std::cerr << "Error: Failed to open file: " << file_path << std::endl
					<< "       Permissions?" << std::endl;
				return -1;
			}

			onames << m_Name << "|" << (int)m_Type << std::endl;;
			
			onames.close();

			return 0;

		}
	};

	inline bool isValidName(std::string_view n){
		for(auto c: n)
			if((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9') && c != '_')
				return false;
		return true;
	}
}
