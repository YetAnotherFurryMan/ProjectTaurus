#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <trsap/trsap.hpp>

#include <trsp/Module.hpp>
#include <trsp/Language.hpp>

enum class Builder{
	DEFAULT, MAKE, NINJA
};

static int buildMake(){
	using namespace trsp;

	std::vector<Module> mods;

	std::ifstream imods("trsp.config/modules.csv");
	if(!imods.good()){
		std::cerr << "Error: Failed to open file: trsp.config/modules.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	while(true){
		auto row = csv::fgetrow(imods, '|');
		if(row.m_Count){
			if(std::filesystem::exists(row.m_Values[0])){
				mods.emplace_back(row);
				if(!mods.back().m_IsValid){
					std::cerr << "Warning: Invalid module." << std::endl;
					mods.pop_back();
				}
			} else{
				std::cerr << "Warning: Module \"" << row.m_Values[0] << "\" does not exist." << std::endl;
			}
		} else break;
	}

	imods.close();

	std::vector<Language> langs;

	std::ifstream ilangs("trsp.config/languages.csv");
	if(!ilangs.good()){
		std::cerr << "Error: Failed to open file: trsp.config/languagess.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	while(true){
		auto row = csv::fgetrow(ilangs, '|');
		if(!row.m_Count)
			break;

		langs.emplace_back(row);
		if(!langs.back().m_IsValid){
			std::cerr << "Warning: Invalid language." << std::endl;
			langs.pop_back();
		}
	}

	ilangs.close();

	std::ofstream makefile("Makefile");
	if(!makefile.good()){
		std::cerr << "Error: Failed to open file: Makefile" << std::endl;
		return -1;
	}

	makefile << "BUILD ?= build" << std::endl;
	makefile << std::endl;

	// Utils
	makefile << "rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))" << std::endl;
	makefile << std::endl;

	makefile << "dirs := ";
	for(auto& mod: mods) // TODO: Search for subdirs
		makefile << "$(BUILD)/" << mod.m_Name << ".dir ";
	makefile << std::endl << std::endl;

	makefile << ".PHONY: all" << std::endl;
	makefile << "all: $(dirs) ";
	for(auto& mod: mods){
		makefile << "$(BUILD)/";
		switch(mod.m_Type){
			case ModuleType::EXE:
				makefile << mod.m_Name;
				break;
			case ModuleType::LIB:
				makefile << "lib" << mod.m_Name << ".a";
				// TODO: When in release create .so targets too
				break;
			default:
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
		}
		makefile << " ";
	}
	makefile << std::endl << std::endl;

	makefile << "clean:" << std::endl;
	makefile << "\t$(RM) -r $(BUILD)" << std::endl;
	makefile << std::endl;

	makefile << "$(dirs):" <<std::endl;
	makefile << "\tmkdir -p $@" << std::endl;
	makefile << std::endl;

	for(auto& mod: mods){
		makefile << mod.m_Name << "_src := $(call rwildcard," << mod.m_Name << ",";
		for(auto& mlang: mod.m_Languages){
			for(auto& lang: langs){
				if(lang.m_Name == mlang || lang.m_Strict == mlang){
					makefile <<" *." << lang.m_Extension;
					break;
				}
			}
		}
		makefile << ")" << std::endl;
		makefile << mod.m_Name << "_bin := $(patsubst " << mod.m_Name << "/%,$(BUILD)/" << mod.m_Name << ".dir/%.o,$(" << mod.m_Name << "_src))" << std::endl;

		makefile << "$(BUILD)/";
		switch(mod.m_Type){
			case ModuleType::EXE:
				makefile << mod.m_Name;
				break;
			case ModuleType::LIB:
				makefile << "lib" << mod.m_Name << ".a";
				break;
			default:
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
		}
		makefile << ": $(" << mod.m_Name << "_bin)" << std::endl;
		switch(mod.m_Type){
			case ModuleType::EXE:
				makefile << "\t$(CXX) -o $@ $^ -std=c++17";
				break;
			case ModuleType::LIB:
				makefile << "\t$(AR) qc $@ $^";
				break;
			default:
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
		}
		makefile << std::endl << std::endl;

		// TODO: Go away with executable, use variable defined like $(strict)_exe ?= $(exe)
		for(auto& mlang: mod.m_Languages){
			for(auto& lang: langs){
				if(lang.m_Name == mlang || lang.m_Strict == mlang){
					makefile << "$(filter %." << lang.m_Extension << ".o,$(" << mod.m_Name << "_bin)): $(BUILD)/" << mod.m_Name << ".dir/%.o:" << mod.m_Name << "/%" << std::endl;
					makefile << "\t" << lang.compile(lang.m_Executable, "$^", "$@", "-Iinclude", (mod.m_Type == ModuleType::EXE?lang.m_ExeFlags:lang.m_LibFlags)) << std::endl;
					break;
				} 
			}
			makefile << std::endl << std::endl;
		}
	}

	makefile.close();
	return 0;
}

static int buildNinja(){
	std::cerr << "Not implemented yet." << std::endl;
	return 0;
}

int build_callback(int argc, const char** argv){
	Builder builder = Builder::DEFAULT;

	trs::ap::Desc descs[] = {
		{ 0, "make", trs::ap::ArgType::FLAG },
		{ 0, "ninja", trs::ap::ArgType::FLAG }
	};

	auto args = trs::ap::getAll(2, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

		if(arg.m_Id < 0){
			// TODO: Error msg
			return -1;
		}

		switch(arg.m_Id){
			case 0:
			{ // Make
				if(builder != Builder::DEFAULT){
					std::cerr << "Error: Builder already setted." << std::endl;
					return -1;
				}

				builder = Builder::MAKE;
			} break;
			case 1:
			{ // Ninja
				if(builder != Builder::DEFAULT){
					std::cerr << "Error: Builder already setted." << std::endl;
					return -1;
				}

				builder = Builder::NINJA;
			} break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
	}

	switch(builder){
		case Builder::DEFAULT:
		case Builder::MAKE:
			return buildMake();
		case Builder::NINJA:
			return buildNinja();
		default:
			std::cerr << "Error: Unreachable." << std::endl;
			return -1;
	}

	return 0;
}
