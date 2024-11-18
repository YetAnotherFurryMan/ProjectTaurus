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
			// Compile arg into eax and then save to destination
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
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, add eax and ebx
			if(!obj->args || !obj->args->next){
				fprintf(stderr, "ERROR: ADD expects 2 arguments!\n");
				return 1;
			}

			fputs("(add ", out);
			trs_cgCompileCmd(out, obj->args);
			fputs(" ", out);
			trs_cgCompileCmd(out, obj->args->next);
			fputs(")", out);
		} break;
		case HORN_CMD_MUL:
		{
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, mul by ebx
			if(!obj->args || !obj->args->next){
				fprintf(stderr, "ERROR: MUL expects 2 arguments!\n");
				return 1;
			}

			fputs("(mul ", out);
			trs_cgCompileCmd(out, obj->args);
			fputs(" ", out);
			trs_cgCompileCmd(out, obj->args->next);
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

