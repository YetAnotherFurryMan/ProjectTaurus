#include <trs/horn.h>

#include <toollib/toollib.h>

#include <stdbool.h>

static inline bool horn_analiseBi(horn_Cmd cmd, horn_Obj* ir, const char* defaultStr);
static inline bool horn_analiseExp(horn_Obj* ir);

static inline bool horn_analiseBi(horn_Cmd cmd, horn_Obj* ir, const char* defaultStr){
	if(!ir->as.args){
		ir->cmd = HORN_CMD_INTVAL;
		ir->as.text = tl_strcpy(defaultStr);
	} else{
		horn_Obj** obj = &ir->as.args;
		while(*obj){
			if(!horn_analiseExp(*obj))
				return false;

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

	return true;
}

static inline bool horn_analiseExp(horn_Obj* ir){
	switch(ir->cmd){
		case HORN_CMD_QUOTE:
		case HORN_CMD_INTVAL:
			break;
		case HORN_CMD_GET:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: QUOTE
				// TODO: ERROR
				return false;
			}

			// TODO: next arg(s) with indexes
		} break;
		case HORN_CMD_SET:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: ERROR
				return false;
			}

			if(!ir->as.args->next){
				// TODO: ERROR
				return false;
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
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->next){
				// TODO: ERROR
				return false;
			}
		} break;
		case HORN_CMD_GOTO:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->next){
				// TODO: ERROR
				return false;
			}
		} break;
		case HORN_CMD_VAR:
		{
			// TODO: (var 'id (type) (val)?)
			if(!ir->as.args){
				// TODO: ERORR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->next){
				return horn_analiseExp(ir->as.args->next);
			}
		} break;
		case HORN_CMD_CALL:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_QUOTE){
				// TODO: ERROR
				return false;
			}
		} break;
		default:
			return false;
	}
	return true;
}

bool horn_analise(horn_Obj* ir){
	if(!ir)
		return false;

	horn_Obj* obj = ir;
	while(obj){
		if(!horn_analiseExp(obj)) 
			return false;
		obj = obj->next;
	}

	return true;
}
