#include <iostream>

#include <trsap/trsap.hpp>

int main(int argc, const char** argv){
	const char* program = trs::ap::chop(&argc, &argv);
	std::cout << "Program: " << program << std::endl;

	trs::ap::Desc descs[] = {
		{ 0, "-1", trs::ap::ArgType::FLAG },
		{ 'a', "aa", trs::ap::ArgType::FLAG },
		{ 'b', "bb", trs::ap::ArgType::VALUE },
		{ 'c', "cc", trs::ap::ArgType::VALUE_OPTIONAL },
		{ 'd', "dd", trs::ap::ArgType::VALUE2 },
		{ 'e', "ee", trs::ap::ArgType::VALUE2_OPTIONAL }
	};

	trs::ap::Arg arg;

	do{
		arg = trs::ap::next(5, descs + 1, &argc, &argv);
		std::cout << "Arg[" << descs[arg.m_Id + 1].m_Long << "]: \"";
		if(arg.m_Value) std::cout << arg.m_Value; else std::cout << "(null)";
		std::cout << "\" \"";
		if(arg.m_Value2) std::cout << arg.m_Value2; else std::cout << "(null)";
		std::cout << "\"" << std::endl;
	} while(arg.m_Status == trs::ap::ArgStatus::OK);

	printf("END[%d]\n", (int)arg.m_Status);
}

