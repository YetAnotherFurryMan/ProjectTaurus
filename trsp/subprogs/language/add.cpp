#include <iostream>
#include <iomanip>
#include <string_view>

#include <trsap/trsap.hpp>

#include <trsp/Name.hpp>
#include <trsp/Language.hpp>

/*
 * Before you read this function:
 * Yeah, I know, I know!
 * Macros aren't the best way of writing generic code in C++...
 * BUT I like them. They are simple, unreadable, fast, generic, criptic, etc.
 * I realy do not see any reason wy shouldn't I use them!
 * Also if I would write a function or something that would do the stuf, that was unneded control-flow.
 * But what about inline functions?
 * What about consexpr?
 * What about this, what about that?
 * I DO NOT CARE!
 * And yes I wrote this only for piss you, break your eyes wit my english level, take your time and maybe make you laugh.
 * P.S. Also the friend of mine wannted me to write more commands into my codebase, here you are!
 */

int language_add_callback(int argc, const char** argv){
	using namespace trsp;

	std::string_view name;
	std::string_view strict;
	std::string_view ext;
	std::string_view cmd;
	std::string_view exe;
	std::string_view exeFlags;
	std::string_view libFlags;
	std::string_view debugFlags;
	std::string_view libs;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
		{ 's', "strict", trs::ap::ArgType::VALUE },
		{ 'x', "extension", trs::ap::ArgType::VALUE },
		{ 'c', "cmd", trs::ap::ArgType::VALUE },
		{ 'e', "exe", trs::ap::ArgType::VALUE },
		{ 'l', "libs", trs::ap::ArgType::VALUE },
		{ 'f', "flag", trs::ap::ArgType::VALUE2 }
	};

	auto args = trs::ap::getAll(7, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

#define XCASE(VAR, NME) {                                                                            \
	if(!VAR.empty()){                                                                                \
		std::cerr << "Error: " #NME " already specified (" << std::quoted(VAR) << ")." << std::endl; \
		return -1;                                                                                   \
	}                                                                                                \
	VAR = std::string_view(arg.m_Value, arg.m_ValueLen);                                             \
}

		switch(arg.m_Id){
			// Name
			case -1:
			case 0: XCASE(name, Name) break;
			// Strict
			case 1: XCASE(strict, Strict name) break;
			// Ext
			case 2: XCASE(ext, Extension) break;
			// Cmd
			case 3: XCASE(cmd, Cmd) break;
			// Exe
			case 4: XCASE(exe, Executable) break;
			// Libs
			case 5: XCASE(libs, Libraries) break;
			// Flag
			case 6: 
			{
				std::string_view flag(arg.m_Value, arg.m_ValueLen);
#define XIF(VAR, NME)                                                                                             \
if(flag == #VAR){                                                                                                 \
	if(!VAR##Flags.empty()){                                                                                      \
		std::cerr << "Error: " #NME " flag  already specified (" << std::quoted(VAR##Flags) << ")." << std::endl; \
		return -1;                                                                                                \
	}                                                                                                             \
	VAR##Flags = std::string_view(arg.m_Value2, arg.m_Value2Len);                                                 \
}
				XIF(exe, Executable)
				else XIF(lib, Library)
				else XIF(debug, Debug)
				else{
					std::cerr << "Error: Unknown flag name: " << std::quoted(flag) << std::endl;
					return -1;
				}
#undef XIF
			} break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
#undef XCASE
	}

#define PRESENT(VAR, NME)                                           \
if(VAR.empty()){                                                    \
	std::cerr << "Error: " #NME " was not specified." << std::endl; \
	return -1;                                                      \
}

	PRESENT(name, Name)
	PRESENT(ext, Extension)
	PRESENT(exe, Executable)
	PRESENT(cmd, Command)

#undef PRESENT

	if(strict.empty()){
		std::cerr << "Warning: Strict name was specified, using " << std::quoted(name) << "." << std::endl;
		strict = name;		
	}

	if(!isValidName(strict)){
		std::cerr << "Error: Strict name does not meet tre rule [A-Za-z0-9_]+" << std::endl;
		return -1;
	}

	if(debugFlags.empty()){
		std::cerr << "Warning: Using default debug flag (-ggdb)." << std::endl;
		debugFlags = "-ggdb";
	}

	std::ifstream inames("trsp.config/names.csv");
	if(!inames.good()){
		std::cerr << "Error: Failed to open file: trsp.config/names.csv" << std::endl
			<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
		return false;
	}

	while(true){
		auto row = csv::fgetrow(inames, '|');
		if(!row.m_Count)
			break;

		Name n(row);
		if(!n.m_IsValid){
			std::cerr << "Warning: Invalid name." << std::endl;
		} else if(n.m_Type == NameType::LANGUAGE){
			if(n.m_Name == name){
				std::cerr << "Error: Language " << std::quoted(name) << " cannot be registered." << std::endl;
				return -1;
			} else if(n.m_Name == strict){
				std::cerr << "Error: Language " << std::quoted(name) << " cannot be registered, strict name not unique." << std::endl;
				return -1;
			}
		}
	}

	inames.close();

	Language lang(name, strict, ext, exe, cmd, libFlags, exeFlags, debugFlags, libs);
	Name nam(name, NameType::LANGUAGE);

	std::cout << "Name: " << name << std::endl;
	std::cout << "Strict: " << strict << std::endl;
	std::cout << "Extension: " << ext << std::endl;
	std::cout << "Executable: " << exe << std::endl;
	std::cout << "Cmd: " << cmd << std::endl;
	std::cout << "Executable flags: " << exeFlags << std::endl;
	std::cout << "Library flags: " << libFlags << std::endl;
	std::cout << "Libraries: " << libs << std::endl;
	std::cout << "Debug flags: " << debugFlags << std::endl;

	return lang.write("trsp.config/languages.csv") + nam.write("trsp.config/names.csv");
}
