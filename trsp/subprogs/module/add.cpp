#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string_view>
#include <set>
#include <iomanip>

#include <trsap/trsap.hpp>
#include <csv/csv.hpp>

#include <trsp/Name.hpp>
#include <trsp/Module.hpp>

int module_add_callback(int argc, const char** argv){
	using namespace trsp;
	
	std::string_view name;
	std::set<std::string> languages;
	std::set<std::string_view> modules;
	ModuleType type = ModuleType::DEFAULT;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
		{ 'l', "language", trs::ap::ArgType::VALUE },
		{ 'm', "module", trs::ap::ArgType::VALUE},
		{ 0, "exe", trs::ap::ArgType::FLAG },
		{ 0, "lib", trs::ap::ArgType::FLAG }
	};

	auto args = trs::ap::getAll(5, descs, &argc, &argv);
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
			{ // Module
				modules.emplace(arg.m_Value, arg.m_ValueLen);
			} break;
			case 3:
			{ // Exe
				if(type != ModuleType::DEFAULT){
					std::cerr << "Error: Type already setted." << std::endl;
					return -1;
				}

				type = ModuleType::EXE;
			} break;
			case 4:
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

	if(name.empty()){
		std::cerr << "Error: No name. Try using -n or --name option." << std::endl;
		return -1;
	}

	if(languages.empty()){
		std::cerr << "Error: No language. Try using -l or --language option." << std::endl;
		return -1;
	}

	if(type == ModuleType::DEFAULT){
		std::cerr << "Warning: Using default module type (exe). Use --exe or --lib." << std::endl;
		type = ModuleType::EXE;
	}

	std::vector<Name> names;

	std::ifstream inames("trsp.config/names.csv");
	if(!inames.good()){
		std::cerr << "Error: Failed to open file: trsp.config/names.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	while(true){
		auto row = csv::fgetrow(inames, '|');
		if(row.m_Count){
			names.emplace_back(row);
			if(!names.back().m_IsValid){
				std::cerr << "Warning: Bad name." << std::endl;
				names.pop_back();
			}
		} else break;
	}

	inames.close();

	std::stringstream ss;
	size_t good_modules = 0;
	size_t good_languages = 0;
	for(auto& n: names){
		switch(n.m_Type){
			case NameType::RESERVED:
				if(name == n.m_Name){
					std::cerr << "Error: Name " << std::quoted(name) << " is reserved!" << std::endl;
					return -1;
				}

				for(auto& lang: languages){
					if(lang == n.m_Name){
						std::cerr << "Error: Language " << std::quoted(lang) << " cannot be registered, name is reserved!" << std::endl;
						return -1;
					}
				}

				for(auto& mod: modules){
					if(mod == n.m_Name){
						std::cerr << "Error: Module " << std::quoted(mod) << " cannot be registered, name is reserved!" << std::endl;
						return -1;
					}
				}
				break;
			case NameType::MODULE:
				if(name == n.m_Name){
					std::cerr << "Error: Module " << std::quoted(name) << " already exists!" << std::endl;
					return -1;
				}

				for(auto& mod: modules){
					if(mod == n.m_Name){
						ss << "$build/lib" << mod << ".a ";
						good_modules++;
						break;
					}
				}
				break;
			case NameType::PROJECT:
				if(name == n.m_Name){
					std::cerr << "Error: Project " << std::quoted(name) << " already exists, cannot create module with the same name!" << std::endl;
					return -1;
				}
				break;
			case NameType::LANGUAGE:
				for(auto& lang: languages){
					if(lang == n.m_Name){
						good_languages++;
						break;
					}
				}
				break;
		}
	}
	
	if(good_modules != modules.size()){
		std::cerr << "Error: Some modules was not registered." << std::endl;
		return -1;
	}

	if(good_languages != languages.size()){
		std::cerr << "Error: Some languages was not registered." << std::endl;
		return -1;
	}

	Module mod(name, languages, type, ss.str());
	Name nam(name, NameType::MODULE);
	
	std::cout << "Name: " << mod.m_Name << std::endl;
	std::cout << "Type: " << mod.m_Type << std::endl;
	std::cout << "Languages:" << std::endl;
	for(auto& lang: mod.m_Languages) std::cout << "  " << lang << std::endl;

	return mod.write("trsp.config/modules.csv") + nam.write("trsp.config");
}

