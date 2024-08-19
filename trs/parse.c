#include <trs/ast.h>

#include <stdlib.h>

#include <toollib/carea/carea.h>

static AST* parseExp(void* area, FILE* f);

static ASTCall* parseCall(void* area, FILE* f){
	ASTCall* call = carea_alloc(area, sizeof(ASTCall));
	AST* args = NULL;

	Token tok = nextToken(area, f);
	if(tok.m_Type != TOK_PRENTICE_L){
		call->m_AST.m_Type = AST_ERR;
		return call;
	}

	tok = lookahedToken(area, f);
	if(tok.m_Type != TOK_PRENTICE_R){
		args = parseExp(area, f);
		AST* ast = args;
		tok = nextToken(area, f);
		while(tok.m_Type == TOK_COMMA){
			AST* a = parseExp(area, f);
			ast->m_Next = a;
			ast = a;
			tok = nextToken(area, f);
		}
	} else{
		// Unset lookahed without calling nextToken
		g_Lookahed.m_Type = 0;
	}

	if(tok.m_Type != TOK_PRENTICE_R){
		call->m_AST.m_Type = AST_ERR;
		return call;
	}

	newASTCall(call, NULL, args);
	return call;
}

static AST* parseExp(void* area, FILE* f){
	Token tok = nextToken(area, f);
	switch(tok.m_Type){
		case TOK_EOF: return NULL;
		case TOK_SEMICOLON: return parseExp(area, f);
		case TOK_STR: 
		{
			ASTStr* str = carea_alloc(area, sizeof(ASTStr));
			newASTStr(str, tok.m_Value);
			return (AST*)str;
		} break;
		case TOK_ID:
		{
			Token lh = lookahedToken(area, f);
			if(lh.m_Type == TOK_PRENTICE_L){
				ASTCall* call = parseCall(area, f);
				call->m_Name = tok.m_Value;
				return (AST*)call;
			} else{
				ASTId* id = carea_alloc(area, sizeof(ASTId));
				newASTId(id, tok.m_Value);
				return (AST*)id;
			}
		} break;
		default: break;
	}

	AST* err = carea_alloc(area, sizeof(AST));
	err->m_Type = AST_ERR;
	err->m_Next = NULL;
	return err;
}

AST* parsef(void* area, FILE* f){
	ASTBlock* root = carea_alloc(area, sizeof(ASTBlock));
	root->m_AST.m_Type = AST_BLOCK;
	root->m_AST.m_Next = NULL;
	root->m_Insides = parseExp(area, f);

	AST* ast = root->m_Insides;
	while(ast && ast->m_Type != AST_ERR){
		AST* a = parseExp(area, f);
		ast->m_Next = a;
		ast = a;
	} 

	return (AST*)root;
}
