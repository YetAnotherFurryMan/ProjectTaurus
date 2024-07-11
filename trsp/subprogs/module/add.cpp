#include <iostream>

#include <trsap/trsap.hpp>

#include <trsp/module/register.hpp>

int module_add_callback(int argc, const char** argv){
	using namespace trsp;
	
	ModuleRaw raw;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
		{ 'l', "language", trs::ap::ArgType::VALUE },
		{ 'm', "module", trs::ap::ArgType::VALUE },
		{ 'p', "project", trs::ap::ArgType::VALUE },
		{ 0, "exe", trs::ap::ArgType::FLAG },
		{ 0, "lib", trs::ap::ArgType::FLAG }
	};

	auto args = trs::ap::getAll(6, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

		switch(arg.m_Id){
			// Name
			case -1:
			case 0:
				if(!raw.putArg(arg, ModuleRawArg::NAME)) return -1;;
				break;
			// Language
			case 1:
				if(!raw.putArg(arg, ModuleRawArg::LANGUAGE)) return -1;
				break;
			// Module
			case 2:
				if(!raw.putArg(arg, ModuleRawArg::MODULE)) return -1;
				break;
			// Project
			case 3:
				if(!raw.putArg(arg, ModuleRawArg::PROJECT)) return -1;
				break;
			// Exe
			case 4:
				if(!raw.putArg(arg, ModuleRawArg::EXE)) return -1;
				break;
			// Lib
			case 5:
				if(!raw.putArg(arg, ModuleRawArg::LIB)) return -1;
				break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
	}

	if(!raw.good()) return -1;
	
	Module mod = raw.getModule();
	Name name = raw.getName();
	
	std::cout << "Name: " << mod.m_Name << std::endl;
	std::cout << "Type: " << mod.m_Type << std::endl;
	std::cout << "Languages:" << std::endl;
	for(auto& lang: mod.m_Languages) std::cout << "  " << lang << std::endl;

	return mod.write(raw.getConfigPath("modules.csv")) + name.write(raw.getConfigPath("names.csv"));

}

