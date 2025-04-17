#ifndef _TAURUS_H_
#define _TAURUS_H_

#define TRS_X_enum_TokenType \
	X(UKN)                   \
	X(EOF)                   \
	X(INT)                   \
	X(CHAR)                  \
	X(STR)                   \
	X(OP_EQ)                 \
	X(OP_PLUS)               \
	X(OP_MINUS)              \
	X(OP_MUL)                \
	X(OP_DIV)                \
	X(OP_MOD)                \
	X(OP_LOGICAL_NOT)        \
	X(OP_LOGICAL_EQ)         \
	X(OP_LOGICAL_NEQ)        \
	X(OP_LOGICAL_GT)         \
	X(OP_LOGICAL_LT)         \
	X(OP_LOGICAL_GTQ)        \
	X(OP_LOGICAL_LTQ)        \
	X(OP_LOGICAL_AND)        \
	X(OP_LOGICAL_OR)         \
	X(OP_BINARY_NOT)         \
	X(OP_BINARY_AND)         \
	X(OP_BINARY_XOR)         \
	X(OP_BINARY_OR)          \
	X(EOE)                   \
	X(COLON)                 \
	X(ID)                    \
	X(LP)                    \
	X(RP)                    \
	X(LB)                    \
	X(RB)                    \
	X(LSB)                   \
	X(RSB)                   \
	X(COMMA)

typedef enum{
#define X(Y) TRS_TT_##Y,
	TRS_X_enum_TokenType
#undef X
	TRS_TT_ENUM_END
} trs_TokenType;

static inline const char* trs_TokenTypeToString(trs_TokenType v){
#define X(Y) case TRS_TT_##Y: return #Y;
	switch(v){
		TRS_X_enum_TokenType
		default: return "???";
	}
#undef X
}

#endif //_TAURUS_H_
