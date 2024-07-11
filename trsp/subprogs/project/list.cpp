#include <iostream>
#include <fstream>
#include <filesystem>

#include <trsp/Project.hpp>

int project_list_callback(int argc, const char** argv){
	using namespace trsp;
	(void) argv;

	if(argc > 0)
		std::cerr << "Warning: Ignored arguments: " << argc << std::endl;

	std::ifstream iprojs("trsp.config/projects.csv");
	if(!iprojs.good()){
		std::cerr << "Error: Failed to open file: trsp.config/projects.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	size_t count = 0;
	while(true){
		auto row = csv::fgetrow(iprojs, '|');
		if(row.m_Count){
			if(std::filesystem::exists(row.m_Values[0])){
				Project proj(row);
				if(!proj.m_IsValid)
					std::cerr << "Warning: Invalid project." << std::endl;
				else
					std::cout << "Info: Project{ " << std::quoted(proj.m_Name) << " }" << std::endl;
			} else{
				std::cerr << "Warning: Project \"" << row.m_Values[0] << "\" does not exist." << std::endl;
			}
		} else break;
		count++;
	}

	iprojs.close();

	std::cerr << "Info: Listed " << count << " projects." << std::endl;

	return 0;
}
