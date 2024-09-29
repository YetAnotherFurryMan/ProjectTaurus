#include <trs/std.h>

#include <stdlib.h>

#include <toollib/cvec/cvec.h>

typedef struct{
	const char* m_Name;
	TrueFnPtr m_Ptr;
	union{
		TypeBaseType base;
		int zero;
	} m_Return;
} NameNPtr;

static NameNPtr names[] = {
	{"print", std_print_impl, {0}},
	{"println", std_println_impl, {0}},
	{"scanln", std_scanln_impl, {TYPE_BASE_STR}}
};

#define NAMES_COUNT (sizeof(names) / sizeof(NameNPtr))

void stdLoad(State* s){
	{
		TypeBase* type = malloc(sizeof(TypeBase));
		type->m_Type.m_Kind = TYPE_KIND_BASE;
		type->m_Type.m_RefCount = 0;
		type->m_Type.m_Const = true;
		type->m_Base = TYPE_BASE_I8;

		Def def = {0};
		def.m_Type = (Type*)type;
		def.m_Value.i64 = 0;
		def.m_True = true;
		cassoc_set(s->m_Defs, "null", &def);
	}
	for(size_t i = 0; i < NAMES_COUNT; i++){
		TypeBase* ret = NULL;
		if(names[i].m_Return.zero > 0){
			ret = malloc(sizeof(TypeBase));
			ret->m_Type.m_Kind = TYPE_KIND_BASE;
			ret->m_Type.m_RefCount = 0;
			ret->m_Type.m_Const = true;
			ret->m_Base = names[i].m_Return.base;
		}

		TypeFn* type = malloc(sizeof(TypeFn));
		type->m_Type.m_Kind = TYPE_KIND_FN;
		type->m_Type.m_RefCount = 0;
		type->m_Type.m_Const = true;
		type->m_Return = (Type*)ret;
		type->m_Params = NULL;
		type->m_Arity = 0;
		type->m_Variatic = true;

		Def def = {0}; 
		def.m_Type = (Type*)type;
		def.m_Value.trueFn = names[i].m_Ptr;
		def.m_True = true;
		cassoc_set(s->m_Defs, names[i].m_Name, &def);
	}
}

