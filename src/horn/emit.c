#include <trs/horn.h>

#include <trs/error.h>

static bool s_emit(FILE* out, horn_Obj* obj){
#define XCASE(CMD, TXT) case HORN_CMD_##CMD: fputs("("#TXT, out); break; 
	switch(obj->cmd){
		case HORN_CMD_ID:
		{
			fprintf(out, "'%s", obj->as.text);
			return false;
		} break;
		case HORN_CMD_INTVAL:
		case HORN_CMD_STRVAL:
		{
			fputs(obj->as.text, out);
			return false;
		} break;
		XCASE(GET, get)
		XCASE(GETTYPE, gettype)
		XCASE(SET, set)
		XCASE(LIST, list)
		XCASE(NIL, nil)
		XCASE(MINUS, minus)
		XCASE(ADD, add)
		XCASE(SUB, sub)
		XCASE(MUL, mul)
		XCASE(DIV, div)
		XCASE(MOD, mod)
		XCASE(LNOT, lnot)
		XCASE(LEQ, leq)
		XCASE(LNEQ, lneq)
		XCASE(LGT, lgt)
		XCASE(LLT, llt)
		XCASE(LGTQ, lgtq)
		XCASE(LLTQ, lltq)
		XCASE(LAND, land)
		XCASE(LOR, lor)
		XCASE(BNOT, bnot)
		XCASE(BAND, band)
		XCASE(BXOR, bxor)
		XCASE(BOR, bor)
		XCASE(SCOPE, scope)
		XCASE(LABEL, label)
		XCASE(GOTO, goto)
		XCASE(CALL, call)
		XCASE(RETURN, return)
		XCASE(VARIABLE, variable)
		XCASE(FUNCTION, function)
		XCASE(PROCEDURE, procedure)
		XCASE(PRINT, print)
		XCASE(PUTCHAR, putchar)
		default:
			LOGENL(EIDX_UNEXPECTED, horn_CmdToString(obj->cmd));
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
