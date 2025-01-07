#include <trs/horn.h>

#include <toollib/toollib.h>

#include <stdbool.h>

static inline bool horn_analiseBi(horn_Obj* var, horn_Cmd cmd, horn_Obj* ir, const char* defaultStr);
static inline bool horn_analiseExp(horn_Obj* var, horn_Obj* ir);

static inline bool horn_analiseBi(horn_Obj* var, horn_Cmd cmd, horn_Obj* ir, const char* defaultStr){
	if(!ir->as.args){
		ir->cmd = HORN_CMD_INTVAL;
		ir->as.text = tl_strcpy(defaultStr);
	} else{
		horn_Obj** obj = &ir->as.args;
		while(*obj){
			if(!horn_analiseExp(var, *obj))
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

static inline bool horn_analiseExp(horn_Obj* var, horn_Obj* ir){
	switch(ir->cmd){
		case HORN_CMD_ID:
		case HORN_CMD_INTVAL:
			break;
		case HORN_CMD_SET:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_ID){
				// TODO: ERROR
				return false;
			}

			if(!ir->as.args->next){
				// TODO: ERROR
				return false;
			}

			return horn_analiseExp(var, ir->as.args->next);
		} break;
		case HORN_CMD_ADD:
		case HORN_CMD_SUB:
			return horn_analiseBi(var, ir->cmd, ir, "0");
		case HORN_CMD_MUL:
			return horn_analiseBi(var, HORN_CMD_MUL, ir, "1");
		case HORN_CMD_MINUS:
			return horn_analiseExp(var, ir->as.args);
		case HORN_CMD_SCOPE:
			return horn_analise(ir->as.args);
		case HORN_CMD_LABEL:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_ID){
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

			if(ir->as.args->cmd != HORN_CMD_ID){
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
			if(!var->as.args){
				var->as.args = ir->as.args;
			} else{
				horn_Obj* top = var->as.args;
				while(top->next)
					top = top->next;
				top->next = ir->as.args;
			}

			ir->cmd = HORN_CMD_NOP;
			ir->as.args = NULL;
		} break;
		case HORN_CMD_CALL:
		{
			if(!ir->as.args){
				// TODO: ERROR
				return false;
			}

			if(ir->as.args->cmd != HORN_CMD_ID){
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

	// TODO: Use horn_alloc and pgm
	horn_Obj* var = malloc(sizeof(horn_Obj));
	var->as.args = var->next = NULL;

	horn_Obj* obj = ir;
	while(obj){
		if(!horn_analiseExp(var, obj)) 
			return false;
		obj = obj->next;
	}

	if(var->as.args){
		horn_Obj* tmp = var->as.args;

		var->cmd = ir->cmd;
		var->as.args = ir->as.args;
		var->next = ir->next;
		var->as.text = ir->as.text;

		ir->cmd = HORN_CMD_VAR;
		ir->as.args = tmp;
		ir->next = var;
	}

	return true;
}
