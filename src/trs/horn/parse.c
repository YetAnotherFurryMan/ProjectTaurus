#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>

static inline horn_Obj* horn_alloc(void);

static horn_Obj* horn_parseExp(void);
static inline horn_Obj* horn_parseBi(horn_Cmd cmd, horn_Obj* a);

horn_Obj* horn_parseTaurus(const char* src){
	horn_Token tok = {0};
	horn_LH(&tok, src);

	horn_Obj* root = horn_parseExp();

	horn_Obj* obj = root;
	while(tok.type != HORN_TT_EOF && tok.type != HORN_TT_UKN && obj){
		obj->next = horn_parseExp();
		obj = obj->next;
		horn_LH(&tok, NULL);
	}

	return root;
}

static inline horn_Obj* horn_parseBi(horn_Cmd cmd, horn_Obj* a){
	// Consume
	horn_Token tok = {0};
	horn_next(&tok, NULL);

	horn_Obj* b = horn_parseExp();
			
	horn_Obj* obj = horn_alloc();
	if(!obj) return NULL;
	obj->cmd = cmd;
	
	if(b && b->cmd > HORN_CMD_SET && cmd > b->cmd){
		// (A op (B op C)) -> ((A op B) op C)
		obj->args = a;
		a->next = b->args;
		obj->next = a->next->next;
		a->next->next = NULL;
		b->args = obj;
		obj = b;
	} else{
		obj->args = a;
		a->next = b;
	}

	return obj;
}

static horn_Obj* horn_parseExp(void){
	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	switch(tok.type){
		case HORN_TT_ID:
		{
			horn_next(&tok, NULL);

			horn_Obj* id = horn_alloc();
			if(!id) return NULL;
			id->cmd = HORN_CMD_ID;
			id->text = tok.text;

			horn_LH(&tok, NULL);

			horn_Cmd cmd;
			switch(tok.type){
				case HORN_TT_OP_PLUS: cmd = HORN_CMD_ADD; break;
				case HORN_TT_OP_MUL: cmd = HORN_CMD_MUL; break;
				case HORN_TT_OP_EQ: cmd = HORN_CMD_SET; break;
				case HORN_TT_EOE: horn_next(&tok, NULL); return id; // Consume
				default: return id;
			}
	
			horn_Obj* obj = horn_parseBi(cmd, id);

			return obj;
		} break;
		case HORN_TT_INT:
		{
			horn_next(&tok, NULL);

			horn_Obj* v = horn_alloc();
			if(!v) return NULL;
			v->cmd = HORN_CMD_INTVAL;
			v->text = tok.text;

			horn_LH(&tok, NULL);

			horn_Cmd cmd;
			switch(tok.type){
				case HORN_TT_OP_PLUS: cmd = HORN_CMD_ADD; break;
				case HORN_TT_OP_MUL: cmd = HORN_CMD_MUL; break;
				case HORN_TT_OP_EQ: v->cmd = HORN_CMD_ERROR; return v; // [int] = ... is invalid
				case HORN_TT_EOE: horn_next(&tok, NULL); return v; // Consume
				default: return v;
			}

			return horn_parseBi(cmd, v);
		} break;
		case HORN_TT_LB:
		{
			horn_next(&tok, NULL);

			horn_Obj* scope = horn_alloc();
			if(!scope) return NULL;
			scope->cmd = HORN_CMD_SCOPE;
			scope->text = tok.text;

			scope->args = horn_parseExp();

			horn_LH(&tok, NULL);
			if(tok.type != HORN_TT_RB){
				scope->cmd = HORN_CMD_ERROR;
				return scope;
			}

			// Consume
			horn_next(&tok, NULL);

			scope->next = horn_parseExp();

			return scope;
		} break;
		case HORN_TT_LP:
		{
			horn_next(&tok, NULL);

			horn_Obj* obj = horn_parseExp();
			if(!obj) return NULL;

			horn_LH(&tok, NULL);
			if(tok.type != HORN_TT_RP){
				return NULL;
			}

			horn_next(&tok, NULL);

			horn_LH(&tok, NULL);

			horn_Cmd cmd;
			switch(tok.type){
				case HORN_TT_OP_PLUS: cmd = HORN_CMD_ADD; break;
				case HORN_TT_OP_MUL: cmd = HORN_CMD_MUL; break;
				case HORN_TT_OP_EQ: obj->cmd = HORN_CMD_ERROR; return obj; // (...) = ... is invalid
				case HORN_TT_EOE: horn_next(&tok, NULL); return obj; // Consume
				default: return obj;
			}

			horn_Obj* op = horn_alloc();
			if(!op) return NULL;

			horn_next(&tok, NULL);

			op->cmd = cmd;
			op->args = obj;
			obj->next = horn_parseExp();

			return op;
		} break;
		case HORN_TT_RP:
		case HORN_TT_RB:
			break;
		case HORN_TT_EOE: 
		case HORN_TT_EOF:
			horn_next(&tok, NULL);
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}

static inline horn_Obj* horn_alloc(void){
	horn_Obj* obj = malloc(sizeof(horn_Obj));
	obj->cmd = HORN_CMD_ERROR;
	obj->text = NULL;
	obj->args = NULL;
	obj->next = NULL;
	return obj;
}
