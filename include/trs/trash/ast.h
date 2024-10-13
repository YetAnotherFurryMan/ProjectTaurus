#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "state.h"

typedef enum{
	TOK_ERR = 0,
	TOK_SEMICOLON,
	TOK_COMMA,
	TOK_PRENTICE_L,
	TOK_PRENTICE_R,
	TOK_ID,
	TOK_STR,
	TOK_NUM,
	TOK_OP_EQ,
	TOK_EOF
} TokenType;

typedef enum {
	AST_ROOT = 0,
	AST_BLOCK,
	AST_ID,
	AST_CALL,
	AST_VAR,
	AST_STR,
	AST_NUM
} ASTType;

typedef struct Token{
	TokenType m_Type;
	const char* m_Value;
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
	size_t m_Argc;
	AST* m_Argv;
} ASTCall;

typedef struct ASTVar{
	AST m_AST;
	const char* m_Name;
	AST* m_Value;
} ASTVar;

typedef struct ASTStr{
	AST m_AST;
	const char* m_Value;
} ASTStr;

typedef struct ASTNum{
	AST m_AST;
	uint64_t m_Value;
} ASTNum;

extern Token g_Lookahed;

Token nextToken(State* s, FILE* f);
AST* parsef(State* s, FILE* f);

static inline Token lookahedToken(State* s, FILE* f){
	if(g_Lookahed.m_Type != 0) 
		return g_Lookahed;

	g_Lookahed = nextToken(s, f);
	return g_Lookahed;
}

static inline void newASTId(State* s, ASTId* id, const char* name){
	if(!id){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return;
	}

	id->m_AST.m_Type = AST_ID;
	id->m_AST.m_Next = NULL;
	id->m_Name = name;
}

static inline void newASTCall(State* s, ASTCall* call, const char* name, size_t argc, AST* argv){
	if(!call){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return;
	}

	call->m_AST.m_Type = AST_CALL;
	call->m_AST.m_Next = NULL;
	call->m_Name = name;
	call->m_Argc = argc;
	call->m_Argv = argv;
}

static inline void newASTVar(State* s, ASTVar* var, const char* name, AST* val){
	if(!var){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return;
	}

	var->m_AST.m_Type = AST_VAR;
	var->m_AST.m_Next = NULL;
	var->m_Name = name;
	var->m_Value = val;
}

static inline void newASTStr(State* s, ASTStr* str, const char* val){
	if(!str){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return;
	}

	str->m_AST.m_Type = AST_STR;
	str->m_AST.m_Next = NULL;
	str->m_Value = val;
}

static inline void newASTNum(State* s, ASTNum* num, uint64_t val){
	if(!num){
		s->m_Errno = FE_OUT_OF_MEMORY;
		return;
	}

	num->m_AST.m_Type = AST_NUM;
	num->m_AST.m_Next = NULL;
	num->m_Value = val;
}

#endif // _AST_H_
