#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <set>

#include <trsap/trsap.hpp>
#include <csv/csv.hpp>

#include <trsp/Module.hpp>

int module_add_callback(int argc, const char** argv){
	using namespace trsp;
	
	std::string_view name;
	std::set<std::string> languages;
	ModuleType type = ModuleType::DEFAULT;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
		{ 'l', "language", trs::ap::ArgType::VALUE },
		{ 0, "exe", trs::ap::ArgType::FLAG },
		{ 0, "lib", trs::ap::ArgType::FLAG }
	};

	auto args = trs::ap::getAll(4, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

		switch(arg.m_Id){
			case -1:
			case 0:
			{ // Name
				if(!name.empty()){
					std::cerr << "Error: Name already specified (\"" << name << "\")." << std::endl;
					return -1;
				}

				name = std::string_view(arg.m_Value, arg.m_ValueLen);
			} break;
			case 1:
			{ // Language
				languages.emplace(arg.m_Value, arg.m_ValueLen);
			} break;
			case 2:
			{ // Exe
				if(type != ModuleType::DEFAULT){
					std::cerr << "Error: Type already setted." << std::endl;
					return -1;
				}

				type = ModuleType::EXE;
			} break;
			case 3:
			{ // Lib
				if(type != ModuleType::DEFAULT){
					std::cerr << "Error: Type already setted." << std::endl;
					return -1;
				}

				type = ModuleType::LIB;
			} break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
	}

	// TODO: Name black-list
	if(name.empty()){
		std::cerr << "Error: No name. Try using -n or --name option." << std::endl;
		return -1;
	}

	// TODO: Validate languages more
	if(languages.empty()){
		std::cerr << "Error: No language. Try using -l or --language option." << std::endl;
		return -1;
	}

	if(type == ModuleType::DEFAULT){
		std::cerr << "Warning: Using default module type (exe). Use --exe or --lib." << std::endl;
		type = ModuleType::EXE;
	}

	Module mod(name, languages, type);
	
	std::cout << "Name: " << mod.m_Name << std::endl;
	std::cout << "Type: " << mod.m_Type << std::endl;
	std::cout << "Languages:" << std::endl;
	for(auto& lang: mod.m_Languages) std::cout << "  " << lang << std::endl;

	return mod.write("trsp.config/modules.csv");
}

