#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <stddef.h>

typedef enum{
	TOK_ERR = 0,
	TOK_SEMICOLON,
	TOK_COMMA,
	TOK_PRENTICE_L,
	TOK_PRENTICE_R,
	TOK_ID,
	TOK_STR,
	TOK_EOF
} TokenType;

typedef enum {
	AST_ERR = 0,
	AST_BLOCK,
	AST_ID,
	AST_CALL,
	AST_STR
} ASTType;

typedef struct Token{
	TokenType m_Type;
	const char* m_Value;
	size_t m_LineNo;
} Token;

typedef struct AST AST;
struct AST{
	ASTType m_Type;
	AST* m_Next;
};

typedef struct ASTBlock{
	AST m_AST;
	AST* m_Insides;
} ASTBlock;

typedef struct ASTId{
	AST m_AST;
	const char* m_Name;
} ASTId;

typedef struct ASTCall{
	AST m_AST;
	const char* m_Name;
	AST* m_Args;
} ASTCall;

typedef struct ASTStr{
	AST m_AST;
	const char* m_Value;
} ASTStr;

extern Token g_Lookahed;
extern size_t g_LineNo;

Token nextToken(void* area, FILE* f);
AST* parsef(void* area, FILE* f);

static inline Token lookahedToken(void* area, FILE* f){
	if(g_Lookahed.m_Type != 0) 
		return g_Lookahed;

	g_Lookahed = nextToken(area, f);
	return g_Lookahed;
}

static inline void newASTId(ASTId* id, const char* name){
	id->m_AST.m_Type = AST_ID;
	id->m_AST.m_Next = NULL;
	id->m_Name = name;
}

static inline void newASTCall(ASTCall* call, const char* name, AST* args){
	call->m_AST.m_Type = AST_CALL;
	call->m_AST.m_Next = NULL;
	call->m_Name = name;
	call->m_Args = args;
}

static inline void newASTStr(ASTStr* str, const char* val){
	str->m_AST.m_Type = AST_STR;
	str->m_AST.m_Next = NULL;
	str->m_Value = val;
}

#endif // _AST_H_
