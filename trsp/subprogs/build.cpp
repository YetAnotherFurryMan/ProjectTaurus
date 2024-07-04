#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <trsap/trsap.hpp>

#include <trsp/Module.hpp>

enum class Builder{
	DEFAULT, MAKE, NINJA
};

// name|strict|exe|cmd|LibF|EexF|DebugF|lib
struct Language{
	std::string_view m_Name;
	std::string_view m_Strict;
	std::string_view m_Cmd;
	std::string_view m_LibFlags;
	std::string_view m_ExeFlags;
	std::string_view m_DebugFlags;
	std::string_view m_Libraries;
	bool m_IsValid = false;

	Language() = default;
	Language(const std::string_view& name, const std::string_view& strict, const std::string_view& cmd, 
			const std::string_view& libFlags, const std::string_view& exeFlags, const std::string_view& debugFlags,
			const std::string_view& libraries):
		m_Name{name},
		m_Strict{strict},
		m_Cmd{cmd},
		m_LibFlags{libFlags},
		m_ExeFlags{exeFlags},
		m_DebugFlags{debugFlags},
		m_Libraries{libraries},
		m_IsValid{true}
	{}
};

struct LanguageRow{
	Language m_Language;
	csv::Row m_Row;
	
	LanguageRow(csv::Row& row):
		m_Row{row}
	{
		csv::decode(m_Row, '|');

		if(m_Row.m_Count < 7) 
			return;

		m_Language = Language(m_Row.m_Values[0], m_Row.m_Values[1], m_Row.m_Values[2], m_Row.m_Values[3], m_Row.m_Values[4], m_Row.m_Values[5], m_Row.m_Values[6]);
	}
};

static int buildMake(){
	using namespace trsp;

	std::vector<ModuleRow> mods;

	std::ifstream imods("trsp.config/modules.csv");
	if(!imods.good()){
		std::cerr << "Error: Failed to open file: trsp.config/modules.csv" << std::endl 
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return -1;
	}

	while(true){
		auto row = csv::fgetrow(imods, '|');
		if(row.m_Count){
			if(std::filesystem::exists(row.m_Values[0]))
				mods.emplace_back(row);
			else
				std::cerr << "Warning: Module \"" << row.m_Values[0] << "\" does not exist." << std::endl;
			
			if(!mods.back().m_Module.m_IsValid){
				std::cerr << "Warning: Invalid module." << std::endl;
				mods.pop_back();
			}
		} else break;
	}

	imods.close();

	std::vector<LanguageRow> langs;

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
		if(!langs.back().m_Language.m_IsValid){
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

	makefile << "dirs := ";
	for(auto& mod: mods)
		makefile << "$(BUILD)/" << mod.m_Module.m_Name << ".dir ";
	makefile << std::endl << std::endl;

	makefile << ".PHONY: all" << std::endl;
	makefile << "all: $(dirs) ";
	for(auto& mod: mods){
		makefile << "$(BUILD)/";
		switch(mod.m_Module.m_Type){
			case ModuleType::EXE:
				makefile << mod.m_Module.m_Name;
				break;
			case ModuleType::LIB:
				makefile << "lib" << mod.m_Module.m_Name << ".a";
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
		makefile << mod.m_Module.m_Name << "_src := ";
		for(auto& lang: mod.m_Module.m_Languages){
			if(lang == "c")
				makefile << "$(wildcard " << mod.m_Module.m_Name << "/**/*.c " << mod.m_Module.m_Name << "/*.c) ";
			else if(lang == "c++")
				makefile << "$(wildcard " << mod.m_Module.m_Name << "/**/*.cpp " << mod.m_Module.m_Name << "/*.cpp) ";
			else
				std::cerr << "Error: Unknown language." << std::endl;
		}
		makefile << std::endl;
		makefile << mod.m_Module.m_Name << "_bin := $(patsubst " << mod.m_Module.m_Name << "/%,$(BUILD)/" << mod.m_Module.m_Name << ".dir/%.o,$(" << mod.m_Module.m_Name << "_src))" << std::endl;

		makefile << "$(BUILD)/";
		switch(mod.m_Module.m_Type){
			case ModuleType::EXE:
				makefile << mod.m_Module.m_Name;
				break;
			case ModuleType::LIB:
				makefile << "lib" << mod.m_Module.m_Name << ".a";
				break;
			default:
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
		}
		makefile << ": $(" << mod.m_Module.m_Name << "_bin)" << std::endl;
		switch(mod.m_Module.m_Type){
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

		for(auto& lang: mod.m_Module.m_Languages){
			if(lang == "c"){
				makefile << "$(filter %.c.o,$(" << mod.m_Module.m_Name << "_bin)): $(BUILD)/" << mod.m_Module.m_Name << ".dir/%.o:" << mod.m_Module.m_Name << "/%" << std::endl;
				makefile << "\t$(CC) -c -o $@ $^ -I include -std=c17 -Wall -Wextra -Wpedantic";
				if(mod.m_Module.m_Type == ModuleType::LIB)
					makefile << " -fPIC";
			} else if(lang == "c++"){
				makefile << "$(filter %.cpp.o,$(" << mod.m_Module.m_Name << "_bin)): $(BUILD)/" << mod.m_Module.m_Name << ".dir/%.o:" << mod.m_Module.m_Name << "/%" << std::endl;
				makefile << "\t$(CXX) -c -o $@ $^ -I include -std=c++17 -Wall -Wextra -Wpedantic";
				if(mod.m_Module.m_Type == ModuleType::LIB)
					makefile << " -fPIC";
			} else{
				std::cerr << "Error: Unknown language." << std::endl;
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

int build(int argc, const char** argv){
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
