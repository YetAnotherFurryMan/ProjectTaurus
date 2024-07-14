#include <iomanip>
#include <iostream>
#include <functional>
#include <unordered_map>

#include <trsap/trsap.hpp>

#include <trsp/entries.hpp>

#define X(NME) int language_##NME##_callback(int, const char**);
X_LANGUAGE_ENTRIES
#undef X

int language_callback(int argc, const char** argv){
	const char* subprog = trs::ap::chop(&argc, &argv);
	if(!subprog){
		std::cerr << "Error: Excepted subprogram." << std::endl;
#define X(NME) std::cerr << "    "#NME << std::endl;
		X_LANGUAGE_ENTRIES
#undef X
		return -1;
	}

	std::unordered_map<std::string_view, std::function<int(int, const char**)>> subprogs;
#define X(NME) subprogs[#NME] = language_##NME##_callback;
	X_LANGUAGE_ENTRIES
#undef X

	if(subprogs.find(subprog) != subprogs.end())
		return subprogs[subprog](argc, argv);
	else{
		std::cerr << "Error: Unknown subprogram: "<< std::quoted(subprog)  << std::endl;
		std::exit(-1);
	}

	return 0;
}
