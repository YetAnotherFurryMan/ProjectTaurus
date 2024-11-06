#include <trs/cg.h>

int trs_cgCompileCmd(FILE* out, trs_IR* ir){
	switch(ir->cmd){
		case TRS_IRCMD_LOAD:
		case TRS_IRCMD_INTVAL:
		{
			fputs(ir->text, out);
		} break;
		case TRS_IRCMD_SET:
		{
			// Compile arg into eax and then save to destination
			if(!ir->args){
				fprintf(stderr, "ERROR: SET expects argument!\n");
				return 1;
			}

			fprintf(out, "(set %s ", ir->text);
			trs_cgCompileCmd(out, ir->args);
			fputs(")", out);
		} break;
		case TRS_IRCMD_ADD:
		{
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, add eax and ebx
			if(!ir->args || !ir->args->next){
				fprintf(stderr, "ERROR: ADD expects 2 arguments!\n");
				return 1;
			}

			fputs("(add ", out);
			trs_cgCompileCmd(out, ir->args);
			fputs(" ", out);
			trs_cgCompileCmd(out, ir->args->next);
			fputs(")", out);
		} break;
		case TRS_IRCMD_MUL:
		{
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, mul by ebx
			if(!ir->args || !ir->args->next){
				fprintf(stderr, "ERROR: MUL expects 2 arguments!\n");
				return 1;
			}

			fputs("(mul ", out);
			trs_cgCompileCmd(out, ir->args);
			fputs(" ", out);
			trs_cgCompileCmd(out, ir->args->next);
			fputs(")", out);
		} break;
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", trs_IRCmdToString(ir->cmd));
			return 1;
	}

	return 0;
}

int trs_cgCompile(FILE* out, trs_IR* ir){
	int r = 0;
	while(ir){
		fprintf(stderr, "INFO: IR(%s)\n", trs_IRCmdToString(ir->cmd));
		if((r = trs_cgCompileCmd(out, ir)))
			return r;
		fputs("\n", out);
		ir = ir->next;
	}

	return 0;
}

