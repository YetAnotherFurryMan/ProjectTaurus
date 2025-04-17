#include <trs/horn.h>

#include <trs/error.h>

static horn_Obj* s_parseExpr(horn_Instance* inst, utils_State* state);

static horn_Cmd s_cmd(horn_Instance* inst, const utils_Token* tok){
	if(tok->type != HORN_TT_ID)
		return HORN_CMD_ERROR;

	char* text = horn_getTokenTextTmp(tok);
	horn_Cmd cmd = assoc_getOrDefault_horn_Cmd(inst->kw_map, text, HORN_CMD_ERROR);
	free(text);
	return cmd;
}

static horn_Obj* s_parseSExpr(horn_Instance* inst, utils_State* state){
	utils_Token tok;

	pgm* pgm = &inst->alloc;
	
	horn_Obj* obj = horn_newObj(pgm);
	if(!obj) return NULL;

	horn_next(state, &tok);

	if(tok.type != HORN_TT_LP){
		LOGENL(EIDX_HORN_EXPECTED_GOT, "(", horn_TokenTypeToString(tok.type));
		return NULL;
	}

	horn_next(state, &tok);

	if(tok.type != HORN_TT_ID){
		LOGENL(EIDX_HORN_EXPECTED_GOT, "identifier", horn_TokenTypeToString(tok.type));
		return NULL;
	}

	horn_Cmd cmd = s_cmd(inst, &tok);
	if(cmd == HORN_CMD_ERROR){
		int len = tok.end - tok.begin;
		LOGENL(EIDX_HORN_UNKNOWN_CMD_LEN, len, tok.begin);
		return NULL;
	}

	horn_LH(state, &tok);
	if(tok.type != HORN_TT_RP){
		horn_Obj* args = s_parseExpr(inst, state);
		obj->as.args = args;
		horn_LH(state, &tok);
		while(args && tok.type != HORN_TT_RP){
			args->next = s_parseExpr(inst, state);
			args = args->next;
			horn_LH(state, &tok);
		}
	}

	if(tok.type != HORN_TT_RP){
		LOGENL(EIDX_HORN_EXPECTED_GOT, ")", horn_TokenTypeToString(tok.type));
		return NULL;
	}

	horn_next(state, NULL);

	obj->cmd = cmd;
	return obj;
}


static horn_Obj* s_parseExpr(horn_Instance* inst, utils_State* state){
	utils_Token tok = {0};
	horn_LH(state, &tok);

	pgm* pgm = &inst->alloc;

	switch(tok.type){
		case HORN_TT_LP:
			return s_parseSExpr(inst, state);
		case HORN_TT_INT:
		{
			horn_next(state, NULL);
			horn_Obj* v = horn_newObj(pgm);
			if(!v) return NULL; // TODO: ERROR
			v->cmd = HORN_CMD_INTVAL;
			v->as.text = horn_getTokenText(pgm, &tok);
			return v;
		} break;
		case HORN_TT_STR:
		{
			horn_next(state, NULL);
			horn_Obj* v = horn_newObj(pgm);
			if(!v) return NULL; // TODO: ERROR
			v->cmd = HORN_CMD_STRVAL;
			v->as.text = horn_getTokenText(pgm, &tok);
			return v;
		} break;
		case HORN_TT_OBJ:
		{
			horn_next(state, NULL);

			horn_Obj* id = horn_newObj(pgm);
			if(!id) return NULL; // TODO: ERROR
			id->cmd = HORN_CMD_ID;
			id->as.text = horn_getTokenText(pgm, &tok);
			return id;
		} break;
		case HORN_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		}
	}

	return NULL;
}

bool horn_load(horn_Instance* inst, const char* src){
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

	horn_Obj* ret = s_parseSExpr(inst, &state);

	utils_Token tok = {0};
	horn_LH(&state, &tok);

	horn_Obj* head = ret;
	while(head && tok.type != HORN_TT_EOF){
		head->next = s_parseSExpr(inst, &state);
		head = head->next;
		horn_LH(&state, &tok);
	}

	if(inst->src){
		inst->src_end->next = ret;
		inst->src_end = ret;
	} else{
		inst->src = ret;
		inst->src_end = ret;
	}

	return false;
}
