#ifndef _PARSER_H_
#define _PARSER_H_

#include <toollib/assoc.h>

#include "IR.h"

assoc_GEN_FOR_TYPE(trs_IRCmd)

#define TRS_PARSER_X_enum_TokenType \
	X(UKN)                          \
	X(EOF)                          \
	X(INT)                          \
	X(OP_EQ)                        \
	X(OP_PLUS)                      \
	X(OP_MUL)                       \
	X(EOE)                          \
	X(ID)                           \
	X(LB)                           \
	X(RB)

typedef enum{
#define X(Y) TRS_TT_##Y,
	TRS_PARSER_X_enum_TokenType
#undef X
} trs_TokenType;

typedef struct trs_Token trs_Token;

struct trs_Token{
	trs_TokenType type;
	char* text;
};

extern trs_Token g_trs_lexLookahead;
extern assoc g_trs_lexLispKW;
extern assoc g_trs_lexTaurusKW;

bool trs_lexInit(void);
void trs_lexTerminate(void);

void trs_lexNext(trs_Token* tok, const char* src);
void trs_lexLH(trs_Token* tok, const char* src);

trs_IR* trs_parse(const char* src);
trs_IR* trs_parseLisp(const char* src);

static inline const char* trs_TokenTypeToString(trs_TokenType v){
#define X(Y) case TRS_TT_##Y: return #Y;
	switch(v){
		TRS_PARSER_X_enum_TokenType
		default: return "???";
	}
#undef X
}

#endif // _PARSER_H_
