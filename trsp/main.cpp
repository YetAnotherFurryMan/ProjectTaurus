#include <iostream>
#include <string_view>
#include <unordered_map>
#include <functional>

#include <trsap/trsap.hpp>

void help(const char*);

int init(int, const char**);
int regi(int, const char**);

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
	subprogs["init"] = init;
	subprogs["register"] = regi;

	if(subprogs.find(subprog) != subprogs.end())
		return subprogs[subprog](argc, argv);
	else{
		std::cerr << "Error: Unknown subprogram: \"" << subprog << "\"" << std::endl;
		std::exit(-1);
	}

	return 0;
}
