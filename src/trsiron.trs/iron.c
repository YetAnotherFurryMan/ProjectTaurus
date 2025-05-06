#define IRON_LEX
#include <trs/iron.h>

#include <trs/error.h>
#include <trs/taurus.h>

static horn_Obj* s_exp(pgm* pgm, utils_State* state);

static inline bool s_expect(utils_State* state, utils_Token* tok, size_t type){
	iron_next(state, tok);
	if(tok->type != type){
		LOGENL(EIDX_EXPECTED_GOT, trs_TokenTypeToString(type), trs_TokenTypeToString(tok->type));
		return true;
	}
	return false;
}

static inline horn_Obj* s_newType(pgm* pgm, char* id){
	horn_Obj* gettype = horn_newObj(pgm);
	if(!gettype) return NULL; // TODO: ERROR
	gettype->cmd = HORN_CMD_GETTYPE;

	horn_Obj* type = horn_newObj(pgm);
	if(!type) return NULL; // TODO: ERROR
	type->cmd = HORN_CMD_ID;
	type->as.text = id;
	gettype->as.args = type;

	return gettype;
}

static horn_Obj* s_type(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_KW_I8){
		iron_next(state, &tok);
		return s_newType(pgm, "i8");
	} else if(tok.type == TRS_TT_KW_I16){
		iron_next(state, &tok);
		return s_newType(pgm, "i16");
	} else if(tok.type == TRS_TT_KW_I32){
		iron_next(state, &tok);
		return s_newType(pgm, "i32");
	} else if(tok.type == TRS_TT_KW_I64){
		iron_next(state, &tok);
		return s_newType(pgm, "i64");
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
	else if(strncmp("putchar", tok.begin, 7) == 0)
		meta->cmd = HORN_CMD_PUTCHAR;
	else {
		// TODO: ERROR
		return NULL;
	}

	iron_LH(state, &tok);
	if(tok.type == TRS_TT_LP){
		iron_next(state, NULL);

		iron_LH(state, &tok);
		if(tok.type != TRS_TT_RP){
			horn_Obj dummy = {0};
			horn_Obj* args = &dummy;
			
			args->next = s_exp(pgm, state);
			args = args->next;

			iron_LH(state, &tok);
			while(tok.type != TRS_TT_RP){
				if(s_expect(state, &tok, TRS_TT_COMMA))
					return NULL;

				args->next = s_exp(pgm, state);
				args = args->next;

				if(!args)
					return NULL;
	
				iron_LH(state, &tok);
			}

			meta->as.args = dummy.next;
		}

		if(s_expect(state, &tok, TRS_TT_RP))
			return NULL;
	}

	return meta;
}

static horn_Obj* s_primary(pgm* pgm, utils_State* state){
	utils_Token tok = {0};
	iron_LH(state, &tok);

	horn_Obj* obj = horn_newObj(pgm);
	if(!obj) return NULL; // TODO: ERROR
	obj->as.text = utils_strtok(pgm, &tok);

	switch(tok.type){
		case TRS_TT_INT:
		{
			iron_next(state, NULL);
			obj->cmd = HORN_CMD_INTVAL;
		} break;
		case TRS_TT_CHAR:
		case TRS_TT_STR:
		{
			iron_next(state, NULL);
			obj->cmd = HORN_CMD_STRVAL;
		} break;
		default:
			// TODO: ERROR: Expected primary
			return NULL;
	}

	return obj;
}

static horn_Obj* s_exp(pgm* pgm, utils_State* state){
	// TODO: expressions
	return s_primary(pgm, state);
}

static horn_Obj* s_expr(pgm* pgm, utils_State* state){
	utils_Token tok = {0};

	iron_LH(state, &tok);
	
	switch(tok.type){
		case TRS_TT_AT:
		{
			return s_metaExpr(pgm, state);
		} break;
		case TRS_TT_KW_RETURN:
		{
			horn_Obj* ret = horn_newObj(pgm);
			if(!ret) return NULL; // TODO: ERROR
			ret->cmd = HORN_CMD_RETURN;

			iron_next(state, NULL);
			iron_LH(state, &tok);

			if(tok.type != TRS_TT_EOE && tok.type != TRS_TT_RB){
				ret->as.args = s_exp(pgm, state);
				if(!ret->as.args) return NULL;
			}

			return ret;
		}
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
	id->as.text = utils_strtok(pgm, &tok);
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
