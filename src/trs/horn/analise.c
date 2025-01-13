#include <trs/horn.h>

// FIXME: (add 1) => 1
// FIXME: (add 'A) => ERROR
// FIXME: (add (add ...) ...) => (add ... ...)

#include <toollib/toollib.h>

#include <trs/error.h>

static inline size_t horn_analiseBi(horn_Cmd cmd, horn_Obj* ir, const char* defaultStr);
static inline size_t horn_analiseExp(horn_Obj* ir);

static inline size_t horn_analiseBi(horn_Cmd cmd, horn_Obj* ir, const char* defaultStr){
	if(!ir->as.args){
		ir->cmd = HORN_CMD_INTVAL;
		ir->as.text = tl_strcpy(defaultStr);
	} else{
		horn_Obj** obj = &ir->as.args;
		size_t eidx = 0;
		while(*obj){
			if(!(eidx = horn_analiseExp(*obj)))
				return eidx;

			if((*obj)->cmd == cmd){
				horn_Obj* o = *obj;
				if(o->as.args){
					*obj = o->as.args;

					while(o->as.args->next)
						o->as.args = o->as.args->next;
					o->as.args->next = o->next;
				} else{
					o->cmd = HORN_CMD_INTVAL;
					o->as.text = tl_strcpy(defaultStr);
				}
			}
			
			obj = &(*obj)->next;
		}

		if(!ir->as.args){
			ir->cmd = HORN_CMD_INTVAL;
			ir->as.text = tl_strcpy(defaultStr);
		}
	}

	return 0;
}

static inline size_t horn_analiseExp(horn_Obj* ir){
	switch(ir->cmd){
		case HORN_CMD_QUOTE:
		case HORN_CMD_INTVAL:
			break;
		case HORN_CMD_GET:
		{
			// (get 'id [:int]*)
			if(!ir->as.args){
				LOGENL(EIDX_HORN_EXPECTED, horn_CmdToString(HORN_CMD_QUOTE));
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				LOGENL(EIDX_HORN_EXPECTED_GOT, horn_CmdToString(HORN_CMD_QUOTE), horn_CmdToString(ir->as.args->cmd));
				return EIDX_HORN_EXPECTED_GOT;
			}

			// TODO: next arg(s) with indexes
		} break;
		case HORN_CMD_SET:
		{
			// (set 'id [value])
			if(!ir->as.args){
				LOGENL(EIDX_HORN_EXPECTED, horn_CmdToString(HORN_CMD_QUOTE));
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				LOGENL(EIDX_HORN_EXPECTED_GOT, horn_CmdToString(HORN_CMD_QUOTE), horn_CmdToString(ir->as.args->cmd));
				return EIDX_HORN_EXPECTED_GOT;
			}

			if(!ir->as.args->next){
				LOGENL(EIDX_HORN_EXPECTED, "expression");
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->next->next){
				LOGENL(EIDX_HORN_UNEXPECTED_EXP, horn_CmdToString(ir->as.args->next->next->cmd));
				return EIDX_HORN_UNEXPECTED_EXP;
			}

			return horn_analiseExp(ir->as.args->next);
		} break;
		case HORN_CMD_ADD:
		case HORN_CMD_SUB:
			return horn_analiseBi(ir->cmd, ir, "0");
		case HORN_CMD_MUL:
			return horn_analiseBi(HORN_CMD_MUL, ir, "1");
		case HORN_CMD_MINUS:
			return horn_analiseExp(ir->as.args);
		case HORN_CMD_SCOPE:
			return horn_analise(ir->as.args);
		case HORN_CMD_LABEL:
		case HORN_CMD_GOTO:
		{
			// (label/goto 'id)
			if(!ir->as.args){
				LOGENL(EIDX_HORN_EXPECTED, horn_CmdToString(HORN_CMD_QUOTE));
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				LOGENL(EIDX_HORN_EXPECTED_GOT, horn_CmdToString(HORN_CMD_QUOTE), horn_CmdToString(ir->as.args->cmd));
				return EIDX_HORN_EXPECTED_GOT;
			}

			if(ir->as.args->next){
				LOGENL(EIDX_HORN_UNEXPECTED_EXP, horn_CmdToString(ir->as.args->next->cmd));
				return EIDX_HORN_UNEXPECTED_EXP;
			}
		} break;
		case HORN_CMD_VAR:
		{
			// TODO: (var 'id (type) (val)?)
			if(!ir->as.args){
				LOGENL(EIDX_HORN_EXPECTED, horn_CmdToString(HORN_CMD_QUOTE));
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				LOGENL(EIDX_HORN_EXPECTED_GOT, horn_CmdToString(HORN_CMD_QUOTE), horn_CmdToString(ir->as.args->cmd));
				return EIDX_HORN_EXPECTED_GOT;
			}

			if(ir->as.args->next){
				if(ir->as.args->next->next){
					LOGENL(EIDX_HORN_UNEXPECTED_EXP, horn_CmdToString(ir->as.args->next->next->cmd));
					return EIDX_HORN_UNEXPECTED_EXP;
				}

				return horn_analiseExp(ir->as.args->next);
			}
		} break;
		case HORN_CMD_CALL:
		{
			if(!ir->as.args){
				LOGENL(EIDX_HORN_EXPECTED, horn_CmdToString(HORN_CMD_QUOTE));
				return EIDX_HORN_EXPECTED;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				LOGENL(EIDX_HORN_EXPECTED_GOT, horn_CmdToString(HORN_CMD_QUOTE), horn_CmdToString(ir->as.args->cmd));
				return EIDX_HORN_EXPECTED_GOT;
			}

			size_t err = 0;
			horn_Obj* args = ir->as.args->next;
			while(args){
				if((err = horn_analiseExp(args)))
					return err;
				args = args->next;
			}
		} break;
		default:
		{
			LOGENL(EIDX_HORN_UNKNOWN_CMD, horn_CmdToString(ir->cmd));
			return EIDX_HORN_UNKNOWN_CMD;
		}
	}
	return 0;
}

size_t horn_analise(horn_Obj* ir){
	if(!ir)
		return 1;

	size_t eidx = 0;
	horn_Obj* obj = ir;
	while(!eidx && obj){
		eidx = horn_analiseExp(obj);
		obj = obj->next;
	}

	return eidx;
}
