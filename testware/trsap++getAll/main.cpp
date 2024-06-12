#include <iostream>
#include <sstream>
#include <vector>

#include <trsap/trsap.hpp>

struct Case{
	std::vector<const char*> args;
	std::vector<std::string> lines;
	trs::ap::ArgStatus endStatus = trs::ap::ArgStatus::OK;
};

const Case c1 = {
	{"a", "b", "c", "-a", "-bv", "-b=v", "-b:v", "-c", "-cv", "-c=v", "-c:v", "-dv1:v2", "-dv1=v2", "-d:v1:v2", "-d:v1=v2", "-d=v1:v2", "-d=v1=v2", "-ev1:v2", "-ev1=v2", "-e:v1:v2", "-e:v1=v2", "-e=v1:v2", "-e=v1=v2", "-ev", "-e:v", "-e=v", "--", "--aa", "--bb=v", "--bb:v", "--cc", "--cc=v", "--cc:v", "--dd:v1:v2", "--dd:v1=v2", "--dd=v1:v2", "--dd=v1=v2", "--ee:v1:v2", "--ee:v1=v2", "--ee=v1:v2", "--ee=v1=v2", "--ee:v", "--ee=v"},
	{
		"Arg[-1]: \"a\" \"(null)\"",
		"Arg[-1]: \"b\" \"(null)\"",
		"Arg[-1]: \"c\" \"(null)\"",
		"Arg[aa]: \"(null)\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[cc]: \"(null)\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[-1]: \"--\" \"(null)\"",
		"Arg[aa]: \"(null)\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[cc]: \"(null)\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\""
	}
};

int main(){
	trs::ap::Desc descs[] = {
		{ 0, "-1", trs::ap::ArgType::FLAG },
		{ 'a', "aa", trs::ap::ArgType::FLAG },
		{ 'b', "bb", trs::ap::ArgType::VALUE },
		{ 'c', "cc", trs::ap::ArgType::VALUE_OPTIONAL },
		{ 'd', "dd", trs::ap::ArgType::VALUE2 },
		{ 'e', "ee", trs::ap::ArgType::VALUE2_OPTIONAL }
	};

	int status = 0;
	int argc = c1.args.size();
	const char* const* argv = c1.args.data();
	auto args = trs::ap::getAll(5, descs + 1, &argc, &argv);
	size_t passed = 0;

	for(size_t i = 0; i < args.size(); i++){
		auto& arg = args[i];
		auto& line = c1.lines[i];

		std::stringstream ss;

		ss << "Arg[" << descs[arg.m_Id + 1].m_Long << "]: \"";
		if(arg.m_Value) 
			ss << std::string(arg.m_Value).substr(0, arg.m_ValueLen); 
		else 
			ss << "(null)";
		ss << "\" \"";
		if(arg.m_Value2) 
			ss << std::string(arg.m_Value2).substr(0, arg.m_Value2Len); 
		else 
			ss << "(null)";
		ss << "\"";

		if(ss.str() == line)
			passed++;
		else
			std::cerr << ss.str() << std::endl;
		
		if(arg.m_Status != trs::ap::ArgStatus::OK)
			break;
	}

	if(passed == c1.lines.size() && args.back().m_Status == c1.endStatus){
		std::cout << "OK" << std::endl; 
	} else{
		std::cerr << passed << "/" << c1.lines.size() << std::endl;
		std::cout << "FAILED" << std::endl;
	}

	return 0;
}

