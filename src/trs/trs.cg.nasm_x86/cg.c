#include <trs/cg.h>

int trs_cgCompileAdd(FILE* out, trs_IR* args){
	if(!args){
		fprintf(stderr, "ERROR: ADD expects at least one argument!\n");
		return 1;
	}

	// First arg to eax
	int err = trs_cgCompileCmd(out, args);

	args = args->next;
	while(args && !err){
		switch(args->cmd){
			case TRS_IRCMD_LOAD:
			{
				fprintf(out, "\tadd eax, dword [%s]\n", args->text);
			} break;
			case TRS_IRCMD_INTVAL:
			{
				fprintf(out, "\tadd eax, %s\n", args->text);
			} break;
			case TRS_IRCMD_ADD:
			case TRS_IRCMD_MUL:
			{
				fprintf(out, "\tpush eax\n");
				err = trs_cgCompileCmd(out, args);
				fprintf(out, "\tpop ebx\n");
				fprintf(out, "\tadd eax, ebx\n");
			} break;
			case TRS_IRCMD_SET:
			{
				fprintf(stderr, "ERROR: Cannot do SET operation inside of ADD.\n");
				err = 1;
			} break;
			default:
				fprintf(stderr, "ERROR: Unexpected %s\n", trs_IRCmdToString(args->cmd));
				err = 1;
		}

		args = args->next;
	}

	return err;
}

int trs_cgCompileMul(FILE* out, trs_IR* args){
	if(!args){
		fprintf(stderr, "ERROR: MUL expects at least one argument!\n");
		return 1;
	}

	// First arg to eax
	int err = trs_cgCompileCmd(out, args);

	args = args->next;
	while(args && !err){
		switch(args->cmd){
			case TRS_IRCMD_LOAD:
			{
				fprintf(out, "\tmov ebx, dword [%s]\n", args->text);
				fprintf(out, "\tmul ebx\n");
			} break;
			case TRS_IRCMD_INTVAL:
			{
				fprintf(out, "\tmov ebx, %s\n", args->text);
				fprintf(out, "\tmul ebx\n");
			} break;
			case TRS_IRCMD_ADD:
			case TRS_IRCMD_MUL:
			{
				fprintf(out, "\tpush eax\n");
				err = trs_cgCompileCmd(out, args);
				fprintf(out, "\tpop ebx\n");
				fprintf(out, "\tmul ebx\n");
			} break;
			case TRS_IRCMD_SET:
			{
				fprintf(stderr, "ERROR: Cannot do SET operation inside of MUL.\n");
				err = 1;
			} break;
			default:
				fprintf(stderr, "ERROR: Unexpected %s\n", trs_IRCmdToString(args->cmd));
				err = 1;
		}

		args = args->next;
	}

	return err;
}


int trs_cgCompileCmd(FILE* out, trs_IR* ir){
	int err = 0;

	switch(ir->cmd){
		case TRS_IRCMD_LOAD:
		{
			// Load a value to eax
			fprintf(out, "\tmov eax, dword [%s]\n", ir->text);
		} break;
		case TRS_IRCMD_INTVAL:
		{
			// Put the intval into eax
			fprintf(out, "\tmov eax, %s\n", ir->text);
		} break;
		case TRS_IRCMD_SET:
		{
			// Compile arg into eax and then save to destination
			if(!ir->args){
				fprintf(stderr, "ERROR: SET expects argument!\n");
				return 1;
			}

			err = trs_cgCompileCmd(out, ir->args);
			fprintf(out, "\tmov dword [%s], eax\n", ir->text);
		} break;
		case TRS_IRCMD_ADD:
			return trs_cgCompileAdd(out, ir->args);
		case TRS_IRCMD_MUL:
			return trs_cgCompileMul(out, ir->args);
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", trs_IRCmdToString(ir->cmd));
			return 1;
	}

	return err;
}

// ECX EDX
int trs_cgCompile(FILE* out, trs_IR* ir){
	// Runtime
	fputs("section .data\n", out);
	fputs("\tA dd 0\n", out);
	fputs("\tB dd 0\n", out);
	fputs("\n", out);
	fputs("section .text\n", out);
	fputs("\tglobal _start\n", out);
	fputs("\tglobal main\n", out);
	fputs("\tglobal hex32tos\n", out);
	fputs("\tglobal put_hex\n", out);
	fputs("\tglobal ln\n", out);
	fputs("_start:\n", out);
	fputs("\tcall main\n", out);
	// write A and B
	
	fputs("\tmov ecx, dword [A]\n", out);
	fputs("\tcall put_hex\n", out);
	fputs("\tcall ln\n", out);

	fputs("\tmov ecx, dword [B]\n", out);
	fputs("\tcall put_hex\n", out);
	fputs("\tcall ln\n", out);

	// end
	fputs("\tmov eax, 1\n", out);
	fputs("\tmov ebx, 0\n", out);
	fputs("\tint 80h\n", out);
	fputs("\n", out);
	fputs("put_hex:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov edx, ecx\n", out);
	fputs("\tlea ecx, [ebp - 11]\n", out);
	fputs("\tcall hex32tos\n", out);
	fputs("\tmov eax, 4\n", out);
	fputs("\tmov ebx, 1\n", out);
	fputs("\tlea ecx, [ebp - 11]\n", out);
	fputs("\tmov edx, 10\n", out);
	fputs("\tint 80h\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);
	fputs("hex32tos:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov dword [ebp - 4], 8\n", out);
	fputs("\tmov word [ecx], '0x'\n", out);
	fputs("\tadd ecx, 2\n", out);
	fputs("\tmov dword [ecx], 0\n", out);
	fputs("\tmov dword [ecx + 4], 0\n", out);
	fputs(".L3:\n", out);
	fputs("\tsub dword [ebp - 4], 1\n", out);
	fputs("\tmov eax, dword [ebp - 4]\n", out);
	fputs("\tmov ebx, edx\n", out);
	fputs("\tand ebx, 15\n", out);
	fputs("\tshr edx, 4\n", out);
	fputs("\tcmp ebx, 9\n", out);
	fputs("\tjg .L1\n", out);
	fputs("\tadd bl, '0'\n", out);
	fputs("\tjmp .L2\n", out);
	fputs(".L1:\n", out);
	fputs("\tadd bl, 'A' - 10\n", out);
	fputs(".L2:\n", out);
	fputs("\tmov byte [ecx + eax], bl\n", out);
	fputs("\tcmp eax, 0\n", out);
	fputs("\tjne .L3\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);
	fputs("ln:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov byte [ebp - 1], 10\n", out);
	fputs("\tmov eax, 4\n", out);
	fputs("\tmov ebx, 1\n", out);
	fputs("\tlea ecx, [ebp - 1]\n", out);
	fputs("\tmov edx, 1\n", out);
	fputs("\tint 80h\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);

	// Code
	fputs("main:\n", out);

	int r = 0;
	while(ir){
		fprintf(stderr, "INFO: IR(%s)\n", trs_IRCmdToString(ir->cmd));
		if((r = trs_cgCompileCmd(out, ir)))
			return r;
		ir = ir->next;
	}

	// Exit
	fputs("\tret\n", out);

	return 0;
}
