#ifndef _TRS_BEE_H_
#define _TRS_BEE_H_

#include <stdio.h>

#include <trs/horn.h>

typedef bool(*bee_emit_t)(FILE*, horn_Obj*);

bool bee_emit(FILE* out, horn_Obj* src);

#endif //_TRS_BEE_H_
