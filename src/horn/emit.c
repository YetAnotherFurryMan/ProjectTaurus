#include <trs/horn.h>

#include <trs/error.h>

static bool s_emit(FILE* out, horn_Obj* obj){
#define XCASE(CMD, TXT) case HORN_CMD_##CMD: fputs("("#TXT, out); break; 
	switch(obj->cmd){
		case HORN_CMD_ID:
		{
			fprintf(out, "\'%s", obj->as.text);
			return false;
		} break;
		case HORN_CMD_INTVAL:
		{
			fputs(obj->as.text, out);
			return false;
		} break;
		XCASE(GET, get)
		XCASE(SET, set)
		XCASE(MINUS, minus)
		XCASE(ADD, add)
		XCASE(SUB, sub)
		XCASE(MUL, mul)
		XCASE(SCOPE, scope)
		XCASE(LABEL, label)
		XCASE(GOTO, goto)
		XCASE(VAR, var)
		XCASE(CALL, call)
		default:
			LOGENL(EIDX_HORN_UNEXPECTED, horn_CmdToString(obj->cmd));
			return true;
	}
#undef XCASE

	horn_Obj* arg = obj->as.args;
	while(arg){
		fputs(" ", out);
		if(s_emit(out, arg)) 
			return true;
		arg = arg->next;
	}
	fputs(")", out);

	return false;
}


bool horn_emit(FILE* out, horn_Instance* inst){
	horn_Obj* src = inst->src;
	while(src){
		if(s_emit(out, src))
			return true;
		src = src->next;
	}
	return false;
}
