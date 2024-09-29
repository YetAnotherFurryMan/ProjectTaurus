#include <trs/std.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

static inline void std_print_helper(Value* v){
	Type* type = v->m_Type;
	if(!type)
		return;

	switch(type->m_Kind){
		case TYPE_KIND_BASE:
		{
			TypeBase* base = (TypeBase*)type;
			DefValue val = v->m_Value;

			if(type->m_RefCount > 0){
				size_t rc = type->m_RefCount;
				while(rc > 0){
					val = *((DefValue*)val.ptr);
					rc--;
				}
			} 

			switch(base->m_Base){
				case TYPE_BASE_BOOL:
				{
					printf("%s", (val.b)?"true":"false");
				} break;
				case TYPE_BASE_STR:
				{
					printf("%s", ((ValueStr*)val.ptr)->m_Value);
				} break;
				// case TYPE_BASE_ISIZE:
				case TYPE_BASE_I8:
				// case TYPE_BASE_I16:
				// case TYPE_BASE_I32:
				// case TYPE_BASE_I64:
				{
					printf("%" PRId64, val.i64);
				} break;
				// case TYPE_BASE_USIZE:
				case TYPE_BASE_U8:
				// case TYPE_BASE_U16:
				// case TYPE_BASE_U32:
				// case TYPE_BASE_U64:
				{
					printf("%" PRIu64, val.u64);
				} break;
				case TYPE_BASE_SIZE:
				{
					printf("%" PRIx64, val.u64);
				} break;
			}
		} break;
		case TYPE_KIND_FN:
		{
			printf("fn<%p>", v->m_Value.ptr);
		} break;
	}
}

Value std_print_impl(size_t argc, Value* args){
	for(size_t i = 0; i < argc; i++)
		std_print_helper(&args[i]);
	return (Value){0};
} 
  
Value std_println_impl(size_t argc, Value* args){
	for(size_t i = 0; i < argc; i++)
		std_print_helper(&args[i]);
	puts("");
	return (Value){0};
} 
  
Value std_scanln_impl(size_t argc, Value* args){
	(void) argc;
	(void) args;

	TypeBase* type = malloc(sizeof(TypeBase));
	type->m_Type.m_Kind = TYPE_KIND_BASE;
	type->m_Base = TYPE_BASE_STR;

	char* str = NULL;
	size_t len = 0;
	getline(&str, &len, stdin);

	ValueStr* val = malloc(sizeof(ValueStr));
	val->m_Length = len;
	val->m_Value = str;

	Value ret = {0};
	ret.m_Type = (Type*)type;
	ret.m_Value.ptr = val;
	return ret;
} 
  
