#include <trs/horn.h>

assoc g_horn_lispKW = NULL;
assoc g_horn_taurusKW = NULL;

bool horn_init(void){
	g_horn_lispKW = assoc_new(4);
	if(!g_horn_lispKW)
		return true;

	*assoc_set_trs_IRCmd(&g_horn_lispKW, "load") = TRS_IRCMD_LOAD;
	*assoc_set_trs_IRCmd(&g_horn_lispKW, "set") = TRS_IRCMD_SET;
	*assoc_set_trs_IRCmd(&g_horn_lispKW, "add") = TRS_IRCMD_ADD;
	*assoc_set_trs_IRCmd(&g_horn_lispKW, "mul") = TRS_IRCMD_MUL;
	
	return false;
}

void horn_terminate(void){
	assoc_free(g_horn_lispKW);

	g_horn_lispKW = NULL;
}
