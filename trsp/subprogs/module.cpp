#include <iostream>
#include <functional>
#include <unordered_map>

#include <trsap/trsap.hpp>

#include <trsp/entries.hpp>

#define X(NME) int module_##NME##_callback(int, const char**);
X_MODULE_ENTRIES
#undef X

int module_callback(int argc, const char** argv){
	const char* subprog = trs::ap::chop(&argc, &argv);
	std::unordered_map<std::string_view, std::function<int(int, const char**)>> subprogs;
#define X(NME) subprogs[#NME] = module_##NME##_callback;
	X_MODULE_ENTRIES
#undef X

	if(subprogs.find(subprog) != subprogs.end())
		return subprogs[subprog](argc, argv);
	else{
		std::cerr << "Error: Unknown subprogram: \"" << subprog << "\"" << std::endl;
		std::exit(-1);
	}

	return 0;
}
