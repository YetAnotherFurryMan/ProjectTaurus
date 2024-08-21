#include <string_view>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
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
	std::vector<std::string_view> deps;
	std::string_view flags = "";
};

struct Project{
	std::string_view name;
	std::vector<Module> modules;
};

struct Testware{
	std::string_view project;
	std::string_view name;
	std::vector<std::string_view> deps;
	ModuleLanguage language = ModuleLanguage::C;
	std::string_view flags = "";
};

Project projects[] = {
	{"toollib", {
		{"ap", ModuleType::LIB},
		{"csv", ModuleType::LIB},
		{"cvec", ModuleType::LIB},
		{"carea", ModuleType::LIB}
	}}
};

Module modules[] = {
	{"trs", ModuleType::EXE, {"toollib/libap.a", "toollib/libcvec.a", "toollib/libcarea.a"}}
};

Testware testware[] = {
	{"toollib", "ap", {"toollib/libap.a"}},
	{"toollib", "ap++", {"toollib/libap.a"}, ModuleLanguage::CXX},
	{"toollib", "ap++getAll", {"toollib/libap.a"}, ModuleLanguage::CXX},
	{"toollib", "csv", {"toollib/libcsv.a"}},
	{"toollib", "csv++", {"toollib/libcsv.a"}, ModuleLanguage::CXX},
	{"toollib", "cvec", {"toollib/libcvec.a"}},
	{"toollib", "cassoc", {"toollib/libcvec.a"}},
	{"toollib", "carea", {"toollib/libcarea.a"}}
};

void genMake();
void genNinja();

int main(int argc, const char** argv){
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
					if(build) if(system("make -B testware")) return 1;
				} else if(make == -1){
					if(gen) genNinja();
					if(build) if(system("ninja")) return 1;
				}
			} else{
				genNinja();
				if(system("ninja testware")) return 1;
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
	for(const auto& mod: modules){
		out << " $(BUILD)/" << mod.name << ".dir";
		for(const auto& dir: fs::recursive_directory_iterator(mod.name)){
			if(dir.is_directory()){
				out << " $(BUILD)/" << mod.name << ".dir/" << dir.path().string();
			}
		}
	}
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
	for(const auto& mod: modules){
		out << " $(BUILD)/";
		if(mod.type == ModuleType::EXE)
			out << mod.name;
		else if(mod.type == ModuleType::LIB)
			out << "lib" << mod.name << ".a $(if $(RELEASE),$(BUILD)/" << mod.name << ".so,)";
	}
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
				out << "lib" << mod.name << ".a $(if $(RELEASE),$(BUILD)/" << proj.name << "/" << mod.name << ".so,)";
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
				out << "$(BUILD)/" << proj.name << "/" << mod.name << ": $(bin)";
				for(const auto& dep: mod.deps)
					out << " $(BUILD)/" << dep;
				out << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic -L$(BUILD) $(if $(DEBUG),-ggdb,)" << mod.flags << std::endl;
				out << std::endl;
			} else if(mod.type == ModuleType::LIB){
				out << "libbin += $(bin)" << std::endl;

				// libXYZ.a
				out << "$(BUILD)/" << proj.name << "/lib" << mod.name << ".a: $(bin)" << std::endl;
				out << "\t$(AR) qc $@ $^" << std::endl;
				out << std::endl;
				
				// XYZ.so
				out << "$(BUILD)/" << proj.name << "/" << mod.name << ".so: $(bin)" << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)" << std::endl;
				out << std::endl;

				// TODO: If you want to have .dll, you are welcome to write your code here :-)
			}
		}
	}
	for(const auto& mod: modules){
		out << "bin = $(patsubst " << mod.name << "/%,$(BUILD)/" << mod.name << ".dir/%.o,$(call rwildcard," << mod.name << ",*.c *.cpp))" << std::endl;
		if(mod.type == ModuleType::EXE){
			out << "exebin += $(bin)" << std::endl;
			out << "$(BUILD)/" << mod.name << ": $(bin)";
			for(const auto& dep: mod.deps)
				out << " $(BUILD)/" << dep;
			out << std::endl;
			out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic -L$(BUILD) $(if $(DEBUG),-ggdb,)" << mod.flags << std::endl;
			out << std::endl;
		} else if(mod.type == ModuleType::LIB){
			out << "libbin += $(bin)" << std::endl;

			// libXYZ.a
			out << "$(BUILD)/lib" << mod.name << ".a: $(bin)" << std::endl;
			out << "\t$(AR) qc $@ $^" << std::endl;
			out << std::endl;
			
			// XYZ.so
			out << "$(BUILD)/" << mod.name << ".so: $(bin)" << std::endl;
			out << "\t$(CXX) -o $@ $^ -std=c++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)" << std::endl;
			out << std::endl;

			// TODO: If you want to have .dll, you are welcome to write your code here :-)
		}
	}

	
	out << ".SECONDEXPANSION:" << std::endl;
	out << std::endl;

	// C
	out << "$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	out << "$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=c17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	// C++
	out << "$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	out << "$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
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
		out << "$(BUILD)/" << p << ": " << p << (tw.language == ModuleLanguage::C?".c":".cpp");
		for(const auto& dep: tw.deps)
			out << " $(BUILD)/" << dep;
		out << std::endl;
		if(tw.language == ModuleLanguage::C)
			out << "\t$(CC) -o $@ $^ -std=c17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb " << tw.flags << std::endl;
		else if(tw.language == ModuleLanguage::CXX)
			out << "\t$(CXX) -o $@ $^ -std=c++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb " << tw.flags << std::endl;
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

	// TODO: RELEASE mode
	out << "bin = build" << std::endl;
	out << std::endl;
	out << "rule cc" << std::endl;
	out << "  command = gcc -c $in -o $out -Wall -Wextra -Wpedantic $flags -std=c17 -Iinclude" << std::endl;
	out << std::endl;
	out << "rule cxx" << std::endl;
	out << "  command = g++ -c $in -o $out -Wall -Wextra -Wpedantic $flags -std=c++17 -Iinclude" << std::endl;
	out << std::endl;
	out << "rule link" << std::endl;
	out << "  command = g++ $in -o $out -Wall -Wextra -Wpedantic $flags -std=c++17 -L$bin" << std::endl;
	out << std::endl;
	out << "rule lib" << std::endl;
	out << "  command = ar qc $out $in" << std::endl;
	out << std::endl;
	out << "rule testware" << std::endl;
	out << "  command = gcc $in -o $out -Wall -Wextra -Wpedantic $flags -std=c17 -Iinclude -ggdb" << std::endl;
	out << std::endl;
	out << "rule testwarexx" << std::endl;
	out << "  command = g++ $in -o $out -Wall -Wextra -Wpedantic $flags -std=c++17 -Iinclude -ggdb" << std::endl;
	out << std::endl;

	// Projects
	for(const auto& proj: projects){
		out << "build " << proj.name << ": phony";
		for(const auto& mod: proj.modules){
			out << " $bin/" << proj.name << "/";
			if(mod.type == ModuleType::EXE)
				out << mod.name;
			else if(mod.type == ModuleType::LIB)
				out << "lib" << mod.name << ".a $bin/" << proj.name << "/" << mod.name << ".so";
		}
		out << std::endl;
	}

	// Modules
	for(const auto& proj: projects){
		for(const auto& mod: proj.modules){
			std::stringstream bin;

			std::string p = std::string(proj.name) + "/" + std::string(mod.name);
			for(const auto& e: fs::recursive_directory_iterator(p)){
				if(!e.is_regular_file())
					continue;

				auto erel = e.path().lexically_relative(p);
				if(e.path().extension() == ".c"){
					bin << " $bin/" << p << ".dir/" << erel.c_str() << ".o"; 
					out << "build $bin/" << p << ".dir/" << erel.c_str() << ".o: cc " << p << "/" << erel.c_str() << std::endl;
				} else if(e.path().extension() == ".cpp"){
					bin << " $bin/" << p << ".dir/" << erel.c_str() << ".o"; 
					out << "build $bin/" << p << ".dir/" << erel.c_str() << ".o: cxx " << p << "/" << erel.c_str() << std::endl;
				}

				if(mod.type == ModuleType::EXE)
					out << "  flags = -fPIE" << std::endl;
				else if(mod.type == ModuleType::LIB)
					out << "  flags = -fPIC" << std::endl;
			}

			if(mod.type == ModuleType::EXE){
				out << "build $bin/" << proj.name << "/" << mod.name << ": link" << bin.str();
				for(const auto& dep: mod.deps)
					out << " $bin/" << dep;
				out << std::endl;
				out << "  flags = " << mod.flags << std::endl;
				out << std::endl;
			} else if(mod.type == ModuleType::LIB){
				out << "build $bin/" << proj.name << "/lib" << mod.name << ".a: lib" << bin.str() << std::endl;
				out << std::endl;
				out << "build $bin/" << proj.name << "/" << mod.name << ".so: link" << bin.str() << std::endl;
				out << "  flags = --shared " << mod.flags << std::endl;
				out << std::endl;
			}
		}
	}
	for(const auto& mod: modules){
		std::stringstream bin;

		for(const auto& e: fs::recursive_directory_iterator(mod.name)){
			if(!e.is_regular_file())
				continue;

			auto erel = e.path().lexically_relative(mod.name);
			if(e.path().extension() == ".c"){
				bin << " $bin/" << mod.name << ".dir/" << erel.c_str() << ".o"; 
				out << "build $bin/" << mod.name << ".dir/" << erel.c_str() << ".o: cc " << mod.name << "/" << erel.c_str() << std::endl;
			} else if(e.path().extension() == ".cpp"){
				bin << " $bin/" << mod.name << ".dir/" << erel.c_str() << ".o"; 
				out << "build $bin/" << mod.name << ".dir/" << erel.c_str() << ".o: cxx " << mod.name << "/" << erel.c_str() << std::endl;
			}

			if(mod.type == ModuleType::EXE)
				out << "  flags = -fPIE" << std::endl;
			else if(mod.type == ModuleType::LIB)
				out << "  flags = -fPIC" << std::endl;
		}

		if(mod.type == ModuleType::EXE){
			out << "build $bin/" << mod.name << ": link" << bin.str();
			for(const auto& dep: mod.deps)
				out << " $bin/" << dep;
			out << std::endl;
			out << "  flags = " << mod.flags << std::endl;
			out << std::endl;
		} else if(mod.type == ModuleType::LIB){
			out << "build $bin/lib" << mod.name << ".a: lib" << bin.str() << std::endl;
			out << std::endl;
			out << "build $bin/" << mod.name << ".so: link" << bin.str() << std::endl;
			out << "  flags = --shared " << mod.flags << std::endl;
			out << std::endl;
		}
	}

	// TODO: RELEASE

	// Testware
	out << "build testware: phony ";
	for(const auto& tw: testware)
		out << " $bin/testware/" << tw.project << "/" << tw.name;
	out << std::endl;

	for(const auto& tw: testware){
		out << "build $bin/testware/" << tw.project << "/" << tw.name << ": ";
		if(tw.language == ModuleLanguage::C)
			out << "testware testware/" << tw.project << "/" << tw.name << ".c";
		else if(tw.language == ModuleLanguage::CXX)
			out << "testwarexx testware/" << tw.project << "/" << tw.name << ".cpp";
		for(const auto& dep: tw.deps)
			out << " $bin/" << dep;
		out << std::endl;
	}

	out.close();
	
	std::cout << "DONE" << std::endl;
}
