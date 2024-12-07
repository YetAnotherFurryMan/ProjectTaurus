#include <trs/horn.h>

#include <toollib/toollib.h>

#include <stdbool.h>

static inline bool horn_analiseBi(horn_Obj* var, horn_Cmd cmd, horn_Obj* ir, const char* defaultStr);
static inline bool horn_analiseExp(horn_Obj* var, horn_Obj* ir);

static inline bool horn_analiseBi(horn_Obj* var, horn_Cmd cmd, horn_Obj* ir, const char* defaultStr){
	if(!ir->args){
		ir->cmd = HORN_CMD_INTVAL;
		ir->text = tl_strcpy(defaultStr);
	} else{
		horn_Obj** obj = &ir->args;
		while(*obj){
			if(!horn_analiseExp(var, *obj))
				return false;

			if((*obj)->cmd == cmd){
				horn_Obj* o = *obj;
				if(o->args){
					*obj = o->args;

					while(o->args->next)
						o->args = o->args->next;
					o->args->next = o->next;
				} else{
					o->cmd = HORN_CMD_INTVAL;
					o->text = tl_strcpy(defaultStr);
				}
			}
			
			obj = &(*obj)->next;
		}

		if(!ir->args){
			ir->cmd = HORN_CMD_INTVAL;
			ir->text = tl_strcpy(defaultStr);
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
			if(!ir->args){
				// TODO: ERROR
				return false;
			}

			if(ir->args->cmd != HORN_CMD_ID){
				// TODO: ERROR
				return false;
			}

			if(!ir->args->next){
				// TODO: ERROR
				return false;
			}

			return horn_analiseExp(var, ir->args->next);
		} break;
		case HORN_CMD_ADD:
		case HORN_CMD_SUB:
			return horn_analiseBi(var, ir->cmd, ir, "0");
		case HORN_CMD_MUL:
			return horn_analiseBi(var, HORN_CMD_MUL, ir, "1");
		case HORN_CMD_MINUS:
			return horn_analiseExp(var, ir->args);
		case HORN_CMD_SCOPE:
			return horn_analise(ir->args);
		case HORN_CMD_LABEL:
		{
			if(!ir->args){
				// TODO: ERROR
				return false;
			}

			if(ir->args->cmd != HORN_CMD_ID){
				// TODO: ERROR
				return false;
			}

			if(ir->args->next){
				// TODO: ERROR
				return false;
			}
		} break;
		case HORN_CMD_GOTO:
		{
			if(!ir->args){
				// TODO: ERROR
				return false;
			}

			if(ir->args->cmd != HORN_CMD_ID){
				// TODO: ERROR
				return false;
			}

			if(ir->args->next){
				// TODO: ERROR
				return false;
			}
		} break;
		case HORN_CMD_VAR:
		{
			if(!var->args){
				var->args = ir->args;
			} else{
				horn_Obj* top = var->args;
				while(top->next)
					top = top->next;
				top->next = ir->args;
			}

			ir->cmd = HORN_CMD_NOP;
			ir->args = NULL;
		} break;
		case HORN_CMD_CALL:
		{
			if(!ir->args){
				// TODO: ERROR
				return false;
			}

			if(ir->args->cmd != HORN_CMD_ID){
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
	var->args = var->next = NULL;
	var->text = NULL;

	horn_Obj* obj = ir;
	while(obj){
		if(!horn_analiseExp(var, obj)) 
			return false;
		obj = obj->next;
	}

	if(var->args){
		horn_Obj* tmp = var->args;

		var->cmd = ir->cmd;
		var->args = ir->args;
		var->next = ir->next;
		var->text = ir->text;

		ir->cmd = HORN_CMD_VAR;
		ir->args = tmp;
		ir->next = var;
		ir->text = NULL;
	}

	return true;
}
