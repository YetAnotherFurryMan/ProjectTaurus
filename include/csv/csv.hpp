#pragma once

#include <istream>
#include <string>

#include <cstring>

namespace csv{
	extern "C"{
		#include "csv.h"
	}

	struct Row{
		size_t m_Count;
		char** m_Values;

		Row(size_t count, char** values):
			m_Count{count},
			m_Values{values}
		{}

		Row(csv_Row row):
			m_Count{row.m_Count},
			m_Values{row.m_Values}
		{}

		Row(const Row& row):
			m_Count{row.m_Count}
		{
			m_Values = (char**)malloc(row.m_Count * sizeof(char*));
			for(std::size_t i = 0; i < m_Count; i++){
				size_t len = strlen(row.m_Values[i]);
				m_Values[i] = (char*)malloc(len + 1);
				memcpy(m_Values[i], row.m_Values[i], len);
				m_Values[i][len] = 0;
			}
		}

		~Row(){
			csv_freeRow(*((csv_Row*) this));
		}
	};

	inline Row fgetrow(FILE* f, char delimeter){
		return 	(Row) csv_fgetrow(f, delimeter);
	}

	inline Row fgetrow(std::istream& f, char delimeter){
		std::string s;
		std::getline(f, s);
		if(s.empty())
			return Row(0, 0);
		return (Row) csv_parseRow(s.data(), delimeter);
	}

	inline Row parseRow(const char* line, char delimeter){
		return (Row) csv_parseRow(line, delimeter);
	}

	inline Row parseRow(char* line, char delimeter){
		return (Row) csv_parseRow2(line, delimeter);
	}

	inline void encode(Row& row, char delimeter){
		csv_encode((csv_Row*) &row, delimeter);
	}

	inline void decode(Row& row, char delimeter){
		csv_decode((csv_Row*) &row, delimeter);
	}

	inline char* strrow(const Row& row, char delimeter){
		return csv_strrow(*((csv_Row*) &row), delimeter);
	}

	inline std::string to_string(const Row& row, char delimeter = '|'){
		return std::string(strrow(row, delimeter));
	}
}
