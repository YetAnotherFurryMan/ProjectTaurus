#include <trs/cg.h>

int trs_cgCompileTerm(FILE* out, const char* name, horn_Obj* args){
	if(!args){
		fprintf(stderr, "ERROR: ADD expects at least one argument!\n");
		return 1;
	}

	// First arg to eax
	int err = trs_cgCompileCmd(out, args);

	args = args->next;
	while(args && !err){
		switch(args->cmd){
			case HORN_CMD_ID:
			{
				fprintf(out, "\t%s eax, dword [%s]\n", name, args->text);
			} break;
			case HORN_CMD_INTVAL:
			{
				fprintf(out, "\t%s eax, %s\n", name, args->text);
			} break;
			case HORN_CMD_MINUS:
			case HORN_CMD_ADD:
			case HORN_CMD_SUB:
			case HORN_CMD_MUL:
			case HORN_CMD_CALL:
			{
				fputs("\tpush eax\n", out);
				err = trs_cgCompileCmd(out, args);
				fputs("\tmov eax, ebx\n", out);
				fputs("\tpop eax\n", out);
				fprintf(out, "\t%s eax, ebx\n", name);
			} break;
			case HORN_CMD_SET:
			{
				fprintf(stderr, "ERROR: Cannot do SET operation inside of TERM.\n");
				err = 1;
			} break;
			default:
				fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(args->cmd));
				err = 1;
		}

		args = args->next;
	}

	return err;
}

int trs_cgCompileMul(FILE* out, horn_Obj* args){
	if(!args){
		fprintf(stderr, "ERROR: MUL expects at least one argument!\n");
		return 1;
	}

	// First arg to eax
	int err = trs_cgCompileCmd(out, args);

	args = args->next;
	while(args && !err){
		switch(args->cmd){
			case HORN_CMD_ID:
			{
				fprintf(out, "\tmov ebx, dword [%s]\n", args->text);
				fprintf(out, "\tmul ebx\n");
			} break;
			case HORN_CMD_INTVAL:
			{
				fprintf(out, "\tmov ebx, %s\n", args->text);
				fprintf(out, "\tmul ebx\n");
			} break;
			case HORN_CMD_MINUS:
			case HORN_CMD_ADD:
			case HORN_CMD_SUB:
			case HORN_CMD_MUL:
			case HORN_CMD_CALL:
			{
				fprintf(out, "\tpush eax\n");
				err = trs_cgCompileCmd(out, args);
				fprintf(out, "\tpop ebx\n");
				fprintf(out, "\tmul ebx\n");
			} break;
			case HORN_CMD_SET:
			{
				fprintf(stderr, "ERROR: Cannot do SET operation inside of MUL.\n");
				err = 1;
			} break;
			default:
				fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(args->cmd));
				err = 1;
		}

		args = args->next;
	}

	return err;
}

int trs_cgCompileScope(FILE* out, horn_Obj* scope){
	int r = 0;
	horn_Obj* obj = scope->args;
	while(obj){
		fprintf(stderr, "INFO: obj(%s)\n", horn_CmdToString(obj->cmd));
		if((r = trs_cgCompileCmd(out, obj)))
			return r;
		obj = obj->next;
	}
	return 0;
}

int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
	int err = 0;

	switch(obj->cmd){
		case HORN_CMD_NOP: break;
		case HORN_CMD_ID:
		{
			// Load a value to eax
			fprintf(out, "\tmov eax, dword [%s]\n", obj->text);
		} break;
		case HORN_CMD_INTVAL:
		{
			// Put the intval into eax
			fprintf(out, "\tmov eax, %s\n", obj->text);
		} break;
		case HORN_CMD_SET:
		{
			// Compile arg into eax and then save to destination
			// The first arg is always ID and the second is value
			err = trs_cgCompileCmd(out, obj->args->next);
			fprintf(out, "\tmov dword [%s], eax\n", obj->args->text);
		} break;
		case HORN_CMD_MINUS:
		{
			// TODO: Solve - - and -INTVAL
			// Compile arg into eax and change sign
			err = trs_cgCompileCmd(out, obj->args);
			fputs("\tnot eax\n", out);
			fputs("\tinc eax\n", out);
		} break;
		case HORN_CMD_ADD:
			return trs_cgCompileTerm(out, "add", obj->args);
		case HORN_CMD_SUB:
			return trs_cgCompileTerm(out, "sub", obj->args);
		case HORN_CMD_MUL:
			return trs_cgCompileMul(out, obj->args);
		case HORN_CMD_SCOPE:
			return trs_cgCompileScope(out, obj);
		case HORN_CMD_LABEL:
		{
			fprintf(out, ".%s:\n", obj->args->text);
		} break;
		case HORN_CMD_GOTO:
		{
			fprintf(out, "\tjmp .%s\n", obj->args->text);
		} break;
		case HORN_CMD_CALL:
		{
			// TODO: Compile args
			fprintf(out, "\tcall %s\n", obj->args->text);
		} break;
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", horn_CmdToString(obj->cmd));
			return 1;
	}

	return err;
}

// TODO: Revrite std to cdecl
// ECX EDX
int trs_cgCompile(FILE* out, horn_Obj* obj){
	// Runtime
	fputs("section .data\n", out);
	fputs("\tA dd 0\n", out);
	fputs("\tB dd 0\n", out);

	// Compile VAR if exists
	if(obj->cmd == HORN_CMD_VAR){
		horn_Obj* var = obj->args;
		while(var){
			fprintf(out, "\t%s dd 0\n", var->text);
			var = var->next;
		}

		obj = obj->next;
	}
	// END

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
	while(obj){
		fprintf(stderr, "INFO: obj(%s)\n", horn_CmdToString(obj->cmd));
		if((r = trs_cgCompileCmd(out, obj)))
			return r;
		obj = obj->next;
	}

	// Exit
	fputs("\tret\n", out);

	return 0;
}
