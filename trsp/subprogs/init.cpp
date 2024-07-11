#include <iostream>
#include <fstream>
#include <filesystem>

static bool createFile(const char* p, const char* cnt = 0){
	std::ofstream f(p);
	if(!f.good()){
		std::cerr << "Error: Failed to create file: " << p << std::endl;
		return false;
	}
	if(cnt) f << cnt;
	f.close();
	return true;
}

// name|strict|ext|exe|cmd|LibF|EexF|DebugF|lib
static const char* languages_csv = 
"c|c|c|gcc|$exe -c $in -o $out $includes -std=c17 -Wall -Wextra -Wpedantic $flags|-fPIC|-fPIE|-ggdb|\n"
"c++|cpp|cpp|g++|$exe -c $in -o $out $includes -std=c++17 -Wall -Wextra -Wpedantic $flags|-fPIC|-fPIE|-ggdb|\n";

// name|type
static const char* names_csv = 
"build|0\n"
"Makefile|0\n"
"c|3\n"
"cpp|3\n"
"c++|3\n";

int init_callback(int argc, const char** argv){
	(void)argv;

	if(argc > 0){
		std::cerr << "Error: init does not expect any arguments, got " << argc << "." << std::endl;
		return -1;
	}

	if(std::filesystem::exists("trsp.config")){
		std::cerr << "Error: Project already exists." << std::endl;
		std::cerr << "Note: If you want to recreate the project, or it is error, please delete trsp.config directory." << std::endl;
		return -1;
	}

	if(!std::filesystem::create_directory("trsp.config")){
		std::cerr << "Error: Failed to create directory: trsp.config" << std::endl;
		return -1;
	}

	if(!std::filesystem::exists("include"))
		if(!std::filesystem::create_directory("include")){
			std::cerr << "Error: Failed to create directory: include" << std::endl;
			return -1;
		}

	if(!createFile("trsp.config/modules.csv"))
		return -1;

	if(!createFile("trsp.config/projects.csv"))
		return -1;

	if(!createFile("trsp.config/languages.csv", languages_csv))
		return -1;

	if(!createFile("trsp.config/names.csv", names_csv))
		return -1;
	
	return 0;
}
