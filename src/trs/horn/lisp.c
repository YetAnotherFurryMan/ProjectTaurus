#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline horn_Obj* horn_parseLispOrVal();
static inline horn_Obj* horn_alloc();

static inline horn_Cmd horn_parseLispCmd(const horn_Token* tok){
	if(tok->type != HORN_TT_ID)
		return HORN_CMD_ERROR;
	return assoc_getOrDefault_horn_Cmd(g_horn_lispKW, tok->text, HORN_CMD_ERROR);
}

horn_Obj* horn_parseLispSExp(){
	horn_Token tok;

	horn_Obj* obj = horn_alloc();
	if(!obj) return NULL;

	horn_next(&tok, NULL);

	horn_Cmd cmd = horn_parseLispCmd(&tok);
	if(cmd == HORN_CMD_ERROR){
		if(tok.type != HORN_TT_ID)
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		else
			fprintf(stderr, "ERROR: Unknown command: %s\n", tok.text);
		return obj;
	}

	horn_Obj* args = horn_parseLispOrVal();
	obj->args = args;
	while(args && tok.type != HORN_TT_RP){
		args->next = horn_parseLispOrVal();
		args = args->next;
	}

	horn_next(&tok, NULL);

	obj->cmd = cmd;
	return obj;
}

static inline horn_Obj* horn_parseLispOrVal(){
	horn_Token tok;
	horn_LH(&tok, NULL);
	switch(tok.type){
		case HORN_TT_ID:
		{
			horn_next(&tok, NULL);
			horn_Obj* obj = horn_alloc();
			if(!obj) return NULL;
			obj->cmd = HORN_CMD_ID;
			obj->text = tok.text;
			return obj;
		} break;
		case HORN_TT_INT:
		{
			horn_next(&tok, NULL);
			horn_Obj* v = horn_alloc();
			if(!v) return NULL;
			v->cmd = HORN_CMD_INTVAL;
			v->text = tok.text;
			return v;
		} break;
		case HORN_TT_LP:
			horn_next(&tok, NULL);
			return horn_parseLispSExp();
		case HORN_TT_RP: 
		case HORN_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}

horn_Obj* horn_parseLisp(const char* src){
	horn_Token tok = {0};
	horn_LH(&tok, src);

	switch(tok.type){
		case HORN_TT_LP:
		{
			horn_next(&tok, NULL);
			horn_Obj* obj = horn_parseLispSExp();
			if(obj)
				obj->next = horn_parseLisp(NULL);
			return obj;
		} break;
		case HORN_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}

static inline horn_Obj* horn_alloc(){
	horn_Obj* obj = malloc(sizeof(horn_Obj));
	obj->cmd = HORN_CMD_ERROR;
	obj->text = NULL;
	obj->args = NULL;
	obj->next = NULL;
	return obj;
}
