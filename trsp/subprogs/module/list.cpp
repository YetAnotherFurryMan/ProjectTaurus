#include <iostream>
#include <fstream>
#include <filesystem>

#include <trsp/Module.hpp>

int module_list_callback(int argc, const char** argv){
	using namespace trsp;
	(void) argv;

	if(argc > 0)
		std::cerr << "Warning: Ignored arguments: " << argc << std::endl;

	std::ifstream imods("trsp.config/modules.csv");
	if(!imods.good()){
		std::cerr << "Error: Failed to open file: trsp.config/modules.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	size_t count = 0;
	while(true){
		auto row = csv::fgetrow(imods, '|');
		if(row.m_Count){
			if(std::filesystem::exists(row.m_Values[0])){
				Module mod(row);
				if(!mod.m_IsValid){
					std::cerr << "Warning: Invalid module." << std::endl;
				} else{
					std::cout << "Info: Module{ " << std::quoted(mod.m_Name) << ":" << mod.m_Type << " ";
					for(auto& lang: mod.m_Languages){
						std::cout << std::quoted(lang) << " ";
					}
					std::cout << "}" << std::endl;
				}
			} else{
				std::cerr << "Warning: Module \"" << row.m_Values[0] << "\" does not exist." << std::endl;
			}
		} else break;
		count++;
	}

	imods.close();

	std::cerr << "Info: Listed " << count << " modules." << std::endl;

	return 0;
}
