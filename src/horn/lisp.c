#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static horn_Obj* horn_parseLispSExp(horn_State* state);
static inline horn_Obj* horn_parseLispOrVal(horn_State* state);

static inline horn_Cmd horn_parseLispCmd(const horn_Token* tok){
	if(tok->type != HORN_TT_ID)
		return HORN_CMD_ERROR;

	char* text = horn_getTokenText(tok);
	horn_Cmd cmd = assoc_getOrDefault_horn_Cmd(g_horn_lispKW, text, HORN_CMD_ERROR);
	free(text);
	return cmd;
}

static horn_Obj* horn_parseLispSExp(horn_State* state){
	horn_Token tok;
	
	horn_Obj* obj = horn_newObj();
	if(!obj) return NULL;

	horn_next(state, &tok);

	if(tok.type != HORN_TT_ID){
		fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		return obj;
	}

	horn_Cmd cmd = horn_parseLispCmd(&tok);
	if(cmd == HORN_CMD_ERROR){
		int len = tok.end - tok.begin;
		fprintf(stderr, "ERROR: Unknown command: %*s\n", len, tok.begin);
		return obj;
	}

	horn_Obj* args = horn_parseLispOrVal(state);
	obj->as.args = args;
	while(args && tok.type != HORN_TT_RP){
		args->next = horn_parseLispOrVal(state);
		args = args->next;
	}

	horn_next(state, NULL);

	obj->cmd = cmd;
	return obj;
}

static inline horn_Obj* horn_parseLispOrVal(horn_State* state){
	horn_Token tok;
	horn_LH(state, &tok);
	switch(tok.type){
		case HORN_TT_ID:
		{
			horn_next(state, NULL);
			horn_Obj* obj = horn_newObj();
			if(!obj) return NULL;
			obj->cmd = HORN_CMD_ID;
			obj->as.text = horn_getTokenText(&tok); // TODO: PMA
			return obj;
		} break;
		case HORN_TT_OP_EQ:
		{
			horn_next(state, NULL);
			horn_next(state, &tok);
			if(tok.type != HORN_TT_ID){
				return NULL;
			}

			horn_Obj* obj = horn_newObj();
			if(!obj) return NULL;
			obj->cmd = HORN_CMD_ID;
			obj->as.text = horn_getTokenText(&tok); // TODO: PMA

			return obj;
		} break;
		case HORN_TT_INT:
		{
			horn_next(state, NULL);
			horn_Obj* v = horn_newObj();
			if(!v) return NULL;
			v->cmd = HORN_CMD_INTVAL;
			v->as.text = horn_getTokenText(&tok); // TODO: PMA
			return v;
		} break;
		case HORN_TT_LP:
			horn_next(state, NULL);
			return horn_parseLispSExp(state);
		case HORN_TT_RP: 
		case HORN_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		}
	}

	return NULL;
}

horn_Obj* horn_parseLisp(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	switch(tok.type){
		case HORN_TT_LP:
		{
			horn_next(state, NULL);
			horn_Obj* obj = horn_parseLispSExp(state);
			if(obj)
				obj->next = horn_parseLisp(state);
			return obj;
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
