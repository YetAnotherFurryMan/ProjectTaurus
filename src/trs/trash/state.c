#include <trs/state.h>

#include <stdlib.h>

#include <toollib/cvec/cvec.h>
#include <toollib/carea/carea.h>

void freeDefValue(Type* t, DefValue* val){
	switch(t->m_Kind){
		case TYPE_KIND_BASE:
		{
			TypeBase* base = (TypeBase*)t;
			switch(base->m_Base){
				case TYPE_BASE_STR:
				{
					ValueStr* str = val->ptr;
					free((char*)str->m_Value);
					free(str);
				} break;
				default:
					// No need to free that stuf
					break;
			}
		} break;
		case TYPE_KIND_FN:
		{
			// TODO: No need for implementing that now
		} break;
	}
}

void freeType(Type* t){
	switch(t->m_Kind){
		case TYPE_KIND_BASE:
			break;
		case TYPE_KIND_FN:
		{
			TypeFn* fn = (TypeFn*)t;
			if(fn->m_Return)
				freeType(fn->m_Return);
			if(fn->m_Arity && fn->m_Params){
				for(size_t i = 0; i < fn->m_Arity; i++)
					freeType(fn->m_Params[i]);
				free(fn->m_Params);
			}
		} break;
	}

	free(t);
}

void freeState(State* s){
	for(size_t i = 0; i < cvec_length(s->m_Defs); i++){
		Def* def = cvec_get(s->m_Defs, i);
		Type* type = def->m_Type;
		def->m_Type = 0;

		if(type->m_RefCount > 0){
			free(def->m_Value.ptr);
		} else if(!def->m_True){
			freeDefValue(type, &def->m_Value);
		}

		freeType(type);
	}

	cassoc_free(s->m_Defs);
	carea_freeHard(s->m_Area);
}

