#include <trs/cg.h>

int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
	switch(obj->cmd){
		case HORN_CMD_ID:
		case HORN_CMD_INTVAL:
		{
			fputs(obj->text, out);
			return 0;
		} break;
		case HORN_CMD_SET:
		{
			fputs("(set", out);
		} break;
		case HORN_CMD_MINUS:
			fputs("(minus", out);
			break;
		case HORN_CMD_ADD:
		{
			fputs("(add", out);
		} break;
		case HORN_CMD_SUB:
		{
			fputs("(sub", out);
		} break;
		case HORN_CMD_MUL:
		{
			fputs("(mul", out);
		} break;
		case HORN_CMD_SCOPE:
		{
			fputs("(scope", out);
		} break;
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(obj->cmd));
			return 1;
	}

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
		fprintf(stderr, "INFO: obj(%s)\n", horn_CmdToString(obj->cmd));
		if((r = trs_cgCompileCmd(out, obj)))
			return r;
		fputs("\n", out);
		obj = obj->next;
	}

	return 0;
}

