#include <iostream>
#include <fstream>
#include <iomanip>

#include <trsp/Language.hpp>

int language_list_callback(int argc, const char** argv){
	using namespace trsp;
	(void) argv;

	if(argc > 0)
		std::cerr << "Warning: Ignored arguments: " << argc << std::endl;

	std::ifstream ilangs("trsp.config/languages.csv");
	if(!ilangs.good()){
		std::cerr << "Error: Failed to open file: trsp.config/languages.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	size_t count = 0;
	while(true){
		auto row = csv::fgetrow(ilangs, '|');
		if(row.m_Count){
			Language lang(row);
			if(!lang.m_IsValid){
				std::cerr << "Warning: Invalid language." << std::endl;
			} else{
				std::cout << "Info: Language{" << std::endl 
					<< "    " << std::quoted(lang.m_Name) << " (aka " << std::quoted(lang.m_Strict) << "): " << std::endl 
					<< "    " << std::quoted(lang.m_Executable) << " (" << std::quoted(lang.m_Cmd) << ")" << std::endl
					<< "    exe: " << lang.m_ExeFlags << std::endl
					<< "    lib: " << lang.m_LibFlags << std::endl
					<< "    debug: " << lang.m_DebugFlags << std::endl
					<< "}" << std::endl;
			}
		} else break;
		count++;
	}

	ilangs.close();

	std::cerr << "Info: Listed " << count << " languages." << std::endl;

	return 0;
}
