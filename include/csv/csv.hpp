#pragma once

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

		~Row(){
			csv_freeRow(*((csv_Row*) this));
		}
	};

	inline Row fgetrow(FILE* f, char delimeter){
		return 	(Row) csv_fgetrow(f, delimeter);
	}

	inline Row parseRow(const char* line, char delimeter){
		return (Row) csv_parseRow(line, delimeter);
	}

	inline Row parseRow2(char* line, char delimeter){
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

	// TODO: inline Row fgetrow(std::istream& f, char delimeter)
	// TODO: inline std::string to_string(const Row& row, char delimeter = '|')
}
