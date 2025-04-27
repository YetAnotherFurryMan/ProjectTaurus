#include <trs/bee.h>

static bool s_scope(FILE* out, horn_Obj* scope, size_t depth);

static bool s_gettype(FILE* out, horn_Obj* obj){
	if(obj->cmd != HORN_CMD_ID){
		// TODO: ERROR
		return true;
	}

	if(strcmp("i32", obj->as.text) == 0){
		fputs("int32_t", out);
	} else{
		// TODO: ERROR
		return true;
	}
	return false;
}

static bool s_type(FILE* out, horn_Obj* type){
	switch(type->cmd){
		case HORN_CMD_GETTYPE:
		{
			return s_gettype(out, type->as.args);
		} break;
		default:
			// TODO: ERROR
			return true;
	}
}

static bool s_args(FILE* out, horn_Obj* args){
	switch(args->cmd){
		case HORN_CMD_NIL:
		{
			fputs("void", out);
		} break;
		case HORN_CMD_LIST:
		{
			// TODO: TODO
			return true;
		} break;
		default:
			// TODO: ERROR
			return true;
	}
	return false;
}

static bool s_exp(FILE* out, horn_Obj* exp){
	switch(exp->cmd){
		case HORN_CMD_INTVAL:
		{
			fputs(exp->as.text, out);
		} break;
		case HORN_CMD_STRVAL:
		{
			// TODO: Compile text
			fputs(exp->as.text, out);
		} break;
		default:
			// TODO: ERROR
			return true;
	}
	return false;
}

static bool s_expr(FILE* out, horn_Obj* expr, size_t depth){
	switch(expr->cmd){
		case HORN_CMD_SCOPE:
		{
			for(size_t i = 0; i < depth; i++)
				fputs("\t", out);
			fputs("{", out);
			
			if(s_scope(out, expr, depth + 1)) return true;

			for(size_t i = 0; i < depth; i++)
				fputs("\t", out);
			fputs("}", out);
		} break;
		case HORN_CMD_PRINT:
		{
			horn_Obj* args = expr->as.args;
			while(args){
				for(size_t i = 0; i < depth; i++)
					fputs("\t", out);
				switch(args->cmd){
					case HORN_CMD_INTVAL:
					{
						fprintf(out, "fputs(\"%s\", stdout);", args->as.text);
					} break;
					case HORN_CMD_STRVAL:
					{
						// TODO: Compile text
						fprintf(out, "fputs(%s, stdout);", args->as.text);
					} break;
					default:
						// TODO: Typeof expr and put it here
						return true;
				}
				args = args->next;

				if(args)
					fputs("\n", out);
			} 
		} break;
		case HORN_CMD_RETURN:
		{
			for(size_t i = 0; i < depth; i++)
				fputs("\t", out);
			if(!expr->as.args){
				fputs("return;", out);
			} else {
				fputs("return ", out);
				if(s_exp(out, expr->as.args)) return true;
				fputs(";", out);
			}
		} break;
		default:
			// TODO: ERROR
			return true;
	}

	return false;
}

static bool s_scope(FILE* out, horn_Obj* scope, size_t depth){
	if(scope->cmd == HORN_CMD_SCOPE){
		horn_Obj* body = scope->as.args;
		while(body){
			fputs("\n", out);
			if(s_expr(out, body, depth)) return true;
			body = body->next;
		}

		if(scope->as.args)
			fputs("\n", out);
	} else{
		fputs("\n", out);
		if(s_expr(out, scope, depth)) return true;
		fputs("\n", out);
	}
	return false;
}

static bool s_function(FILE* out, horn_Obj* fn){
	horn_Obj* id = fn->as.args;
	horn_Obj* args = id->next;
	horn_Obj* rtype = args->next;
	horn_Obj* body = rtype->next;

	if(s_type(out, rtype)) return true;
	fprintf(out, " %s(", id->as.text);
	if(s_args(out, args)) return true;
	fputs("){", out);
	if(s_scope(out, body, 1)) return true;
	fputs("}\n\n", out);
	return false;
}

bool bee_emit(FILE* out, horn_Obj* src){
	if(!out || !src)
		return true;

	// Header
	fputs("#include <stdio.h>\n", out);
	fputs("#include <stdlib.h>\n", out);
	fputs("#include <stdint.h>\n\n", out);

	while(src){
		switch(src->cmd){
			case HORN_CMD_FUNCTION:
			{
				if(s_function(out, src))
					return true;
			} break;
			default:
				// TODO: ERROR
				return true;
		}

		src = src->next;
	}

	return false;
}
