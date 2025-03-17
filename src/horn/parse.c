#include <trs/horn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static horn_Obj* horn_parseStm(horn_State* state);
static inline horn_Obj* horn_parseKeyword(horn_State* state, horn_Cmd cmd);
static inline horn_Obj* horn_parseExp(horn_State* state);
static inline horn_Obj* horn_parseLogicTerm(horn_State* state);
static inline horn_Obj* horn_parseCmp(horn_State* state);
static inline horn_Obj* horn_parseTerm(horn_State* state);
static inline horn_Obj* horn_parseFactor(horn_State* state);
static inline horn_Obj* horn_parseBinary(horn_State* state);
static inline horn_Obj* horn_parseUnary(horn_State* state);
static inline horn_Obj* horn_parsePrimary(horn_State* state);
static inline horn_Obj* horn_parseCall(horn_State* state, horn_Obj* id);

static inline horn_Obj* horn_makeUnary(horn_TokenType tt, horn_Obj* e);
static inline horn_Obj* horn_makeBi(horn_TokenType tt, horn_Obj* lhs, horn_Obj* rls);

static inline horn_Cmd horn_parseTaurusCmd(const horn_Token* tok){
	if(tok->type != HORN_TT_ID)
		return HORN_CMD_ERROR;

	char* text = horn_getTokenText(tok);
	horn_Cmd cmd = assoc_getOrDefault_horn_Cmd(g_horn_taurusKW, text, HORN_CMD_ERROR);
	free(text);
	return cmd;
}

horn_Obj* horn_parseTaurus(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	horn_Obj* root = horn_parseStm(state);

	horn_Obj* obj = root;
	while(tok.type != HORN_TT_EOF && tok.type != HORN_TT_UKN && obj){
		if(!obj->next)
			obj->next = horn_parseStm(state);
		
		obj = obj->next;

		horn_LH(state, NULL);
	}

	return root;
}

static horn_Obj* horn_parseStm(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	switch(tok.type){
		case HORN_TT_EOE:
		{
			horn_next(state, NULL);
			return horn_parseStm(state);
		} break;
		case HORN_TT_ID:
		{
			horn_Cmd cmd = horn_parseTaurusCmd(&tok);
			if(cmd != HORN_CMD_ERROR)
				return horn_parseKeyword(state, cmd);

			horn_next(state, NULL);

			horn_Obj* id = horn_newObj();
			if(!id) return NULL;
			id->cmd = HORN_CMD_ID;
			id->as.text = horn_getTokenText(&tok); //PMA

			horn_Obj* call = horn_parseCall(state, id);
			if(call) return call;

			horn_LH(state, &tok);
			switch(tok.type){
				case HORN_TT_OP_EQ:
				{
					horn_next(state, NULL);
					
					horn_Obj* stm = horn_newObj();
					if(!stm) return NULL;

					stm->cmd = HORN_CMD_SET;
					stm->as.args = id;
					id->next = horn_parseExp(state);

					return stm;
				} break;
				case HORN_TT_COLON:
				{
					horn_next(state, NULL);

					horn_Obj* stm = horn_newObj();
					if(!stm) return NULL;

					stm->cmd = HORN_CMD_LABEL;
					stm->as.args = id;

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
			horn_next(state, NULL);

			horn_Obj* scope = horn_newObj();
			if(!scope) return NULL;

			scope->cmd = HORN_CMD_SCOPE;
			scope->as.args = horn_parseStm(state);

			horn_LH(state, NULL);

			horn_Obj* obj = scope->as.args;
			while(tok.type != HORN_TT_EOF && tok.type != HORN_TT_UKN && obj){
				if(!obj->next)
					obj->next = horn_parseStm(state);
				
				obj = obj->next;

				horn_LH(state, NULL);
			}

			horn_LH(state, &tok);

			if(tok.type != HORN_TT_RB){
				// TODO: ERROR
				return NULL;
			}

			horn_next(state, NULL);

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

static inline horn_Obj* horn_parseKeyword(horn_State* state, horn_Cmd cmd){
	horn_Token tok = {0};
	horn_next(state, &tok);

	switch(cmd){
		case HORN_CMD_VAR:
		{
			horn_LH(state, NULL);

			// TODO: var a: T = ...
			// TODO: var (a, b, c) = (...)
			// TODO: var (a, b, c): T = (...)
			
			if(tok.type != HORN_TT_ID){
				// TODO: ERROR
				return NULL;
			}

			horn_next(state, NULL);

			horn_Obj* id = horn_newObj();
			if(!id) return NULL;
			id->cmd = HORN_CMD_ID;
			id->as.text = horn_getTokenText(&tok); //PMA

			horn_Obj* var = horn_newObj();
			if(!var) return NULL;
			var->cmd = cmd;
			var->as.args = id;

			// TODO: Types

			horn_LH(state, &tok);

			if(tok.type == HORN_TT_OP_EQ){
				horn_next(state, NULL);

				horn_Obj* exp = horn_parseExp(state);
				id->next = exp;
			}

			return var;
		} break;
		case HORN_CMD_GOTO:
		{
			horn_LH(state, &tok);

			if(tok.type != HORN_TT_ID){
				// TODO: ERROR
				return NULL;
			}

			horn_next(state, NULL);

			horn_Obj* id = horn_newObj();
			if(!id) return NULL;
			id->cmd = HORN_CMD_ID;
			id->as.text = horn_getTokenText(&tok); //PMA

			horn_Obj* gt = horn_newObj();
			if(!gt) return NULL;
			gt->cmd = cmd;
			gt->as.args = id;

			return gt;
		} break;
		default:
		{
			// TODO: ERROR
		}
	}

	return NULL;
}

static inline horn_Obj* horn_parseExp(horn_State* state){
	horn_Obj* exp = horn_parseLogicTerm(state);

	return exp;
}

static inline horn_Obj* horn_makeUnary(horn_TokenType tt, horn_Obj* e){
	if(!e){
		// TODO: ERROR
		return NULL;
	}

	horn_Obj* obj = horn_newObj();
	if(!obj) return NULL;

	switch(tt){
		case HORN_TT_OP_MINUS:
			obj->cmd = HORN_CMD_MINUS;
			break;
		case HORN_TT_OP_LOGICAL_NOT:
			obj->cmd = HORN_CMD_LNOT;
			break;
		case HORN_TT_OP_BINARY_NOT:
			obj->cmd = HORN_CMD_BNOT;
			break;
		default:
			obj->cmd = HORN_CMD_ERROR;
			break;
	}

	obj->as.args = e;

	return obj;
}

static inline horn_Obj* horn_makeBi(horn_TokenType tt, horn_Obj* lhs, horn_Obj* rhs){
	if(!lhs || !rhs){
		// TODO: ERROR
		return NULL;
	}
	
	horn_Obj* obj = horn_newObj();
	if(!obj) return NULL;

	switch(tt){
#define XCASE(OP, CMD) case HORN_TT_OP_##OP: obj->cmd = HORN_CMD_##CMD; break;
		XCASE(PLUS, ADD)
		XCASE(MINUS, SUB)
		XCASE(MUL, MUL)
		XCASE(DIV, DIV)
		XCASE(MOD, MOD)
		XCASE(LOGICAL_EQ, LEQ)
		XCASE(LOGICAL_NEQ, LNEQ)
		XCASE(LOGICAL_GT, LGT)
		XCASE(LOGICAL_LT, LLT)
		XCASE(LOGICAL_GTQ, LGTQ)
		XCASE(LOGICAL_LTQ, LLTQ)
		XCASE(LOGICAL_AND, LAND)
		XCASE(LOGICAL_OR, LOR)
		XCASE(BINARY_AND, BAND)
		XCASE(BINARY_XOR, BXOR)
		XCASE(BINARY_OR, BOR)
#undef XCASE
		default:
			obj->cmd = HORN_CMD_ERROR;
			break;
	}

	obj->as.args = lhs;
	lhs->next = rhs;

	return obj;
}

static inline horn_Obj* horn_parseLogicTerm(horn_State* state){
	horn_Obj* exp = horn_parseCmp(state);

	horn_Token tok = {0};
	horn_LH(state, &tok);

	while(tok.type == HORN_TT_OP_LOGICAL_AND || tok.type == HORN_TT_OP_LOGICAL_OR){
		horn_next(state, NULL);
		horn_Obj* rhs = horn_parseCmp(state);
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(state, &tok);
	}
	
	return exp;
}

static inline horn_Obj* horn_parseCmp(horn_State* state){
	horn_Obj* exp = horn_parseTerm(state);

	horn_Token tok = {0};
	horn_LH(state, &tok);

	while(
			tok.type == HORN_TT_OP_LOGICAL_EQ || tok.type == HORN_TT_OP_LOGICAL_NEQ || tok.type == HORN_TT_OP_LOGICAL_GT ||
			tok.type == HORN_TT_OP_LOGICAL_LT || tok.type == HORN_TT_OP_LOGICAL_GTQ || tok.type == HORN_TT_OP_LOGICAL_LTQ){
		horn_next(state, NULL);
		horn_Obj* rhs = horn_parseTerm(state);
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(state, &tok);
	}
	
	return exp;
}

static inline horn_Obj* horn_parseTerm(horn_State* state){
	horn_Obj* exp = horn_parseFactor(state);

	horn_Token tok = {0};
	horn_LH(state, &tok);

	while(tok.type == HORN_TT_OP_PLUS || tok.type == HORN_TT_OP_MINUS){
		horn_next(state, NULL);
		horn_Obj* rhs = horn_parseFactor(state);
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(state, &tok);
	}

	return exp;
}

static inline horn_Obj* horn_parseFactor(horn_State* state){
	horn_Obj* exp = horn_parseBinary(state);

	horn_Token tok = {0};
	horn_LH(state, &tok);

	while(tok.type == HORN_TT_OP_MUL || tok.type == HORN_TT_OP_DIV || tok.type == HORN_TT_OP_MOD){
		horn_next(state, NULL);
		horn_Obj* rhs = horn_parseBinary(state);
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(state, &tok);
	}

	return exp;
}

static inline horn_Obj* horn_parseBinary(horn_State* state){
	horn_Obj* exp = horn_parseUnary(state);

	horn_Token tok = {0};
	horn_LH(state, &tok);

	while(tok.type == HORN_TT_OP_BINARY_AND || tok.type == HORN_TT_OP_BINARY_XOR || tok.type == HORN_TT_OP_BINARY_OR){
		horn_next(state, NULL);
		horn_Obj* rhs = horn_parseUnary(state);
		exp = horn_makeBi(tok.type, exp, rhs);
		horn_LH(state, &tok);
	}
	
	return exp;
}

static inline horn_Obj* horn_parseUnary(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	if(tok.type == HORN_TT_OP_MINUS || tok.type == HORN_TT_OP_LOGICAL_NOT || tok.type == HORN_TT_OP_BINARY_NOT){
		horn_next(state, &tok);
		return horn_makeUnary(tok.type, horn_parseUnary(state));
	}
	
	return horn_parsePrimary(state);
}

static inline horn_Obj* horn_parsePrimary(horn_State* state){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	switch(tok.type){
		case HORN_TT_INT:
		{
			horn_next(state, NULL);

			horn_Obj* obj = horn_newObj();
			if(!obj) return NULL;

			obj->cmd = HORN_CMD_INTVAL;
			obj->as.text = horn_getTokenText(&tok); //PMA

			return obj;
		} break;
		case HORN_TT_ID:
		{
			horn_next(state, NULL);

			if(horn_parseTaurusCmd(&tok) != HORN_CMD_ERROR){
				// TODO: ERROR: Unexpected keyword
				return NULL;
			}

			horn_Obj* obj = horn_newObj();
			if(!obj) return NULL;
			obj->cmd = HORN_CMD_ID;
			obj->as.text = horn_getTokenText(&tok); //PMA

			horn_Obj* call = horn_parseCall(state, obj);
			if(call) return call;
			
			horn_Obj* get = horn_newObj();
			if(!get) return NULL;
			get->cmd = HORN_CMD_GET;
			get->as.args = obj;

			return get;
		} break;
		case HORN_TT_LP:
		{
			horn_next(state, NULL);
			horn_Obj* obj = horn_parseExp(state);

			horn_LH(state, &tok);
			if(tok.type != HORN_TT_RP){
				// TODO: ERROR
				return NULL;
			}

			horn_next(state, NULL);

			return obj;
		} break;
		default:
		{
			// TODO: ERORR
		}
	}

	return NULL;
}

static inline horn_Obj* horn_parseCall(horn_State* state, horn_Obj* id){
	horn_Token tok = {0};
	horn_LH(state, &tok);

	if(tok.type != HORN_TT_LP)
		return NULL;

	horn_next(state, NULL);

	horn_Obj* args = id;

	// TODO: Parse args
	horn_LH(state, &tok);
	if(tok.type != HORN_TT_RP){
		do{
			args->next = horn_parseExp(state);
			args = args->next;

			horn_next(state, &tok);
		} while(tok.type == HORN_TT_COMMA);
	} else{
		horn_next(state, NULL);
	}

	if(tok.type != HORN_TT_RP){
		// TODO: ERROR
		return NULL;
	}

	horn_Obj* call = horn_newObj();
	if(!call) return NULL;
	call->cmd = HORN_CMD_CALL;
	call->as.args = id;

	return call;
}

