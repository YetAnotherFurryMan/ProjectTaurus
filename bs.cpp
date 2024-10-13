/*
 * Project Taurus build system generator
 * Compile with folowing command:
 *
 * g++ bs.cpp -o bs
 *
 * This program generates Makefile and build.ninja files
 * for building Project Taurus.
 *
 * Usage:
 * 	./bs - Generates build files
 * 	./bs [make|ninja] - Generates Makefile|build.ninja
 * 	./bs test - Generates build.ninja, runs ninja and runs tests
 * 	./bs test [nogen|make|ninja|nobuild]
 * 	    - nogen - Do not generate bildfiles
 * 	    - make - Use make
 * 	    - ninja - Use ninja
 * 	    - nobuild - Do not rebuild the project
 *
 * Project tree:
 * 	build/      - All binares goes here
 * 	      bin/  - Executables
 * 	      lib/  - Libraries
 * 	      obj/  - Object files (.o)
 * 	      test/ - Test executables
 * 	src/        - All source goes here
 * 	include/    - All headers goes here
 * 	docs/       - All documentation goes here
 *
 * 	The project is made of modules and subprojects.
 * 	A module is a source of exacly one product, a library or an executable.
 * 	A subproject is a group of modules. Be aware of that a subproject is not
 * 	a namespace, so a module's name must be unique in entire project to avoid 
 * 	conflicts.
 */

#include <string_view>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <set>

namespace fs = std::filesystem;

enum class DepType{
	INTERNAL, EXTERNAL
};

enum class ModType{
	EXE, LIB
};

enum class Lang{
	C, CXX
};

struct Dep{
	DepType type;
	std::string_view args[2];

	static Dep idep(std::string_view project, std::string_view name){
		Dep d(project, name);
		d.type = DepType::INTERNAL;
		return d;
	}

	static Dep edep(std::string_view cflags = "", std::string_view lflags = ""){
		Dep d(cflags, lflags);
		d.type = DepType::EXTERNAL;
		return d;
	}

	inline constexpr const std::string_view operator[](size_t ix) const{
		return args[ix];
	}

	inline constexpr const std::string_view operator[](const std::string_view ix) const{
		if(ix == "project" || ix == "cflags")
			return args[0];
		else if(ix == "name" || ix == "lflags")
			return args[1];
		return "";
	}

private:
	Dep(std::string_view a, std::string_view b){
		args[0] = a;
		args[1] = b;
	}
};

struct Test{
	std::string_view project;
	std::string_view name;
	Lang lang;
	std::vector<Dep> deps;
};

struct Mod{
	ModType type;
	std::string_view project;
	std::string_view name;
	std::set<Lang> langs;
	std::vector<Dep> deps;
};

Mod mods[] = {
	{ModType::LIB, "toollib", "ap", { Lang::C }, {}},
	{ModType::LIB, "toollib", "csv", { Lang::C }, {}},
	{ModType::LIB, "toollib", "vec", { Lang::C }, {}},
	{ModType::LIB, "toollib", "assoc", { Lang::C }, {}},
	{ModType::LIB, "toollib", "pgm", { Lang::C }, {}},

	{ModType::EXE, "trs", "trsc", { Lang::C }, {}},

	//{"trs", ModuleType::EXE, {"libap.a", "libcvec.a", "libcarea.a"}}
};

Test tests[] = {
	{"toollib", "ap", Lang::C, {
		Dep::idep("toollib", "ap")
	}},
	{"toollib", "ap++", Lang::CXX, {
		Dep::idep("toollib", "ap")
	}},
	{"toollib", "ap++getAll", Lang::CXX, {
		Dep::idep("toollib", "ap")
	}},
	{"toollib", "csv", Lang::C, {
		Dep::idep("toollib", "csv")
	}},
	{"toollib", "csv++", Lang::CXX, {
		Dep::idep("toollib", "csv")
	}},
	{"toollib", "vec", Lang::C, {
		Dep::idep("toollib", "vec")
	}},
	{"toollib", "vec_int", Lang::C, {
		Dep::idep("toollib", "vec")
	}},
	{"toollib", "assoc", Lang::C, {
		Dep::idep("toollib", "assoc")
	}},
	{"toollib", "assoc_int", Lang::C, {
		Dep::idep("toollib", "assoc")
	}},
	{"toollib", "pgm", Lang::C, {
		Dep::idep("toollib", "pgm")
	}},
};

std::string langExt(Lang l){
	switch(l){
		case Lang::C:
			return "c";
		case Lang::CXX:
			return "cpp";
	}

	return "";
}

std::string operator+(std::string_view s1, std::string_view s2){
	return std::string(s1) + std::string(s2);
}

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
					if(build) if(system("make DEBUG=1 -B test")) return 1;
				} else if(make == -1){
					if(gen) genNinja();
					if(build){
						if(system("ninja -t clean")) return 1; 
						if(system("ninja")) return 1; 
					}
				}
			} else{
				genNinja();
				if(system("ninja -t clean")) return 1;
				if(system("ninja test")) return 1;
			}

			size_t all = 0;
			size_t fail = 0;
			for(const auto& test: tests){
				std::string p = "build/test/" + test.name;
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
	out << "getsrc=$(patsubst $(BUILD)/obj/%.o,src/%,$1)" << std::endl;
	out << std::endl;

	out << "dirs := $(BUILD)/obj $(BUILD)/bin $(BUILD)/lib $(BUILD)/test";
	for(const auto& mod: mods){
		std::string pr_mod = (!mod.project.empty()?(mod.project + "/"):"") + mod.name;
		out << " $(BUILD)/obj/" << pr_mod;
		for(const auto& dir: fs::recursive_directory_iterator("src/" + pr_mod)){
			if(dir.is_directory()){
				out << " $(BUILD)/obj/" << pr_mod << "/" << dir.path().string();
			}
		}
	}
	out << std::endl << std::endl;

	// PHONY rules
	out << ".PHONY: clean all test" << std::endl;
	out << std::endl;
	
	// all
	out << "all: $(dirs)";
	for(const auto& mod: mods){
		out << " $(BUILD)/";
		if(mod.type == ModType::EXE)
			out << "bin/" << mod.name;
		else if(mod.type == ModType::LIB)
			out << "lib/lib" << mod.name << ".a $(if $(RELEASE),$(BUILD)/lib/" << mod.name << ".so,)";
	}
	out << std::endl << std::endl;

	// clean
	out << "clean:" << std::endl;
	out << "\t$(RM) -r $(BUILD)" << std::endl;
	out << std::endl;

	// dirs
	out << "$(dirs):" << std::endl;
	out << "\tmkdir -p $@" << std::endl;
	out << std::endl;

	// bins
	out << "exebin := " << std::endl;
	out << "libbin := " << std::endl;
	out << std::endl;

	// modules
	for(const auto& mod: mods){
		std::string pr_mod = (!mod.project.empty()?mod.project + "/":"") + mod.name;
	 	out << "bin = $(patsubst src/%,$(BUILD)/obj/%.o,$(call rwildcard,src/" << pr_mod << ",";
		for(const auto& lang: mod.langs)
			out << " *." << langExt(lang);
		out << "))" << std::endl;
		switch(mod.type){
			case ModType::EXE:
			{
				out << "exebin += $(bin)" << std::endl;
				out << "$(BUILD)/bin/" << mod.name << ": $(bin)";
				std::string lflags = "";
				for(const auto& dep: mod.deps){
					if(dep.type == DepType::INTERNAL){
						out << " $(BUILD)/lib/lib" << dep.args[1] << ".a";
					} else{
						lflags += " " + dep.args[1];
					}
				}
				out << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic -L$(BUILD) $(if $(DEBUG),-ggdb,)" << lflags << std::endl;
				out << std::endl;
			} break;
			case ModType::LIB:
			{
				std::string lflags = "";
				std::string deps = "";
				for(const auto& dep: mod.deps){
					if(dep.type == DepType::INTERNAL){
						deps += " $(BUILD)/lib/lib" + dep.args[1] + ".a";
					} else{
						lflags += " " + dep[1];
					}
				}

				out << "libbin += $(bin)" << std::endl;

				// libXYZ.a
				out << "$(BUILD)/lib/lib" << mod.name << ".a: $(bin)" << deps << std::endl;
				out << "\t$(AR) qc $@ $^" << std::endl;
				out << std::endl;
				
				// XYZ.so
				out << "$(BUILD)/lib/" << mod.name << ".so: $(bin)" << deps << std::endl;
				out << "\t$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -Wpedantic --shared $(if $(DEBUG),-ggdb,)" << lflags << std::endl;
				out << std::endl;

				// TODO: If you want to have .dll, you are welcome to write your code here :-)
			} break;
		}
	}
	
	// Tests
	out << "test: all";
	for(const auto& test: tests){
		out << " $(BUILD)/test/" << test.name;
	}
	out << std::endl << std::endl;

	out << ".SECONDEXPANSION:" << std::endl;
	out << std::endl;

	// C
	out << "$(filter %.c.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	out << "$(filter %.c.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CC) -c $^ -o $@ -std=gnu17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	// C++
	out << "$(filter %.cpp.o, $(libbin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIC $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	out << "$(filter %.cpp.o, $(exebin)): %: $$(call getsrc,%)" << std::endl;
	out << "\t$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -Wpedantic -Iinclude -fPIE $(if $(RELEASE),-O3 -DNODEBUG -DRELEASE,) $(if $(DEBUG),-ggdb -DDEBUG,)" << std::endl;
	out << std::endl;

	// Test src
	for(const auto& test: tests){
		out << "$(BUILD)/test/" << test.name << ": test/" << (!test.project.empty()?test.project + "/":"") << test.name << "." << langExt(test.lang);
		std::string flags = "";
		for(const auto& dep: test.deps){
			if(dep.type == DepType::INTERNAL){
				out << " $(BUILD)/lib/lib" << dep[1] << ".a";
			} else{
				flags += " " + dep[0] + " " + dep[1];
			}
		}
		out << std::endl;
		switch(test.lang){
			case Lang::C:
			{
				out << "\t$(CC) -o $@ $^ -std=gnu17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb " << flags << std::endl;
			} break;
			case Lang::CXX:
			{
				out << "\t$(CXX) -o $@ $^ -std=gnu++17 -Iinclude -Wall -Wextra -Wpedantic -L$(BUILD) -ggdb " << flags << std::endl;
			} break;
		}
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
	out << "  command = gcc -c $in -o $out -Wall -Wextra -Wpedantic $flags -std=gnu17 -Iinclude" << std::endl;
	out << std::endl;
	out << "rule cxx" << std::endl;
	out << "  command = g++ -c $in -o $out -Wall -Wextra -Wpedantic $flags -std=gnu++17 -Iinclude" << std::endl;
	out << std::endl;
	out << "rule link" << std::endl;
	out << "  command = g++ $in -o $out -Wall -Wextra -Wpedantic $flags -std=gnu++17 -L$bin" << std::endl;
	out << std::endl;
	out << "rule lib" << std::endl;
	out << "  command = ar qc $out $in" << std::endl;
	out << std::endl;
	out << "rule testc" << std::endl;
	out << "  command = gcc $in -o $out -Wall -Wextra -Wpedantic $flags -std=gnu17 -Iinclude -ggdb" << std::endl;
	out << std::endl;
	out << "rule testcxx" << std::endl;
	out << "  command = g++ $in -o $out -Wall -Wextra -Wpedantic $flags -std=gnu++17 -Iinclude -ggdb" << std::endl;
	out << std::endl;

	for(const auto& mod: mods){
		std::stringstream bin;

		std::string p = (!mod.project.empty()?mod.project + "/":"") + mod.name;
		for(const auto& e: fs::recursive_directory_iterator("src/" + p)){
			if(!e.is_regular_file())
				continue;

			// TODO: Ignore nonenabled language extensions

			auto erel = e.path().lexically_relative("src/" + p);
			if(e.path().extension() == ".c"){
				bin << " $bin/obj/" << p << "/" << erel.c_str() << ".o"; 
				out << "build $bin/obj/" << p << "/" << erel.c_str() << ".o: cc src/" << p << "/" << erel.c_str() << std::endl;
			} else if(e.path().extension() == ".cpp"){
				bin << " $bin/obj/" << p << "/" << erel.c_str() << ".o"; 
				out << "build $bin/obj/" << p << "/" << erel.c_str() << ".o: cxx src/" << p << "/" << erel.c_str() << std::endl;
			}

			switch(mod.type){
				case ModType::EXE:
				{
					out << "  flags = -fPIE";
				} break;
				case ModType::LIB:
				{
					out << "  flags = -fPIC";
				} break;
			}
			for(const auto& dep: mod.deps){
				if(dep.type == DepType::EXTERNAL){
					out << " " << dep["cflags"];
				}
			}
			out << std::endl;
		}

		std::string lflags = "";
		switch(mod.type){
			case ModType::EXE:
			{
				out << "build $bin/bin/" << mod.name << ": link" << bin.str();
				for(const auto& dep: mod.deps){
					if(dep.type == DepType::INTERNAL)
						out << " $bin/lib/lib" << dep["name"] << ".a";
					else
						lflags += " " + dep["lflags"];
				}
			} break;
			case ModType::LIB:
			{
				lflags = " --shared";
				out << "build $bin/lib/lib" << mod.name << ".a: lib" << bin.str() << std::endl;
				out << std::endl;
				out << "build $bin/lib/" << mod.name << ".so: link" << bin.str();
				for(const auto& dep: mod.deps){
					if(dep.type == DepType::INTERNAL)
						out << " $bin/lib/lib" << dep["name"] << ".a";
					else
						lflags += " " + dep["lflags"];
				}
			} break;
		}
		out << std::endl;
		out << "  flags =" << lflags << std::endl;
		out << std::endl;
	}
	// TODO: RELEASE

	// Test
	out << "build test: phony";
	for(const auto& test: tests)
		out << " $bin/test/" << test.name;
	out << std::endl;

	// Test src
	for(const auto& test: tests){
		out << "build $bin/test/" << test.name << ": ";
		out << "test";
		switch(test.lang){
			case Lang::C:
			{
				out << "c";
			} break;
			case Lang::CXX:
			{
				out << "cxx";
			} break;
		}
		out << " test/" << test.project << "/" << test.name << "." << langExt(test.lang);
		std::string flags = "";
		for(const auto& dep: test.deps){
			if(dep.type == DepType::INTERNAL)
				out << " $bin/lib/lib" << dep["name"] << ".a";
			else
				flags += " " + dep["cflags"] + " " + dep["lflags"];
		}
		out << std::endl;
		if(!flags.empty())
			out << "  flags = " << flags << std::endl;
	}

	out.close();
	
	std::cout << "DONE" << std::endl;
}
