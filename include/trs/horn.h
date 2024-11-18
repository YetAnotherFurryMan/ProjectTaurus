#ifndef _HORN_H_
#define _HORN_H_

#include <toollib/assoc.h>

#include "IR.h"

assoc_GEN_FOR_TYPE(trs_IRCmd)

#define HORN_X_enum_TokenType \
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
	HORN_X_enum_TokenType
#undef X
} horn_TokenType;

typedef struct horn_Token horn_Token;

struct horn_Token{
	horn_TokenType type;
	char* text;
};

extern horn_Token g_horn_lookahead;
extern assoc g_horn_lispKW;
extern assoc g_horn_taurusKW;

bool horn_init(void);
void horn_terminate(void);

void horn_next(horn_Token* tok, const char* src);
void horn_LH(horn_Token* tok, const char* src);

trs_IR* horn_parseLisp(const char* src);
trs_IR* horn_parseTaurus(const char* src);

static inline const char* horn_TokenTypeToString(horn_TokenType v){
#define X(Y) case TRS_TT_##Y: return #Y;
	switch(v){
		HORN_X_enum_TokenType
		default: return "???";
	}
#undef X
}

#endif // _HORN_H_
