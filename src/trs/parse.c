#include <trs/ast.h>

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <trs/error.h>

#include <toollib/carea/carea.h>

static AST* parseExp(State* s, FILE* f);

static ASTCall* parseCall(State* s, FILE* f){
	void* area = s->m_Area;

	size_t argc = 0;
	AST* argv = NULL;

	Token tok = nextToken(s, f);
	if(tok.m_Type != TOK_PRENTICE_L){
		s->m_Errno = E_EXCEPTED;
		s->m_ErrorArgs[0] = "(";
		s->m_ErrorArgs[1] = tok.m_Value;
		return NULL;
	}

	tok = lookahedToken(s, f);
	if(tok.m_Type != TOK_PRENTICE_R){
		argv = parseExp(s, f);
		argc++;

		AST* ast = argv;
		tok = nextToken(s, f);
		while(tok.m_Type == TOK_COMMA){
			AST* a = parseExp(s, f);
			ast->m_Next = a;
			ast = a;
			tok = nextToken(s, f);

			argc++;
		}
	} else{
		// Unset lookahed without calling nextToken
		g_Lookahed.m_Type = 0;
	}

	if(tok.m_Type != TOK_PRENTICE_R){
		s->m_Errno = E_EXCEPTED;
		s->m_ErrorArgs[0] = ")";
		s->m_ErrorArgs[1] = tok.m_Value;
		return NULL;
	}

	ASTCall* call = carea_alloc(area, sizeof(ASTCall));
	newASTCall(s, call, NULL, argc, argv);
	return call;
}

static ASTVar* parseVar(State* s, FILE* f){
	void* area = s->m_Area;

	const char* name = 0;
	AST* val = 0;

	Token tok = nextToken(s, f);
	if(tok.m_Type != TOK_ID){
		s->m_Errno = E_EXCEPTED_NAME;
		s->m_ErrorArgs[0] = "identifier";
		s->m_ErrorArgs[1] = tok.m_Value;
		return NULL;
	}

	name = tok.m_Value;

	tok = lookahedToken(s, f);
	if(tok.m_Type == TOK_OP_EQ){
		// Unset lookahed without calling nextToken
		g_Lookahed.m_Type = 0;

		val = parseExp(s, f);
	}

	ASTVar* var = carea_alloc(area, sizeof(ASTVar));
	newASTVar(s, var, name, val);
	return var;
}

static AST* parseExp(State* s, FILE* f){
	void* area = s->m_Area;

	Token tok = nextToken(s, f);
	switch(tok.m_Type){
		case TOK_EOF: return NULL;
		case TOK_SEMICOLON: return parseExp(s, f);
		case TOK_STR: 
		{
			ASTStr* str = carea_alloc(area, sizeof(ASTStr));
			newASTStr(s, str, tok.m_Value);
			return (AST*)str;
		} break;
		case TOK_NUM: 
		{
			uint64_t val = 0;
			sscanf(tok.m_Value, "%" SCNu64, &val);
			
			ASTNum* num = carea_alloc(area, sizeof(ASTNum));
			newASTNum(s, num, val);
			return (AST*)num;
		} break;
		case TOK_ID:
		{
			if(strcmp(tok.m_Value, "var") == 0){
				ASTVar* var = parseVar(s, f);
				return (AST*)var;
			}

			Token lh = lookahedToken(s, f);
			if(lh.m_Type == TOK_PRENTICE_L){
				ASTCall* call = parseCall(s, f);
				if(!call)
					return NULL;

				call->m_Name = tok.m_Value;
				return (AST*)call;
			} else{
				ASTId* id = carea_alloc(area, sizeof(ASTId));
				newASTId(s, id, tok.m_Value);
				return (AST*)id;
			}
		} break;
		default: break;
	}

	s->m_Errno = E_UNEXPECTED_TOKEN;
	s->m_ErrorArgs[0] = tok.m_Value;
	return NULL;
}

AST* parsef(State* s, FILE* f){
	void* area = s->m_Area;

	AST* root = carea_alloc(area, sizeof(AST));
	if(!root){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return NULL;
	}
	
	root->m_Type = AST_ROOT;

	AST* ast = root;
	while(ast){
		AST* a = parseExp(s, f);
		ast->m_Next = a;
		ast = a;
	} 

	return (AST*)root;
}
