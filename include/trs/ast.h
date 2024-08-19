#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <stddef.h>

typedef enum{
	TOK_ERR = 0,
	TOK_ID,
	TOK_STR,
	TOK_PRENTICE_L,
	TOK_PRENTICE_R,
	TOK_SEMICOLON,
	TOK_EOF
} TokenType;

typedef struct Token{
	TokenType m_Type;
	const char* m_Value;
} Token;

typedef enum {
	AST_ERROR = 0,
	AST_BLOCK,
	AST_CALL,
	AST_STR
} ASTType;

typedef struct AST AST;
struct AST{
	ASTType m_Type;
	AST* m_Next;
};

typedef struct ASTBlock{
	AST m_AST;
	AST* m_Insides;
} ASTBlock;

typedef struct ASTCall{
	AST m_AST;
	const char* m_Name;
	AST* m_Args;
} ASTCall;

typedef struct ASTStr{
	AST m_AST;
	size_t m_Length;
	const char* m_Value;
} ASTStr;

Token nextToken(void* area, FILE* f);
AST* parsef(void* area, FILE* f);

#endif // _AST_H_
