#include <iostream>
#include <fstream>
#include <string_view>

#include <csv/csv.hpp>

namespace trsp{
	struct Project{
		std::string m_Name;
		bool m_IsValid = false;

		Project() = default;

		Project(std::string_view name):
			m_Name{name},
			m_IsValid{true}
		{}

		Project(csv::Row& row){
			if(row.m_Count < 1)
				return;

			*this = Project(row.m_Values[0]);
		}

		inline int write(std::string_view file_path){
			std::ofstream oprojs(file_path.data(), std::ios::app | std::ios::out);
			if(!oprojs.good()){
				std::cerr << "Error: Failed to open file: " << file_path << std::endl
					<< "       Permissions?" << std::endl;
				return -1;
			}

			oprojs << m_Name << std::endl;

			oprojs.close();

			return 0;
		}
	};
}
