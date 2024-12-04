#include <trs/cg.h>

int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
	fprintf(stderr, "INFO: obj(%s)\n", horn_CmdToString(obj->cmd));
#define XCASE(CMD, TXT) case HORN_CMD_##CMD: fputs("("#TXT, out); break; 
	switch(obj->cmd){
		case HORN_CMD_NOP: 
			return 0;
		case HORN_CMD_ID:
		case HORN_CMD_INTVAL:
		{
			fprintf(stderr, "INFO: %s{text: %s next: %p}\n", horn_CmdToString(obj->cmd), obj->text, (void*)obj->next);
			fputs(obj->text, out);
			return 0;
		} break;
		XCASE(SET, set)
		XCASE(MINUS, minus)
		XCASE(ADD, add)
		XCASE(SUB, sub)
		XCASE(MUL, mul)
		XCASE(SCOPE, scope)
		XCASE(VAR, var)
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(obj->cmd));
			return 1;
	}
#undef XCASE

	horn_Obj* arg = obj->args;
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

