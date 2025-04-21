#define IRON_LEX
#include <trs/iron.h>

#include <trs/error.h>
#include <trs/taurus.h>

static inline bool s_expect(utils_State* state, utils_Token* tok, size_t type){
	iron_next(state, tok);
	if(tok->type != type){
		LOGENL(EIDX_EXPECTED_GOT, trs_TokenTypeToString(type), trs_TokenTypeToString(tok->type));
		return true;
	}
	return false;
}

static horn_Obj* s_type(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_KW_I32){
		iron_next(state, &tok);

		horn_Obj* gettype = horn_newObj(pgm);
		if(!gettype) return NULL; // TODO: ERROR
		gettype->cmd = HORN_CMD_GETTYPE;

		horn_Obj* i32 = horn_newObj(pgm);
		if(!i32) return NULL; // TODO: ERROR
		i32->cmd = HORN_CMD_ID;
		i32->as.text = "i32";
		gettype->as.args = i32;

		return gettype;
	} else{
		// TODO: More types and error
		return NULL;
	}
}

static horn_Obj* s_metaExpr(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	if(s_expect(state, &tok, TRS_TT_AT))
		return NULL;

	// TODO: Parse meta-scope
	if(s_expect(state, &tok, TRS_TT_ID))
		return NULL;

	horn_Obj* meta = horn_newObj(pgm);
	if(!meta) return NULL; // TODO: ERROR
	
	if(strncmp("print", tok.begin, 5) == 0)
		meta->cmd = HORN_CMD_PRINT;
	else {
		// TODO: ERROR
		return NULL;
	}

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_LP){
		iron_next(state, NULL);

		// TODO: Parse args list
		iron_LH(state, &tok);
		if(tok.type != TRS_TT_RP){
			horn_Obj* arg = horn_newObj(pgm);
			if(!arg) return NULL; // TODO: ERROR
			iron_next(state, NULL);
			switch(tok.type){
				case TRS_TT_STR:
				{
					arg->cmd = HORN_CMD_STRVAL;
					arg->as.text = horn_getTokenText(pgm, &tok); // TODO: Move to utils 
				} break;
				default: return NULL;
			}
			meta->as.args = arg;
		}

		if(s_expect(state, &tok, TRS_TT_RP))
			return NULL;
	}

	return meta;
}

static horn_Obj* s_expr(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	iron_LH(state, &tok);
	
	switch(tok.type){
		case TRS_TT_AT:
		{
			return s_metaExpr(pgm, state);
		} break;
		default:
			LOGENL(EIDX_UNEXPECTED, trs_TokenTypeToString(tok.type));
			return NULL;
	}
}

static horn_Obj* s_scope(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_LB){
		iron_next(state, NULL);

		horn_Obj* scope = horn_newObj(pgm);
		if(!scope) return NULL; // TODO: ERROR
		scope->cmd = HORN_CMD_SCOPE;

		horn_Obj _args = {0};
		horn_Obj* args = &_args;

		iron_LH(state, &tok);
		while(tok.type != TRS_TT_RB && args){
			args->next = s_expr(pgm, state);
			args = args->next;

			iron_LH(state, &tok);
			while(tok.type == TRS_TT_EOE){
				iron_next(state, NULL);
				iron_LH(state, &tok);
			}
		}

		if(s_expect(state, &tok, TRS_TT_RB))
			return NULL;

		scope->as.args = _args.next;
	
		return scope;
	} 
	
	return s_expr(pgm, state);
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
		iron_next(state, NULL);

		horn_Obj* type = s_type(pgm, state);
		if(!type) return NULL;
		args->next = type;
		args = type;
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
