#include <trs/cg.h>

int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
	switch(obj->cmd){
		case HORN_CMD_ID:
		case HORN_CMD_INTVAL:
		{
			fputs(obj->text, out);
		} break;
		case HORN_CMD_SET:
		{
			if(!obj->args){
				fprintf(stderr, "ERROR: SET expects argument!\n");
				return 1;
			}

			fprintf(out, "(set %s ", obj->text);
			trs_cgCompileCmd(out, obj->args);
			fputs(")", out);
		} break;
		case HORN_CMD_ADD:
		{
			fputs("(add", out);

			horn_Obj* arg = obj->args;
			while(arg){
				fputs(" ", out);
				trs_cgCompileCmd(out, arg);
				arg = arg->next;
			}
			fputs(")", out);
		} break;
		case HORN_CMD_MUL:
		{
			fputs("(mul", out);

			horn_Obj* arg = obj->args;
			while(arg){
				fputs(" ", out);
				trs_cgCompileCmd(out, arg);
				arg = arg->next;
			}
			fputs(")", out);
		} break;
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(obj->cmd));
			return 1;
	}

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

