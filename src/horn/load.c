#include <trs/horn.h>

#include <stdio.h>

static horn_Obj* s_parseExpr(horn_State* state);

static horn_Cmd s_cmd(const horn_Token* tok){
	if(tok->type != HORN_TT_ID)
		return HORN_CMD_ERROR;

	char* text = horn_getTokenText(tok);
	horn_Cmd cmd = assoc_getOrDefault_horn_Cmd(g_horn_lispKW, text, HORN_CMD_ERROR);
	free(text);
	return cmd;
}

static horn_Obj* s_parseSExpr(horn_State* state){
	horn_Token tok;
	
	horn_Obj* obj = horn_newObj();
	if(!obj) return NULL;

	horn_next(state, &tok);

	if(tok.type != HORN_TT_LP){
		// TODO: ERROR: Expected (
		return NULL;
	}

	horn_next(state, &tok);

	if(tok.type != HORN_TT_ID){
		fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		return obj;
	}

	horn_Cmd cmd = s_cmd(&tok);
	if(cmd == HORN_CMD_ERROR){
		int len = tok.end - tok.begin;
		fprintf(stderr, "ERROR: Unknown command: %*s\n", len, tok.begin);
		return obj;
	}

	horn_LH(state, &tok);
	if(tok.type != HORN_TT_RP){
		horn_Obj* args = s_parseExpr(state);
		obj->as.args = args;
		horn_LH(state, &tok);
		while(args && tok.type != HORN_TT_RP){
			args->next = s_parseExpr(state);
			args = args->next;
			horn_LH(state, &tok);
		}
	}

	if(tok.type != HORN_TT_RP){
		// TODO: ERROR: Exceped ')'
		return NULL;
	}

	horn_next(state, NULL);

	obj->cmd = cmd;
	return obj;
}


static horn_Obj* s_parseExpr(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	switch(tok.type){
		case HORN_TT_LP:
			return s_parseSExpr(state);
		case HORN_TT_INT:
		{
			horn_next(state, NULL);
			horn_Obj* v = horn_newObj();
			if(!v) return NULL; // TODO: ERROR
			v->cmd = HORN_CMD_INTVAL;
			v->as.text = horn_getTokenText(&tok); // TODO: PMA
			return v;
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

horn_Obj* horn_load(const char* src){
	horn_State state = {0};
	horn_resetState(&state, src);

	horn_Obj* ret = s_parseSExpr(&state);

	horn_Obj* head = ret;
	while(head){
		head->next = s_parseSExpr(&state);
		head = head->next;
	}

	return ret;
}
