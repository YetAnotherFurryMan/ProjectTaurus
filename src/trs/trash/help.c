#include <stdio.h>

#include <trs/state.h>

void help(State* s){
	printf("Taurus help:\n");
	printf(" Usage: %s [files] [options]\n", s->m_Program);
	// TODO: write help
}
