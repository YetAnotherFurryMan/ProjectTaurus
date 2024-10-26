#ifndef _CG_H_
#define _CG_H_

#include <stdio.h>

#include <dlfcn.h>

#include "IR.h"

typedef int(*trs_cgCompile_t)(FILE*, trs_IR*);

typedef struct trs_CG trs_CG;

struct trs_CG{
	void* hnd;
	trs_cgCompile_t compile;
	trs_cgCompile_t compileCmd;
};

static inline trs_CG trs_cgLoad(const char* target){
	trs_CG cg = {0, 0, 0};

	char so[255] = {0};
	snprintf(so, 255, "trs.cg.%s.so", target);

	dlerror();
	cg.hnd = dlopen(so, RTLD_LAZY);
	if(!cg.hnd){
		fprintf(stderr, "ERROR: Failed to load codegen: %s\n", so);
		fprintf(stderr, "NOTE: %s\n", dlerror());
		return cg;
	}

	cg.compile = (trs_cgCompile_t) dlsym(cg.hnd, "trs_cgCompile");
	if(!cg.compile){
		fprintf(stderr, "ERROR: Codegen does not implement trs_cgCompile: %s\n", so);
		fprintf(stderr, "NOTE: %s\n", dlerror());
		dlclose(cg.hnd);
		cg.hnd = 0;
		return cg;
	}

	cg.compileCmd = (trs_cgCompile_t) dlsym(cg.hnd, "trs_cgCompileCmd");
	if(!cg.compileCmd){
		fprintf(stderr, "WARNING: Codegen does not implement trs_cgCompileCmd: %s\n", so);
		fprintf(stderr, "Note: %s\n", dlerror());
	}

	return cg;
}

static inline void trs_cgUnload(trs_CG* cg){
	dlclose(cg->hnd);
	cg->hnd = NULL;
}

int trs_cgCompile(FILE* out, trs_IR* ir);
int trs_cgCompileCmd(FILE* out, trs_IR* ir);

#endif //_CG_H_
