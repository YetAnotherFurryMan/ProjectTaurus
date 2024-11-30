#include <trs/horn.h>

assoc g_horn_lispKW = NULL;
assoc g_horn_taurusKW = NULL;

bool horn_init(void){
	g_horn_lispKW = assoc_new(4);
	if(!g_horn_lispKW)
		return true;

	// IDK if this makes sense
	//*assoc_set_horn_Cmd(&g_horn_lispKW, "id") = HORN_CMD_ID;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "set") = HORN_CMD_SET;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "add") = HORN_CMD_ADD;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "mul") = HORN_CMD_MUL;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "scope") = HORN_CMD_SCOPE;
	
	return false;
}

void horn_terminate(void){
	assoc_free(g_horn_lispKW);

	g_horn_lispKW = NULL;
}
