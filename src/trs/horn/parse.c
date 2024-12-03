#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>

static inline horn_Obj* horn_alloc(void);

static horn_Obj* horn_parseStm(void);
static inline horn_Obj* horn_parseExp(void);
static inline horn_Obj* horn_parseLogicTerm(void);
static inline horn_Obj* horn_parseCmp(void);
static inline horn_Obj* horn_parseTerm(void);
static inline horn_Obj* horn_parseFactor(void);
static inline horn_Obj* horn_parseBinary(void);
static inline horn_Obj* horn_parseUnary(void);
static inline horn_Obj* horn_parsePrimary(void);

static inline horn_Obj* horn_makeBi(horn_TokenType tt, horn_Obj* lhs, horn_Obj* rls);

horn_Obj* horn_parseTaurus(const char* src){
	horn_Token tok = {0};
	horn_LH(&tok, src);

	horn_Obj* root = horn_parseStm();

	horn_Obj* obj = root;
	while(tok.type != HORN_TT_EOF && tok.type != HORN_TT_UKN && obj){
		obj->next = horn_parseStm();
		obj = obj->next;

		horn_LH(&tok, NULL);
	}

	return root;
}

static horn_Obj* horn_parseStm(void){
	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	switch(tok.type){
		case HORN_TT_EOE:
		{
			fprintf(stderr, "EOE\n");
			horn_next(&tok, NULL);
			return horn_parseStm();
		} break;
		case HORN_TT_ID:
		{
			horn_Obj* id = horn_parsePrimary();

			horn_LH(&tok, NULL);
			switch(tok.type){
				case HORN_TT_OP_EQ:
				{
					horn_next(&tok, NULL);
					
					horn_Obj* stm = horn_alloc();
					if(!stm) return NULL;

					stm->cmd = HORN_CMD_SET;
					stm->args = id;
					id->next = horn_parseExp();

					return stm;
				} break;
				default:
				{
					// TODO: ERROR
				}
			}

			return NULL;
		} break;
		case HORN_TT_LB:
		{
			horn_next(&tok, NULL);

			horn_Obj* scope = horn_alloc();
			if(!scope) return NULL;

			scope->cmd = HORN_CMD_SCOPE;
			scope->args = horn_parseStm();

			horn_LH(&tok, NULL);

			horn_Obj* obj = scope->args;
			while(tok.type != HORN_TT_EOF && tok.type != HORN_TT_UKN && obj){
				obj->next = horn_parseStm();
				obj = obj->next;

				horn_LH(&tok, NULL);
			}

			horn_LH(&tok, NULL);

			if(tok.type != HORN_TT_RB){
				// TODO: ERROR
				return NULL;
			}

			horn_next(&tok, NULL);

			return scope;
		} break;
		case HORN_TT_RB:
			break;
		default:
		{
			// TODO: ERROR
		}
	}

	return NULL;
}

static inline horn_Obj* horn_parseExp(void){
	horn_Obj* exp = horn_parseLogicTerm();

	return exp;
}

static inline horn_Obj* horn_makeBi(horn_TokenType tt, horn_Obj* lhs, horn_Obj* rhs){
	if(!lhs || !rhs){
		// TODO: ERROR
		return NULL;
	}
	
	horn_Obj* obj = horn_alloc();
	if(!obj) return NULL;

	switch(tt){
		case HORN_TT_OP_PLUS:
			obj->cmd = HORN_CMD_ADD;
			break;
		case HORN_TT_OP_MINUS:
			obj->cmd = HORN_CMD_SUB;
			break;
		case HORN_TT_OP_MUL:
			obj->cmd = HORN_CMD_MUL;
			break;
		default:
			obj->cmd = HORN_CMD_ERROR;
			break;
	}

	obj->args = lhs;
	lhs->next = rhs;

	return obj;
}

static inline horn_Obj* horn_parseLogicTerm(void){
	horn_Obj* exp = horn_parseCmp();

	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	// TODO: &&, ||
	
	return exp;
}

static inline horn_Obj* horn_parseCmp(void){
	horn_Obj* exp = horn_parseTerm();

	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	// TODO: ==, !=, >, <, >=, <=
	
	return exp;
}

static inline horn_Obj* horn_parseTerm(void){
	horn_Obj* exp = horn_parseFactor();

	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	while(
			tok.type == HORN_TT_OP_PLUS  ||
			tok.type == HORN_TT_OP_MINUS
		 ){
		horn_next(&tok, NULL);
		horn_Obj* rhs = horn_parseFactor();
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(&tok, NULL);
	}

	return exp;
}

static inline horn_Obj* horn_parseFactor(void){
	horn_Obj* exp = horn_parseBinary();

	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	while(tok.type == HORN_TT_OP_MUL){
		horn_next(&tok, NULL);
		horn_Obj* rhs = horn_parseBinary();
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(&tok, NULL);
	}

	return exp;
}

static inline horn_Obj* horn_parseBinary(void){
	horn_Obj* exp = horn_parseUnary();

	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	// TODO: &, |, ^
	
	return exp;
}

static inline horn_Obj* horn_parseUnary(void){
	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	// TODO: -, !, ~
	
	return horn_parsePrimary();
}

static inline horn_Obj* horn_parsePrimary(void){
	horn_Token tok = {0};
	horn_LH(&tok, NULL);

	switch(tok.type){
		case HORN_TT_INT:
		{
			horn_next(&tok, NULL);

			horn_Obj* obj = horn_alloc();
			if(!obj) return NULL;

			obj->cmd = HORN_CMD_INTVAL;
			obj->text = tok.text;
			tok.text = NULL;

			return obj;
		} break;
		case HORN_TT_ID:
		{
			horn_next(&tok, NULL);

			horn_Obj* obj = horn_alloc();
			if(!obj) return NULL;

			obj->cmd = HORN_CMD_ID;
			obj->text = tok.text;
			tok.text = NULL;

			return obj;
		} break;
		case HORN_TT_LP:
		{
			horn_next(&tok, NULL);
			horn_Obj* obj = horn_parseExp();

			horn_LH(&tok, NULL);
			if(tok.type != HORN_TT_RP){
				// TODO: ERROR
				return NULL;
			}

			horn_next(&tok, NULL);

			return obj;
		} break;
		default:
		{
			// TODO: ERORR
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
