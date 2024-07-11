#include <trsp/project/register.hpp>

#include <iomanip>

namespace trsp{
	bool ProjectRaw::putArg(const trs::ap::Arg& arg, ProjectRawArg type){
		switch(type){
			case ProjectRawArg::NAME:
			{
				if(!m_Name.empty()){
					std::cerr << "Error: Name already specified (" << std::quoted(m_Name) << ")." << std::endl;
					return false;
				}

				m_Name = std::string_view(arg.m_Value, arg.m_ValueLen);
			} break;  
			
			default:
				return false;
		}

		return true;
	}


	bool ProjectRaw::good(){
		if(m_Name.empty()){
			std::cerr << "Error: No name. Try using -n or --name option." << std::endl;
			return false;
		}

		if(!isValidName(m_Name)){
			std::cerr << "Error: Name " << std::quoted(m_Name) << " is not valid." << std::endl;
			return false;
		}

		std::vector<Name> names;

		std::ifstream inames("trsp.config/names.csv");
		if(!inames.good()){
			std::cerr << "Error: Failed to open file: trsp.config/names.csv" << std::endl 
				<< "       Are you shure you initialized the project? Try calling init subprogram first" << std::endl;
			return false;
		}

		while(true){
			auto row = csv::fgetrow(inames, '|');
			if(row.m_Count){
				names.emplace_back(row);
				if(!names.back().m_IsValid){
					std::cerr << "Warning: Bad name." << std::endl;
					names.pop_back();
				}
			} else break;
		}

		inames.close();

		for(auto& n: names){
			switch(n.m_Type){
				case NameType::RESERVED:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Name " << std::quoted(m_Name) << " is reserved!" << std::endl;
						return false;
					}
					break;
				case NameType::MODULE:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Module " << std::quoted(m_Name) << " already exists, cannot create project with the same name!" << std::endl;
						return false;
					}
					break;
				case NameType::PROJECT:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Project " << std::quoted(m_Name) << " already exists!" << std::endl;
						return false;
					}
					break;
				case NameType::LANGUAGE:
					break;
			}
		}
 
		return true;
	}
}
