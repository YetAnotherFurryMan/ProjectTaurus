#include <iostream>
#include <string_view>
#include <unordered_map>
#include <functional>

#include <trsap/trsap.hpp>

#include <trsp/entries.hpp>

#define X(NME) int NME##_callback(int, const char**);
X_ENTRIES
#undef X

void help(const char*);

int main(int argc, const char** argv){
	const char* program = trs::ap::chop(&argc, &argv);
	const char* subprog = 0; 

	trs::ap::Desc descs[] = {
		{ 'h', "help", trs::ap::ArgType::FLAG }
	};

	auto arg = trs::ap::next(1, descs, &argc, &argv);

	if(arg.m_Status == trs::ap::ArgStatus::OK && arg.m_Id == -1)
		subprog = arg.m_Value;
	else if(arg.m_Status == trs::ap::ArgStatus::OK || arg.m_Status == trs::ap::ArgStatus::EOI)
		help(program);
	else{
		std::cerr << "Error: Unknown argument: \"" << arg.m_Value << "\"" << std::endl;
		std::exit(-1);
	}

	std::unordered_map<std::string_view, std::function<int(int, const char**)>> subprogs;
#define X(NME) subprogs[#NME] = NME##_callback;
	X_ENTRIES
#undef X

	if(subprogs.find(subprog) == subprogs.end()){
		std::cerr << "Error: Unknown subprogram: \"" << subprog << "\"" << std::endl;
		std::exit(-1);
	}

	auto r = subprogs[subprog](argc, argv);
	if(r == 0) std::cout << "OK" << std::endl;
	return r;
}
