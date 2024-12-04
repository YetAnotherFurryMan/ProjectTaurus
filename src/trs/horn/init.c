#include <trs/horn.h>

assoc g_horn_lispKW = NULL;
assoc g_horn_taurusKW = NULL;

bool horn_init(void){
	g_horn_lispKW = assoc_new(7);
	if(!g_horn_lispKW)
		return true;

	g_horn_taurusKW = assoc_new(1);
	if(!g_horn_taurusKW)
		return true;

	// IDK if this makes sense
	//*assoc_set_horn_Cmd(&g_horn_lispKW, "id") = HORN_CMD_ID;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "set") = HORN_CMD_SET;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "minus") = HORN_CMD_MINUS;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "add") = HORN_CMD_ADD;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "sub") = HORN_CMD_SUB;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "mul") = HORN_CMD_MUL;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "scope") = HORN_CMD_SCOPE;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "var") = HORN_CMD_VAR;

	*assoc_set_horn_Cmd(&g_horn_taurusKW, "var") = HORN_CMD_VAR;
	
	return false;
}

void horn_terminate(void){
	assoc_free(g_horn_lispKW);
	assoc_free(g_horn_taurusKW);

	g_horn_lispKW = NULL;
}
