#ifndef _STD_H_
#define _STD_H_

#include "state.h"

Value std_print_impl(size_t argc, Value* args);
Value std_println_impl(size_t argc, Value* args);
Value std_scanln_impl(size_t argc, Value* args);

void stdLoad(State* s);

#endif // _STD_H_
