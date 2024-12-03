#ifndef _HORN_H_
#define _HORN_H_

#include <toollib/assoc.h>

#define HORN_X_enum_TokenType \
	X(UKN)                    \
	X(EOF)                    \
	X(INT)                    \
	X(OP_EQ)                  \
	X(OP_PLUS)                \
	X(OP_MINUS)               \
	X(OP_MUL)                 \
	X(EOE)                    \
	X(ID)                     \
	X(LP)                     \
	X(RP)                     \
	X(LB)                     \
	X(RB)                     \
	X(LSB)                    \
	X(RSB)

#define HORN_X_enum_Cmd \
	X(ERROR)  			\
	X(ID)   			\
	X(INTVAL) 			\
	X(SET)    			\
	X(MINUS)            \
	X(ADD)    			\
	X(SUB)    			\
	X(MUL)              \
	X(SCOPE)

typedef enum{
#define X(Y) HORN_TT_##Y,
	HORN_X_enum_TokenType
#undef X
} horn_TokenType;

typedef enum{
#define X(Y) HORN_CMD_##Y,
	HORN_X_enum_Cmd
#undef X
} horn_Cmd;

typedef struct horn_Token horn_Token;
typedef struct horn_Obj horn_Obj;

struct horn_Token{
	horn_TokenType type;
	char* text;
};

struct horn_Obj{
	horn_Cmd cmd;
	char* text;
	horn_Obj* args;
	horn_Obj* next;
};

assoc_GEN_FOR_TYPE(horn_Cmd)

extern horn_Token g_horn_lookahead;
extern assoc g_horn_lispKW;
extern assoc g_horn_taurusKW;

bool horn_init(void);
void horn_terminate(void);

void horn_next(horn_Token* tok, const char* src);
void horn_LH(horn_Token* tok, const char* src);

horn_Obj* horn_parseLisp(const char* src);
horn_Obj* horn_parseTaurus(const char* src);

bool horn_analise(horn_Obj* ir);

static inline const char* horn_TokenTypeToString(horn_TokenType v){
#define X(Y) case HORN_TT_##Y: return #Y;
	switch(v){
		HORN_X_enum_TokenType
		default: return "???";
	}
#undef X
}

static inline const char* horn_CmdToString(horn_Cmd v){
#define X(Y) case HORN_CMD_##Y: return #Y;
	switch(v){
		HORN_X_enum_Cmd
		default: return "???";
	}
#undef X
}

#endif // _HORN_H_
