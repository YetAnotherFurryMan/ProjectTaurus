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

	std::vector<Module> rows;

	std::ifstream imods("trsp.config/modules.csv");
	if(!imods.good()){
		std::cerr << "Error: Failed to open file: trsp.config/modules.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	while(true){
		auto row = csv::fgetrow(imods, '|');
		if(row.m_Count){
			rows.emplace_back(row);
			if(rows.back().m_Name == mod.m_Name){
				std::cerr << "Error: Module with name \"" << mod.m_Name << "\" already exists." << std::endl;
				imods.close();
				std::exit(-1);
			}
		} else break;
	}

	imods.close();

	std::ofstream omods("trsp.config/modules.csv", std::ios::app | std::ios::out);
	if(!omods.good()){
		std::cerr << "Error: Failed to open file: trsp.config/modules.csv" << std::endl
			<< "       Permissions?" << std::endl;
		return -1;
	}

	omods << mod.m_Name << "|" << (int)mod.m_Type << "|";
	auto lang_it = mod.m_Languages.begin();
	omods << *lang_it;
	for(lang_it++; lang_it != mod.m_Languages.end(); lang_it++)
		omods << ";" << *lang_it;
	omods << std::endl;

	omods.close();

	std::cout << "Name: " << mod.m_Name << std::endl;
	std::cout << "Type: " << (int)mod.m_Type << std::endl;
	std::cout << "Languages:" << std::endl;
	for(auto& lang: mod.m_Languages) std::cout << "  " << lang << std::endl;

	return 0;
}
