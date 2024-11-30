#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>

static inline horn_Obj* horn_alloc();

horn_Obj* horn_parseTaurus(const char* src){
	horn_Token tok = {0};
	horn_LH(&tok, src);

	switch(tok.type){
		case HORN_TT_ID:
		{
			horn_next(&tok, NULL);

			horn_Obj* obj = horn_alloc();
			if(!obj) return NULL;
			obj->cmd = HORN_CMD_ID;
			obj->text = tok.text;

			horn_LH(&tok, NULL);

			switch(tok.type){
				case HORN_TT_OP_PLUS: obj->cmd = HORN_CMD_ADD; break;
				case HORN_TT_OP_MUL: obj->cmd = HORN_CMD_MUL; break;
				case HORN_TT_OP_EQ: obj->cmd = HORN_CMD_SET; break;
				case HORN_TT_EOE: horn_next(&tok, NULL); return obj; // Consume
				default: return obj;
			}

			// Consume
			horn_next(&tok, NULL);
			
			if(obj->cmd == HORN_CMD_SET){
				obj->args = horn_parseTaurus(NULL);
				obj->next = horn_parseTaurus(NULL);
			} else{
				horn_Obj* arg = horn_alloc();
				if(!arg) return NULL;
				arg->cmd = HORN_CMD_ID;
				arg->text = obj->text;
				arg->next = horn_parseTaurus(NULL);

				obj->args = arg;
				obj->text = NULL;
			}

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

			// Consume
			horn_next(&tok, NULL);

			horn_Obj* obj = horn_alloc();
			if(!obj) return NULL;
			obj->cmd = cmd;
			obj->args = v;

			v->next = horn_parseTaurus(NULL);

			return obj;
		} break;
		case HORN_TT_LB:
		{
			horn_next(&tok, NULL);

			horn_Obj* scope = horn_alloc();
			if(!scope) return NULL;
			scope->cmd = HORN_CMD_SCOPE;
			scope->text = tok.text;

			scope->args = horn_parseTaurus(NULL);

			horn_LH(&tok, NULL);
			if(tok.type != HORN_TT_RB){
				scope->cmd = HORN_CMD_ERROR;
				return scope;
			}

			// Consume
			horn_next(&tok, NULL);

			scope->next = horn_parseTaurus(NULL);

			return scope;
		} break;
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

static inline horn_Obj* horn_alloc(){
	horn_Obj* obj = malloc(sizeof(horn_Obj));
	obj->cmd = HORN_CMD_ERROR;
	obj->text = NULL;
	obj->args = NULL;
	obj->next = NULL;
	return obj;
}
