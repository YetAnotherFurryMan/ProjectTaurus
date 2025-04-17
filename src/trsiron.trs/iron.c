#define IRON_LEX
#include <trs/iron.h>

#include <trs/error.h>
#include <trs/taurus.h>

static inline bool s_expect(utils_State* state, utils_Token* tok, size_t type){
	iron_next(state, tok);
	if(tok->type != type){
		// TODO: ERROR
		return true;
	}
	return false;
}

static horn_Obj* s_scope(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	horn_Obj* scope = horn_newObj(pgm);
	if(!scope) return NULL; // TODO: ERROR
	scope->cmd = HORN_CMD_SCOPE;

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_LB){
		// TODO: Parse code
		
		iron_next(state, NULL);

		// TODO: U know

		if(s_expect(state, &tok, TRS_TT_RB))
			return NULL;
	} else{
		// TODO: Parse one command and return it
	}

	return scope;
}

static horn_Obj* s_fn(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	horn_Obj* fn = horn_newObj(pgm);
	if(!fn) return NULL; // TODO: ERROR
	fn->cmd = HORN_CMD_FUNCTION;
	
	if(s_expect(state, &tok, TRS_TT_KW_FN))
		return NULL;

	if(s_expect(state, &tok, TRS_TT_ID))
		return NULL;

	horn_Obj* id = horn_newObj(pgm);
	if(!id) return NULL;
	id->cmd = HORN_CMD_ID;
	id->as.text = horn_getTokenText(pgm, &tok); // TODO: Move to utils
	fn->as.args = id;

	// TODO: Parse args
	if(s_expect(state, &tok, TRS_TT_LP))
		return NULL;

	horn_Obj* args = horn_newObj(pgm);
	if(!args) return NULL; // TODO: ERROR
	args->cmd = HORN_CMD_NIL;
	id->next = args;

	if(s_expect(state, &tok, TRS_TT_RP))
		return NULL;

	iron_LH(state, &tok);

	if(tok.type == TRS_TT_COLON){
		// TODO: Parse type
	} else {
		fn->cmd = HORN_CMD_PROCEDURE;
	}

	horn_Obj* scope = s_scope(pgm, state);
	if(!scope) return NULL;
	args->next = scope;

	return fn;
}

bool iron_load(horn_Instance* inst, const char* src){
	if(!inst){
		LOGENL(EIDX_HORN_BAD_INSTANCE, NULL);
		return true;
	}

	if(!src){
		LOGENL(EIDX_INTERNAL_BAD_DATA_SRC, "src");
		return true;
	}

	utils_State state = {0};
	utils_resetState(&state, src);

	horn_Obj obj_base = {0};
	horn_Obj* obj = &obj_base;

	utils_Token tok = {0};
	iron_LH(&state, &tok);

	do{
		switch(tok.type){
			case TRS_TT_KW_FN:
			{
				obj->next = s_fn(&inst->alloc, &state);
			} break;
			default:
				// TODO: ERROR
				return true;
		}

		obj = obj->next;
		iron_LH(&state, &tok);
	} while(obj && tok.type != TRS_TT_EOF && tok.type != TRS_TT_UKN);

	if(inst->src){
		inst->src_end->next = obj_base.next;
		inst->src_end = obj_base.next;
	} else{
		inst->src = obj_base.next;
		inst->src_end = obj_base.next;
	}
	
	return false;
}
