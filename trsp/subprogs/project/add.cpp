#include <iostream>

#include <trsap/trsap.hpp>

#include <trsp/project/register.hpp>

int project_add_callback(int argc, const char** argv){
	using namespace trsp;

	ProjectRaw raw;

	trs::ap::Desc descs[] = {
		{ 'n', "name", trs::ap::ArgType::VALUE },
	};
	
	auto args = trs::ap::getAll(5, descs, &argc, &argv);
	for(auto& arg: args){
		if(arg.m_Status != trs::ap::ArgStatus::OK){
			// TODO: Error msg
			return -1;
		}

		switch(arg.m_Id){
			// Name
			case -1:
			case 0:
				if(!raw.putArg(arg, ProjectRawArg::NAME)) return -1;
				break;
			default:
			{
				std::cerr << "Error: Unreachable." << std::endl;
				return -1;
			}
		}
	}

	if(!raw.good()) return -1;
	
	Project proj = raw.getProject();
	Name name = raw.getName();
	
	std::cout << "Name: " << proj.m_Name << std::endl;

	return proj.write("trsp.config/projects.csv") + name.write("trsp.config/names.csv");
}
