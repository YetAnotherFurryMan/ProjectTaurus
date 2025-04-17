#include <trs/horn.h>

#include <trs/error.h>

bool horn_init(horn_Instance* inst){
	if(!inst){
		LOGENL(EIDX_HORN_BAD_INSTANCE, NULL);
		return true;
	}

	inst->kw_map = assoc_new(7);
	if(!inst->kw_map){
		LOGENL(EIDX_OUT_OF_MEM, NULL);
		return true;
	}

	*assoc_set_horn_Cmd(&inst->kw_map, "get") = HORN_CMD_GET;
	*assoc_set_horn_Cmd(&inst->kw_map, "gettype") = HORN_CMD_GETTYPE;
	*assoc_set_horn_Cmd(&inst->kw_map, "set") = HORN_CMD_SET;
	*assoc_set_horn_Cmd(&inst->kw_map, "list") = HORN_CMD_LIST;
	*assoc_set_horn_Cmd(&inst->kw_map, "nil") = HORN_CMD_NIL;
	*assoc_set_horn_Cmd(&inst->kw_map, "minus") = HORN_CMD_MINUS;
	*assoc_set_horn_Cmd(&inst->kw_map, "add") = HORN_CMD_ADD;
	*assoc_set_horn_Cmd(&inst->kw_map, "sub") = HORN_CMD_SUB;
	*assoc_set_horn_Cmd(&inst->kw_map, "mul") = HORN_CMD_MUL;
	*assoc_set_horn_Cmd(&inst->kw_map, "div") = HORN_CMD_DIV;
	*assoc_set_horn_Cmd(&inst->kw_map, "mod") = HORN_CMD_MOD;
	*assoc_set_horn_Cmd(&inst->kw_map, "lnot") = HORN_CMD_LNOT;
	*assoc_set_horn_Cmd(&inst->kw_map, "leq") = HORN_CMD_LEQ;
	*assoc_set_horn_Cmd(&inst->kw_map, "lneq") = HORN_CMD_LNEQ;
	*assoc_set_horn_Cmd(&inst->kw_map, "lgt") = HORN_CMD_LGT;
	*assoc_set_horn_Cmd(&inst->kw_map, "llt") = HORN_CMD_LLT;
	*assoc_set_horn_Cmd(&inst->kw_map, "lgtq") = HORN_CMD_LGTQ;
	*assoc_set_horn_Cmd(&inst->kw_map, "lltq") = HORN_CMD_LLTQ;
	*assoc_set_horn_Cmd(&inst->kw_map, "land") = HORN_CMD_LAND;
	*assoc_set_horn_Cmd(&inst->kw_map, "lor") = HORN_CMD_LOR;
	*assoc_set_horn_Cmd(&inst->kw_map, "bnot") = HORN_CMD_BNOT;
	*assoc_set_horn_Cmd(&inst->kw_map, "band") = HORN_CMD_BAND;
	*assoc_set_horn_Cmd(&inst->kw_map, "bxor") = HORN_CMD_BXOR;
	*assoc_set_horn_Cmd(&inst->kw_map, "bor") = HORN_CMD_BOR;
	*assoc_set_horn_Cmd(&inst->kw_map, "scope") = HORN_CMD_SCOPE;
	*assoc_set_horn_Cmd(&inst->kw_map, "label") = HORN_CMD_LABEL;
	*assoc_set_horn_Cmd(&inst->kw_map, "goto") = HORN_CMD_GOTO;
	*assoc_set_horn_Cmd(&inst->kw_map, "call") = HORN_CMD_CALL;
	*assoc_set_horn_Cmd(&inst->kw_map, "return") = HORN_CMD_RETURN;
	*assoc_set_horn_Cmd(&inst->kw_map, "variable") = HORN_CMD_VARIABLE;
	*assoc_set_horn_Cmd(&inst->kw_map, "function") = HORN_CMD_FUNCTION;
	*assoc_set_horn_Cmd(&inst->kw_map, "procedure") = HORN_CMD_PROCEDURE;
	*assoc_set_horn_Cmd(&inst->kw_map, "print") = HORN_CMD_PRINT;

	return false;
}

bool horn_freeInstance(horn_Instance* inst){
	assoc_free(inst->kw_map);
	pgm_free(&inst->alloc);

	*inst = (horn_Instance){0};

	return false;
}
