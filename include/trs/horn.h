#ifndef _HORN_H_
#define _HORN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <toollib/pgm.h>
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
	X(CHRVAL) 			\
	X(STRVAL) 			\
	X(GET)    			\
	X(GETTYPE)          \
	X(SET)    			\
	X(LIST)             \
	X(NIL)              \
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
	X(RET)              \
	X(VAR)              \
	X(FN)               \
	X(PRINT)

typedef enum{
#define X(Y) HORN_TT_##Y,
	HORN_X_enum_TokenType
#undef X
	HORN_TT_ENUM_END
} horn_TokenType;

typedef enum{
#define X(Y) HORN_CMD_##Y,
	HORN_X_enum_Cmd
#undef X
} horn_Cmd;

typedef union horn_ObjAs horn_ObjAs;
typedef struct horn_Instance horn_Instance;
typedef struct horn_State horn_State;
typedef struct horn_Token horn_Token;
typedef struct horn_Obj horn_Obj;

union horn_ObjAs{
	char* text;
	horn_Obj* args;
};

struct horn_Instance{
	assoc kw_map;
	pgm alloc;
	horn_Obj* src;
	horn_Obj* src_end;
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

void horn_next(horn_State* state, horn_Token* token);
void horn_LH(horn_State* state, horn_Token* token);

bool horn_init(horn_Instance* inst);
bool horn_freeInstance(horn_Instance* inst);

void horn_resetState(horn_State* state, const char* src);

bool horn_load(horn_Instance* inst, const char* src);
bool horn_emit(FILE* out, horn_Instance* inst);

// TODO: horn_Program* horn_makeProgram(horn_Obj* src);

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

static inline char* horn_getTokenText(pgm* pgm, const horn_Token* tok){
	size_t len = tok->end - tok->begin;
	char* text = pgm_allocTN(pgm, char, len + 1);
	
	if(!text)
		return NULL;

	memcpy(text, tok->begin, len);
	text[len] = 0;
	return text;
}

static inline char* horn_getTokenTextTmp(const horn_Token* tok){
	size_t len = tok->end - tok->begin;
	char* text = (char*) malloc(len + 1);
	
	if(!text)
		return NULL;

	memcpy(text, tok->begin, len);
	text[len] = 0;
	return text;
}

static inline horn_Obj* horn_newObj(pgm* pgm){
	horn_Obj* obj = pgm_allocT(pgm, horn_Obj);

	if(!obj)
		return NULL;

	obj->cmd = HORN_CMD_ERROR;
	obj->as.text = NULL;
	obj->next = NULL;

	return obj;
}

#endif // _HORN_H_
