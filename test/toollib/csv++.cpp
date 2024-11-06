#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string_view>

#include <toollib/csv.hpp>

namespace tl = toollib;

struct Case{
	const char delimeter;
	const std::string_view line;
	const std::vector<std::vector<std::string_view>> rows;
};

const std::vector<Case> cases = {
	{'|', "a|b|c\n", {
			{"a", "b", "c"}
		}
	}, {';', "a;b;d\n", {
			{"a", "b", "d"}
		}
	}, {'\t', "ala\tma\tbialego\tkota\na\tbialy\tkot\tma\tale\n", {
			{"ala", "ma", "bialego", "kota"},
			{"a", "bialy", "kot", "ma", "ale"}
		}
	}, {'|', "a||c\n", {
			{"a", "", "c"}
		}
	}, {'|', "a|b|\n", {
			{"a", "b", ""}
		}
	},
};

int main(int argc, const char** argv){
	(void) argc;

	std::string tmp_path = argv[0];
	tmp_path += ".csv";

	size_t passed = 0;
	for(size_t casei = 0; casei < cases.size(); casei++){
		char delim = cases[casei].delimeter;
		const auto& rows = cases[casei].rows;
		const std::string_view line = cases[casei].line;
		
		std::ofstream otmp(tmp_path);
		if(!otmp.good()){
			std::cerr << "FAILED(io): Cannot open tmp file " << std::quoted(tmp_path) << std::endl;
			std::cout << "FAILED" << std::endl;
			return -1;
		}

		otmp << line;

		otmp.close();

		std::ifstream file(tmp_path);
		if(!file.good()){
			std::cerr << "FAILED(io): Cannot open file " << std::quoted(tmp_path) << std::endl;
			std::cout << "FAILED" << std::endl;
			return -1;
		}

		size_t passed_rows = 0;
		std::stringstream ss;

		size_t rowi = 0;
		do {
			tl::csv::Row r = tl::csv::fgetrow(file, delim);
			if(!r.m_Values)
				break;

			if(r.m_Count == rows[rowi].size()){
				size_t passed_cols = 0;
				for(size_t i = 0; i < r.m_Count; i++){
					if(r.m_Values[i] == rows[rowi][i])
						passed_cols++;
					else
						std::cerr << "FAILED(case " << casei << "): Column dismatch " << std::quoted(r.m_Values[i]) << " != " << std::quoted(rows[rowi][i]) << std::endl;
				}

				std::string str = tl::csv::to_string(r, delim);
				if(str.length() + ss.str().length() > line.length())
					std::cerr << "FAILED(case " << casei << "): tl::csv::to_string length dismatch: " << std::quoted(ss.str()) << ", " << std::quoted(str);
				else
					ss << str;

				if(passed_cols == r.m_Count)
					passed_rows++;
			} else {
				std::cerr << "FAILED(case " << casei << "): Column count dismatch " << r.m_Count << " != " << rows[rowi].size() << std::endl;
				break;
			}

			rowi++;
		} while(true);

		file.close();

		if(ss.str() == line){
			if(passed_rows == cases[casei].rows.size())
				passed++;
		} else{
			std::cerr << "FAILED(case " << casei << "): csv_strrow dismatch " << std::quoted(ss.str()) << " != " <<  std::quoted(line);
		}
	}

	if(passed != cases.size())
		std::cout << "FAILED" << std::endl;
	else
		std::cout << "OK" << std::endl;

	return 0;
}
