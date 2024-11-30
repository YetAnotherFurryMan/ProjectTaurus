#include <trs/horn.h>

#include <toollib/toollib.h>

#include <stdbool.h>

static inline bool horn_analiseExp(horn_Obj* ir);

static inline bool horn_analise1(horn_Cmd cmd, horn_Obj* ir, const char* defaultStr){
	if(!ir->args){
		ir->cmd = HORN_CMD_INTVAL;
		ir->text = tl_strcpy(defaultStr);
	} else{
		horn_Obj* prv = NULL;
		horn_Obj* obj = ir->args;
		while(obj){
			if(!horn_analiseExp(obj))
				return false;

			if(obj->cmd == cmd){
				if(!prv)
					prv = obj->args;
				else
					prv->next = obj->args;
				
				while(prv->next)
					prv = prv->next;

				prv->next = obj->next;
				
				obj->args = NULL;
				obj->next = NULL;
				free(obj);
				
				obj = prv;
			}

			prv = obj;
			obj = obj->next;
		}

		if(!ir->args->next){
			horn_Obj* o = ir->args;
			ir->cmd = o->cmd;
			ir->text = o->text;
			ir->args = o->args;

			o->args = NULL;
			free(o);
		}
	}

	return true;
}

static inline bool horn_analiseExp(horn_Obj* ir){
	switch(ir->cmd){
		case HORN_CMD_ID:
		case HORN_CMD_INTVAL:
			break;
		case HORN_CMD_SET:
		{
			// TODO: Test if 1st arg is ID and second is evaluatable
			ir = ir->args;
			while(ir){
				if(!horn_analiseExp(ir))
					return false;
				ir = ir->next;
			}
		} break;
		case HORN_CMD_ADD:
			return horn_analise1(HORN_CMD_ADD, ir, "0");
		case HORN_CMD_MUL:
			return horn_analise1(HORN_CMD_MUL, ir, "1");
		case HORN_CMD_SCOPE:
			return horn_analise(ir->args);
			break;
		default:
			return false;
	}
	return true;
}

bool horn_analise(horn_Obj* ir){
	if(!ir)
		return false;

	while(ir){
		if(!horn_analiseExp(ir)) 
			return false;
		ir = ir->next;
	}

	return true;
}
