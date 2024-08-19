#include <trs/ast.h>

#include <stdlib.h>

#include <toollib/carea/carea.h>

AST* parsef(void* area, FILE* f){
	ASTBlock* root = carea_alloc(area, sizeof(ASTBlock));
	root->m_AST.m_Type = AST_BLOCK;
	root->m_AST.m_Next = NULL;
	root->m_Insides = NULL;

	Token t = nextToken(area, f);
	if(t.m_Type == TOK_ID){
		Token tt = nextToken(area, f);
		if(tt.m_Type == TOK_PRENTICE_L){
			// This is a function call
			ASTCall* call = carea_alloc(area, sizeof(ASTCall));
			call->m_AST.m_Type = AST_CALL;
			call->m_AST.m_Next = NULL;
			call->m_Name = t.m_Value;
			call->m_Args = NULL;

			Token ttt = nextToken(area, f);

			if(ttt.m_Type == TOK_STR){
				ASTStr* str = carea_alloc(area, sizeof(ASTStr));
				str->m_AST.m_Type = AST_STR;
				str->m_AST.m_Next = NULL;
				str->m_Value = ttt.m_Value;
				call->m_Args = (AST*)str;
				ttt = nextToken(area, f);
			}

			if(ttt.m_Type != TOK_PRENTICE_R){
				fprintf(stderr, "ERROR: Expected )\n");
				// TODO: Print location and whatcha got
				exit(1);
			}

			root->m_Insides = (AST*)call;
		} else{
			fprintf(stderr, "ERROR: Unexpected token%d: %s\n", tt.m_Type, tt.m_Value);
			// TODO: Print location
			exit(1);
		}
	} else{
		fprintf(stderr, "ERROR: Unexpected token: %s\n", t.m_Value);
		// TODO: Print location
		exit(1);
	}

	return (AST*)root;
}
