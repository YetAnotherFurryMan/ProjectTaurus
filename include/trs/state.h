#ifndef _STATE_H_
#define _STATE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum{
	TYPE_KIND_BASE,
	TYPE_KIND_FN
} TypeKind;

typedef struct Type{
	TypeKind m_Kind;
	size_t m_RefCount;
	bool m_Const;
} Type;

typedef enum{
	TYPE_BASE_BOOL = 1,
	TYPE_BASE_STR,
	TYPE_BASE_I8,
	TYPE_BASE_U8,
	TYPE_BASE_SIZE
} TypeBaseType;

typedef struct TypeBase{
	Type m_Type;
	TypeBaseType  m_Base;
} TypeBase;

typedef struct TypeFn{
	Type m_Type;
	Type* m_Return;
	Type** m_Params;
	size_t m_Arity;
	bool m_Variatic;
} TypeFn;

struct Def;
typedef struct Def Value;
typedef Value(*TrueFnPtr)(size_t, Value*);

typedef union{
	void* ptr;
	TrueFnPtr trueFn;
	bool b;
	int64_t i64;
	uint64_t u64;
} DefValue;

typedef struct Def{
	Type* m_Type;
	DefValue m_Value;
	bool m_True;
} Def;

typedef struct ValueStr{
	size_t m_Length;
	const char* m_Value;
} ValueStr;

typedef struct State{
	const char* m_Program;
	const char* m_FileName;
	size_t m_LineNo;
	size_t m_ColNo;
	Def* m_Defs;
	void* m_Area;
	void* m_Stack;
	size_t m_Errno;
	const char* m_ErrorArgs[2];
} State;

void freeDefValue(Type* t, DefValue* val);
void freeType(Type* t);
void freeState(State* s);

#endif // _STATE_H_
