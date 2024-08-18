#include <stdio.h>

extern const char* g_program;

void help(){
	printf("Taurus help:\n");
	printf(" Usage: %s [files] [options]\n", g_program);
	// TODO: write help
}
