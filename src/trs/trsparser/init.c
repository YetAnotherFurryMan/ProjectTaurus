#include <trs/parser.h>

assoc g_trs_lexLispKW = NULL;
assoc g_trs_lexTaurusKW = NULL;

bool trs_lexInit(void){
	g_trs_lexLispKW = assoc_new(4);
	if(!g_trs_lexLispKW)
		return true;

	*assoc_set_trs_IRCmd(&g_trs_lexLispKW, "load") = TRS_IRCMD_LOAD;
	*assoc_set_trs_IRCmd(&g_trs_lexLispKW, "set") = TRS_IRCMD_SET;
	*assoc_set_trs_IRCmd(&g_trs_lexLispKW, "add") = TRS_IRCMD_ADD;
	*assoc_set_trs_IRCmd(&g_trs_lexLispKW, "mul") = TRS_IRCMD_MUL;
	
	return false;
}

void trs_lexTerminate(void){
	assoc_free(g_trs_lexLispKW);

	g_trs_lexLispKW = NULL;
}
