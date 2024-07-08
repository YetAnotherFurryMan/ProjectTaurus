#include <iostream>

int module_init_callback(int argc, const char** argv){
	(void) argc;
	(void) argv;
	std::cerr << "Error: Unimplemented yet!" << std::endl;
	return 0;
}
