#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <trs/IR.h>
#include <trs/cg.h>
#include <trs/parser.h>

const char* g_src = 
"A = 1 + 2 + 1;\n"
"B = A * 8;\n"
"A = A * B * 2;\n"
"B = 2 * B;\n";

const char* g_src2 = 
"(set A (add 1 (add 2 1)))\n"
"(set B (mul A 8))\n"
"(set A (mul A (mul B 2)))\n"
"(set B (mul 2 B))\n";

const char* g_out = "out.nasm";

// CodeGen: trs.cg.{TARGET}.so
//     TARGET: x86, x86-64, arm, aarch64, nasm_x86, nasm_x86-64, nasm_arm, nasm_aarch64, LLVM, QBE, lisp
int main(int argc, const char** argv){
	const char* target = "nasm_x86";
	if(argc >= 2)
		target = argv[1];

	trs_CG cg = trs_cgLoad(target);
	if(!cg.hnd)
		return 1;
	
	trs_IR* ir = trs_parseLisp(g_src2);
	if(!ir){
		fprintf(stderr, "ERROR: Failed to compile.\n");
		return 1;
	}
	
	cg.compile(stdout, ir);
	
	trs_freeIR(ir);

	trs_cgUnload(&cg);
	return 0;
}

