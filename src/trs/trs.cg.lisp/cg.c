#include <trs/cg.h>

int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
#define XCASE(CMD, TXT) case HORN_CMD_##CMD: fputs("("#TXT, out); break; 
	switch(obj->cmd){
		case HORN_CMD_QUOTE:
		{
			fprintf(out, "\'%s", obj->as.text);
			return 0;
		} break;
		case HORN_CMD_INTVAL:
		{
			fputs(obj->as.text, out);
			return 0;
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
			fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(obj->cmd));
			return 1;
	}
#undef XCASE

	horn_Obj* arg = obj->as.args;
	while(arg){
		fputs(" ", out);
		trs_cgCompileCmd(out, arg);
		arg = arg->next;
	}
	fputs(")", out);

	return 0;
}

int trs_cgCompile(FILE* out, horn_Obj* obj){
	int r = 0;
	while(obj){
		if((r = trs_cgCompileCmd(out, obj)))
			return r;
		fputs("\n", out);
		obj = obj->next;
	}

	return 0;
}

