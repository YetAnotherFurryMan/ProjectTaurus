#include <string_view>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <set>

namespace fs = std::filesystem;

enum class ModuleType{
	EXE, LIB
};

enum class ModuleLanguage{
	C, CXX
};

struct Module{
	std::string_view name;
	ModuleType type = ModuleType::EXE;
	std::string_view flags = "";
};

struct Project{
	std::string_view name;
	std::vector<Module> modules;
};

struct Testware{
	std::string_view project;
	std::string_view name;
	std::string_view flags;
	ModuleLanguage language = ModuleLanguage::C;
};

Project projects[] = {
	{"toollib", {
		{"ap", ModuleType::LIB},
		{"csv", ModuleType::LIB}
	}}
};

Module modules[] = {};

Testware testware[] = {
	{"toollib", "ap", "-l:toollib/libap.a"},
	{"toollib", "ap++", "-l:toollib/libap.a", ModuleLanguage::CXX},
	{"toollib", "ap++getAll", "-l:toollib/libap.a", ModuleLanguage::CXX},
	{"toollib", "csv", "-l:toollib/libcsv.a"},
	{"toollib", "csv++", "-l:toollib/libcsv.a", ModuleLanguage::CXX}
};

void genMake();
void genNinja();

int main(int argc, const char** argv){

	// TODO: ./buildsystem test

	if(argc > 1){
		const char* arg = argv[1];
		if(strcmp(arg, "make") == 0){
			genMake();
		} else if(strcmp(arg, "ninja") == 0){
			genNinja();
		} else if(strcmp(arg, "test") == 0){
			if(argc > 2){
				int make = 1;
				bool gen = true;
				bool build = true;
				for(size_t i = 2; argv[i]; i++){
					arg = argv[i];
					if(strcmp(arg, "nogen") == 0)
						gen = false;
					else if(strcmp(arg, "make") == 0)
						make = 1;
					else if(strcmp(arg, "ninja") == 0)
						make = -1;
					else if(strcmp(arg, "nobuild") == 0)
						build = false;
				}

				if(!build) gen = false;

				if(make == 1){
					if(gen) genMake();
					if(build) system("make -B testware");
				} else if(make == -1){
					if(gen) genNinja();
					if(build) system("ninja");
				}
			} else{
				// Make is the default for now but:
				// TODO: Change to ninja
				genMake();
				system("make -B testware");
			}

			size_t all = 0;
			size_t fail = 0;
			for(const auto& tw: testware){
				std::string p = "build/testware/";
				if(!tw.project.empty()) p += std::string(tw.project) + "/";
				p += std::string(tw.name);
				std::cout << p << ": " << std::flush;
				if(system(p.c_str())) fail++;
				all++;
			}
			std::cout << (all - fail) << "/" << all << std::endl;
		} else{
			std::cerr << "Error: Unknown argument: " << arg << std::endl;
			return 1;
		}
	} else{
		genMake();
		genNinja();
	}

	return 0;
}

void genMake(){
	std::cout << "Generating Makefile...";

	std::ofstream out("Makefile");
	if(!out.good()){
		std::cout << std::endl;
		std::cerr << "ERROR: Failed to create file: Makefile" << std::endl;
		return;
	}

	out << "BUILD ?= build" << std::endl;
	out << std::endl;
	out << "rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))" << std::endl;
	out << "getsrc=$(patsubst $(BUILD)/%,%,$(patsubst $(word 1,$(subst .dir,.dir ,$1))/%.o,$(word 1,$(subst .dir, ,$1))/%,$1))" << std::endl;
	out << std::endl;

	out << "dirs :=";
	for(const auto& proj: projects){
		out << " $(BUILD)/" << proj.name;
		for(const auto& mod: proj.modules){
			out << " $(BUILD)/" << proj.name << "/" << mod.name << ".dir";
			for(const auto& dir: fs::recursive_directory_iterator(std::string(proj.name) + "/" + std::string(mod.name))){
				if(dir.is_directory()){
					out << " $(BUILD)/" << proj.name << "/" << mod.name << ".dir/" << dir.path().string();
				}
			}
		}
	}
	// TODO: modules[]
	out << std::endl << std::endl;

	std::set<std::string> testware_dirs_set;
	for(const auto& tw: testware){
		std::string p = "$(BUILD)/testware";
		if(!tw.project.empty()) p += "/" + std::string(tw.project);
		testware_dirs_set.emplace(p);
	}
	out << "testware_dirs :=";
	for(const auto& p: testware_dirs_set){
		out << " " << p;
	}
	out << std::endl << std::endl;

	// all
	out << ".PHONY: all" << std::endl;
	out << "all: $(dirs)";
	for(const auto& proj: projects){
		out << " " << proj.name;
	}
	// TODO: modules[]
	out << std::endl << std::endl;

	// projects
	for(const auto& proj: projects){
		out << ".PHONY: " << proj.name << std::endl;
		out << proj.name << ": $(filter $(BUILD)/" << proj.name << "/%, $(dirs))";
		for(const auto& mod: proj.modules){
			out << " $(BUILD)/" << proj.name << "/";
			if(mod.type == ModuleType::EXE)
				out << mod.name;
			else if(mod.type == ModuleType::LIB)
				out << "lib" << mod.name << ".a"; // TODO: RELEASE case
		}
		out << std::endl << std::endl;
	}

	// clean
	out << "clean:" << std::endl;
	out << "\t$(RM) -r $(BUILD)" << std::endl;
	out << std::endl;

	// dirs
	out << "$(dirs) $(testware_dirs):" << std::endl;
	out << "\tmkdir -p $@" << std::endl;
	out << std::endl;

	// bins
	out << "exebin := " << std::endl;
	out << "libbin := " << std::endl;
	out << std::endl;

	// modules
	for(const auto& proj: projects){
		for(const auto& mod: proj.modules){
			std::string p = std::string(proj.name) + "/" + std::string(mod.name);
			out << "bin = $(patsubst " << p << "/%,$(BUILD)/" << p << ".dir/%.o,$(call rwildcard," << p << ",*.c *.cpp))" << std::endl;
			if(mod.type == ModuleType::EXE){
				out << "exebin += $(bin)" << std::endl;
				out << "$(BUILD)/" << proj.name << "/" << mod.name << ": $(bin)" << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic -L$(BUILD) " << mod.flags << std::endl;
				out << std::endl;
			} else if(mod.type == ModuleType::LIB){
				out << "libbin += $(bin)" << std::endl;

				// libXYZ.a
				out << "$(BUILD)/" << proj.name << "/lib" << mod.name << ".a: $(bin)" << std::endl;
				out << "\t$(AR) qc $@ $^" << std::endl;
				out << std::endl;
				
				// XYZ.so
				out << "$(BUILD)/" << proj.name << "/" << mod.name << ".so: $(bin)" << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared" << std::endl;
				out << std::endl;

				// TODO: If you want to have .dll, you are welcome to write your code here :-)
			}
		}
	}
	// TODO: modules[]
	
	out << ".SECONDEXPANSION:" << std::endl;
	out << std::endl;

	// C
	out << "$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIC" << std::endl;
	out << std::endl;

	out << "$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIE" << std::endl;
	out << std::endl;

	// C++
	out << "$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC" << std::endl;
	out << std::endl;

	out << "$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE" << std::endl;
	out << std::endl;

	// Testware
	out << ".PHONY: testware" << std::endl;
	out << "testware: all $(testware_dirs)";
	for(const auto& tw: testware){
		out << " $(BUILD)/testware/";
		if(!tw.project.empty()) out << tw.project << "/";
		out << tw.name;
	}
	out << std::endl << std::endl;

	for(const auto& tw: testware){
		std::string p = "testware/" + (tw.project.empty()?"":std::string(tw.project) + "/") + std::string(tw.name);
		out << "$(BUILD)/" << p << ": " << p << (tw.language == ModuleLanguage::C?".c":".cpp") << std::endl;
		if(tw.language == ModuleLanguage::C)
			out << "\t$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) " << tw.flags << std::endl;
		else if(tw.language == ModuleLanguage::CXX)
			out << "\t$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) " << tw.flags << std::endl;
		out << std::endl;
	}

	out.close();

	std::cout << "DONE" << std::endl;
}

void genNinja(){
	std::cout << "Generating build.ninja ...";
	
	std::ofstream out("build.ninja");
	if(!out.good()){
		std::cout << std::endl;
		std::cerr << "ERROR: Failed to create file: build.ninja" << std::endl;
		return;
	}

	out.close();
	
	std::cout << "DONE" << std::endl;
}
