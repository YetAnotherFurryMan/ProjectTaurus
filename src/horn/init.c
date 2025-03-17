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

	*assoc_set_horn_Cmd(&g_horn_lispKW, "get") = HORN_CMD_GET;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "set") = HORN_CMD_SET;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "minus") = HORN_CMD_MINUS;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "add") = HORN_CMD_ADD;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "sub") = HORN_CMD_SUB;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "mul") = HORN_CMD_MUL;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "div") = HORN_CMD_DIV;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "mod") = HORN_CMD_MOD;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lnot") = HORN_CMD_LNOT;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "leq") = HORN_CMD_LEQ;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lneq") = HORN_CMD_LNEQ;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lgt") = HORN_CMD_LGT;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "llt") = HORN_CMD_LLT;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lgtq") = HORN_CMD_LGTQ;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lltq") = HORN_CMD_LLTQ;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "land") = HORN_CMD_LAND;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "lor") = HORN_CMD_LOR;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "bnot") = HORN_CMD_BNOT;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "band") = HORN_CMD_BAND;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "bxor") = HORN_CMD_BXOR;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "bor") = HORN_CMD_BOR;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "scope") = HORN_CMD_SCOPE;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "label") = HORN_CMD_LABEL;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "goto") = HORN_CMD_GOTO;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "call") = HORN_CMD_CALL;
	*assoc_set_horn_Cmd(&g_horn_lispKW, "var") = HORN_CMD_VAR;

	*assoc_set_horn_Cmd(&g_horn_taurusKW, "var") = HORN_CMD_VAR;
	*assoc_set_horn_Cmd(&g_horn_taurusKW, "goto") = HORN_CMD_GOTO;
	
	return false;
}

void horn_resetState(horn_State* state, const char* src){
	state->lookahead.type = HORN_TT_UKN;
	state->lookahead.begin = NULL;
	state->lookahead.end = NULL;
	state->src = src;
	state->cursor = src;
	state->row = 0;
	state->column = 0;
}

void horn_terminate(void){
	assoc_free(g_horn_lispKW);
	assoc_free(g_horn_taurusKW);

	g_horn_lispKW = NULL;
}
