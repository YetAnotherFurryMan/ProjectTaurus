#ifndef _HORN_H_
#define _HORN_H_

#include <stdlib.h>
#include <string.h>

#include <toollib/assoc.h>

#define HORN_X_enum_TokenType \
	X(UKN)                    \
	X(EOF)                    \
	X(INT)                    \
	X(CHAR)                   \
	X(STR)                    \
	X(OP_EQ)                  \
	X(OP_PLUS)                \
	X(OP_MINUS)               \
	X(OP_MUL)                 \
	X(OP_DIV)                 \
	X(OP_MOD)                 \
	X(OP_LOGICAL_NOT)         \
	X(OP_LOGICAL_EQ)          \
	X(OP_LOGICAL_NEQ)         \
	X(OP_LOGICAL_GT)          \
	X(OP_LOGICAL_LT)          \
	X(OP_LOGICAL_GTQ)         \
	X(OP_LOGICAL_LTQ)         \
	X(OP_LOGICAL_AND)         \
	X(OP_LOGICAL_OR)          \
	X(OP_BINARY_NOT)          \
	X(OP_BINARY_AND)          \
	X(OP_BINARY_XOR)          \
	X(OP_BINARY_OR)           \
	X(EOE)                    \
	X(COLON)                  \
	X(ID)                     \
	X(LP)                     \
	X(RP)                     \
	X(LB)                     \
	X(RB)                     \
	X(LSB)                    \
	X(RSB)                    \
	X(COMMA)

#define HORN_X_enum_Cmd \
	X(ERROR)  			\
	X(ID)               \
	X(INTVAL) 			\
	X(GET)    			\
	X(SET)    			\
	X(MINUS)            \
	X(ADD)    			\
	X(SUB)    			\
	X(MUL)              \
	X(DIV)              \
	X(MOD)              \
	X(LNOT)             \
	X(LEQ)              \
	X(LNEQ)             \
	X(LGT)              \
	X(LLT)              \
	X(LGTQ)             \
	X(LLTQ)             \
	X(LAND)             \
	X(LOR)              \
	X(BNOT)             \
	X(BAND)             \
	X(BXOR)             \
	X(BOR)              \
	X(SCOPE)            \
	X(LABEL)            \
	X(GOTO)             \
	X(CALL) 			\
	X(VAR)

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

typedef union horn_ObjAs horn_ObjAs;
typedef struct horn_State horn_State;
typedef struct horn_Token horn_Token;
typedef struct horn_Obj horn_Obj;

union horn_ObjAs{
	char* text;
	horn_Obj* args;
};

struct horn_Token{
	horn_TokenType type;
	const char* begin;
	const char* end;
};

struct horn_Obj{
	horn_Cmd cmd;
	horn_ObjAs as;
	horn_Obj* next;
};

struct horn_State{
	horn_Token lookahead;
	const char* src;
	const char* cursor;
	size_t row;
	size_t column;
};

assoc_GEN_FOR_TYPE(horn_Cmd)

extern assoc g_horn_lispKW;
extern assoc g_horn_taurusKW;

bool horn_init(void);
void horn_resetState(horn_State* state, const char* src);
void horn_terminate(void);

void horn_next(horn_State* state, horn_Token* token);
void horn_LH(horn_State* state, horn_Token* token);

horn_Obj* horn_parseLisp(horn_State* state);
horn_Obj* horn_parseTaurus(horn_State* state);

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

static inline char* horn_getTokenText(const horn_Token* tok){
	size_t len = tok->end - tok->begin;
	char* text = (char*) malloc(len + 1);
	
	if(!text)
		return NULL;

	memcpy(text, tok->begin, len);
	text[len] = 0;
	return text;
}

static inline horn_Obj* horn_newObj(void){
	horn_Obj* obj = (horn_Obj*) malloc(sizeof(horn_Obj));

	if(!obj)
		return NULL;

	obj->cmd = HORN_CMD_ERROR;
	obj->as.text = NULL;
	obj->next = NULL;

	return obj;
}

#endif // _HORN_H_
