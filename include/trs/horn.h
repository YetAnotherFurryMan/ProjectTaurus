#ifndef _HORN_H_
#define _HORN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <toollib/pgm.h>
#include <toollib/assoc.h>

#include <trs/utils/lex_state.h>

#define HORN_X_enum_TokenType \
	X(UKN)                    \
	X(EOF)                    \
	X(INT)                    \
	X(STR)                    \
	X(ID)                     \
	X(OBJ)                    \
	X(LP)                     \
	X(RP)                     \
	X(LB)                     \
	X(RB)                     \
	X(LSB)                    \
	X(RSB)

#define HORN_X_enum_Cmd \
	X(ERROR)  			\
	X(ID)               \
	X(INTVAL) 			\
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
	X(RETURN)           \
	X(VARIABLE)         \
	X(FUNCTION)         \
	X(PROCEDURE)        \
	X(PRINT)            \
	X(PUTCHAR)

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

struct horn_Obj{
	horn_Cmd cmd;
	horn_ObjAs as;
	horn_Obj* next;
};

assoc_GEN_FOR_TYPE(horn_Cmd)

void horn_next(utils_State* state, utils_Token* token);
void horn_LH(utils_State* state, utils_Token* token);

bool horn_init(horn_Instance* inst);
bool horn_freeInstance(horn_Instance* inst);

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
