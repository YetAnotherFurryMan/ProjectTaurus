#include <iostream>
#include <filesystem>
#include <fstream>

#include <trsap/trsap.hpp>

#include <trsp/project/register.hpp>

static const char* names_csv = 
"build|0\n"
"Makefile|0\n";

static bool createFile(const std::string& p, const char* cnt = 0){
	std::ofstream f(p);
	if(!f.good()){
		std::cerr << "Error: Failed to create file: " << p << std::endl;
		return false;
	}
	if(cnt) f << cnt;
	f.close();
	return true;
}

// TODO: templates
int project_init_callback(int argc, const char** argv){
	using namespace trsp;

	ProjectRaw raw;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
	};
	
	auto args = trs::ap::getAll(5, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

		switch(arg.m_Id){
			// Name
			case -1:
			case 0:
				if(!raw.putArg(arg, ProjectRawArg::NAME)) return -1;
				break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
	}

	if(!raw.good()) return -1;

	if(std::filesystem::exists(raw.m_Name)){
		std::cerr << "Warning: Project " << raw.m_Name << " will be registered but not initialized." << std::endl;
		std::cerr << " Directory exists." << std::endl;

		if(!std::filesystem::is_directory(raw.m_Name)){
			std::cerr << "Error: " << std::quoted(raw.m_Name) << " is not a directory! Project cannot be initialized." << std::endl;
			return -1;
		}
	} else{
		if(!std::filesystem::create_directory(raw.m_Name)){
			std::cerr << "Error: Failed to create directory: " << raw.m_Name << std::endl;
			return -1;
		}

		std::string name(raw.m_Name);
		if(!std::filesystem::create_directory(name + "/include")){
			std::cerr << "Error: Failed to create directory: " << name << "/include" << std::endl;
			return -1;
		}

		if(!std::filesystem::create_directory(name + "/trsp.config")){
			std::cerr << "Error: Failed to create directory: " << name << "/trsp.config" << std::endl;
			return -1;
		}

		if(!createFile(name + "/trsp.config/modules.csv"))
			return -1;

		if(!createFile(name + "/trsp.config/names.csv", names_csv))
			return -1;
	}

	
	Project proj = raw.getProject();
	Name name = raw.getName();
	
	std::cout << "Name: " << proj.m_Name << std::endl;

	return proj.write("trsp.config/projects.csv") + name.write("trsp.config/names.csv");
}
