#include <trs/eval.h>

#include <stdlib.h>
#include <string.h>

#include <toollib/cvec/cvec.h>

// TODO: yell -> yellp

Value callAST(State* s, ASTCall* call){
	Def* def = cassoc_get(s->m_Defs, call->m_Name);
	if(!def){
		// TODO: E_UNKNOWN_FN
		return (Value){0};
	} else if(def->m_Type->m_Kind != TYPE_KIND_FN){
		// TODO: E_IS_NOT_A_FN
		return (Value){0};
	}

	TypeFn* type = (TypeFn*)def->m_Type;

	size_t argc = call->m_Argc;
	if(argc < type->m_Arity){
		// TODO: E_TOO_FEW_ARGS
		return (Value){0};
	} else if(argc > type->m_Arity && !type->m_Variatic){
		// TODO: E_TOO_MANY_ARGS
		return (Value){0};
	}

	Value* argv = malloc(sizeof(Value) * argc);
	if(!argv)
		yell(FE_OUT_OF_MEMORY);
	
	size_t i = 0;
	AST* ast = call->m_Argv;
	while(ast && i < argc){
		argv[i] = evalValueAST(s, ast);
		ast = ast->m_Next;
		i++;
	}

	DefValue v = def->m_Value;
	for(size_t i = 0; i < type->m_Type.m_RefCount; i++)
		v = *((DefValue*)v.ptr);

	Value ret = {0};
	if(def->m_True){
		ret = v.trueFn(argc, argv);
	} else{
		// TODO: Push argv to the stack
		// TODO: Register args' names
		// TODO: Evaluate fn body
		// TODO: Get return
		// TODO: Unregister args' names and pop them from the stack
		//       Actually, restore defs' previous state
	}

	for(size_t i = 0; i < argc; i++)
		freeType(argv[i].m_Type);
	
	free(argv);

	return ret;
}

Type* cpyType(Type* t){
	Type* ret = NULL;
	switch(t->m_Kind){
		case TYPE_KIND_BASE:
		{
			ret = malloc(sizeof(TypeBase));
			TypeBase* base = (TypeBase*)ret;
			base->m_Type = *t;
			base->m_Base = ((TypeBase*)t)->m_Base;
		} break;
		case TYPE_KIND_FN:
		{
			ret = malloc(sizeof(TypeFn));
			TypeFn* fn = (TypeFn*)ret;
			fn->m_Type = *t;
			fn->m_Return = cpyType(((TypeFn*)t)->m_Return);
			fn->m_Arity = ((TypeFn*)t)->m_Arity;
			fn->m_Params = fn->m_Arity ? malloc(sizeof(Type*) * fn->m_Arity) : NULL;
			for(size_t i = 0; i < fn->m_Arity; i++)
				fn->m_Params[i] = cpyType(((TypeFn*)t)->m_Params[i]);
			fn->m_Variatic = ((TypeFn*)t)->m_Variatic;
		} break;
	}
	return ret;
}

Value cpyValue(Value* v){
	Value ret = {0};
	ret.m_Type = cpyType(v->m_Type);
	ret.m_True = v->m_True;

	// TODO: Use stack for holding values
	ret.m_Value = v->m_Value;

	return ret;
}

Value evalValueAST(State* s, AST* ast){
	Value ret = {0};

	switch(ast->m_Type){
		case AST_ID:
		{
			ASTId* id = (ASTId*)ast;
			Def* def = cassoc_get(s->m_Defs, id->m_Name);
			if(def)
				return cpyValue(def);
		} break;
		case AST_CALL:
		{
			ASTCall* call = (ASTCall*)ast;
			return callAST(s, call);
		} break;
		case AST_NUM:
		{
			ASTNum* num = (ASTNum*)ast;
			
			TypeBase* type = malloc(sizeof(TypeBase));
			type->m_Type.m_Kind = TYPE_KIND_BASE;
			type->m_Base = TYPE_BASE_SIZE;

			ret.m_Value.u64 = num->m_Value;
			ret.m_Type = (Type*)type;
		} break;
		case AST_STR:
		{
			ASTStr* str = (ASTStr*)ast;

			ValueStr* vs = malloc(sizeof(ValueStr));
			vs->m_Length = strlen(str->m_Value);
			vs->m_Value = str->m_Value;

			TypeBase* type = malloc(sizeof(TypeBase));
			type->m_Type.m_Kind = TYPE_KIND_BASE;
			type->m_Base = TYPE_BASE_STR;

			ret.m_Value.ptr = vs;
			ret.m_Type = (Type*)type;
		} break;
		default:
		{
			// TODO: E_EXPECTED_EXP
		} break;
	}

	return ret;
}

void evalAST(State* s, AST* ast){
	if(ast->m_Type != AST_ROOT){
		// TODO: Decide what to do in this scenario
		return;
	}

	ast = ast->m_Next;
	while(ast){
		switch(ast->m_Type){
			case AST_ROOT:
			{
				ast = ast->m_Next;
			} break;
			case AST_BLOCK:
			{
				// TODO: Needs to use stack, but that is not implemented yet
			} break;
			case AST_ID:
			{
				ASTId* id = (ASTId*)ast;
				yell(W_STAND_ALONE_ID, id->m_Name);
			} break;
			case AST_CALL:
			{
				ASTCall* call = (ASTCall*)ast;
				callAST(s, call);
			} break;
			case AST_VAR:
			{
				// TODO: now
			} break;
			case AST_NUM:
			{
				ASTNum* num = (ASTNum*)ast;
				yell(W_STAND_ALONE_NUM, num->m_Value);
			} break;
			case AST_STR:
			{
				ASTStr* str = (ASTStr*)ast;
				yell(W_STAND_ALONE_STR, str->m_Value);
			} break;
		}

		ast = ast->m_Next;
	}
}

