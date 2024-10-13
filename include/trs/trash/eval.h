#ifndef _EVAL_H_
#define _EVAL_H_

#include "state.h"
#include "ast.h"

Value evalValueAST(State* s, AST* ast);
void evalAST(State* s, AST* ast);

#endif // _EVAL_H_
