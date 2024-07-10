#include <trsp/module/register.hpp>

#include <iomanip>

namespace trsp{
	bool ModuleRaw::putArg(const trs::ap::Arg& arg, ModuleRawArg type){
		switch(type){
			case ModuleRawArg::NAME:
			{
				if(!m_Name.empty()){
					std::cerr << "Error: Name already specified (" << std::quoted(m_Name) << ")." << std::endl;
					return false;
				}

				m_Name = std::string_view(arg.m_Value, arg.m_ValueLen);
			} break;  
			case ModuleRawArg::LANGUAGE:
			{
				m_Languages.emplace(arg.m_Value, arg.m_ValueLen);
			} break;  
			case ModuleRawArg::MODULE:
			{
				m_Modules.emplace(arg.m_Value, arg.m_ValueLen);
			} break;  
			case ModuleRawArg::EXE:
			{
				if(m_Type != ModuleType::DEFAULT){
					std::cerr << "Error: Type already setted." << std::endl;
					return false;
				}

				m_Type = ModuleType::EXE;
			} break;  
			case ModuleRawArg::LIB:
			{
				if(m_Type != ModuleType::DEFAULT){
					std::cerr << "Error: Type already setted." << std::endl;
					return false;
				}

				m_Type = ModuleType::LIB;
			} break;  

			default:
				return false;
		}

		return true;
	}


	bool ModuleRaw::good(){
		if(m_Name.empty()){
			std::cerr << "Error: No name. Try using -n or --name option." << std::endl;
			return false;
		}

		if(!isValidName(m_Name)){
			std::cerr << "Error: Name " << std::quoted(m_Name) << " is not valid." << std::endl;
			return false;
		}

		if(m_Languages.empty()){
			std::cerr << "Error: No language. Try using -l or --language option." << std::endl;
			return false;
		}

		if(m_Type == ModuleType::DEFAULT){
			std::cerr << "Warning: Using default module type (exe). Use --exe or --lib." << std::endl;
			m_Type = ModuleType::EXE;
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

		std::stringstream ss;
		size_t good_modules = 0;
		size_t good_languages = 0;
		for(auto& n: names){
			switch(n.m_Type){
				case NameType::RESERVED:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Name " << std::quoted(m_Name) << " is reserved!" << std::endl;
						return false;
					}

					for(auto& lang: m_Languages){
						if(lang == n.m_Name){
							std::cerr << "Error: Language " << std::quoted(lang) << " cannot be registered, name is reserved!" << std::endl;
							return false;
						}
					}

					for(auto& mod: m_Modules){
						if(mod == n.m_Name){
							std::cerr << "Error: Module " << std::quoted(mod) << " cannot be registered, name is reserved!" << std::endl;
							return false;
						}
					}
					break;
				case NameType::MODULE:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Module " << std::quoted(m_Name) << " already exists!" << std::endl;
						return false;
					}

					for(auto& mod: m_Modules){
						if(mod == n.m_Name){
							ss << "$build/lib" << mod << ".a ";
							good_modules++;
							break;
						}
					}
					break;
				case NameType::PROJECT:
					if(m_Name == n.m_Name){
						std::cerr << "Error: Project " << std::quoted(m_Name) << " already exists, cannot create module with the same name!" << std::endl;
						return false;
					}
					break;
				case NameType::LANGUAGE:
					for(auto& lang: m_Languages){
						if(lang == n.m_Name){
							good_languages++;
							break;
						}
					}
					break;
			}
		}
 
		if(good_modules != m_Modules.size()){
			std::cerr << "Error: Some modules was not registered." << std::endl;
			return false;
		}
    
		if(good_languages != m_Languages.size()){
			std::cerr << "Error: Some languages was not registered." << std::endl;
			return false;
		} 

		m_Linker = ss.str();

		return true;
	}
}
