#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <trs/IR.h>
#include <trs/cg.h>
#include <trs/parser.h>

char* g_src = NULL;

const char* g_src1 = 
"A = 1 + 2 + 1;\n"
"B = A * 8;\n"
"A = A * B * 2;\n"
"B = 2 * B;\n";

const char* g_src2 = 
"(set A (add 1  2 1))\n"
"(set B (mul A 8))\n"
"(set A (mul A B 2))\n"
"(set B (mul 2 B))\n";

const char* g_out = "out.nasm";

enum{
	PARSE_LISP,
	PARSE_TRS
} g_whatToParse = PARSE_TRS;

void loadSrc(const char* path);

// CodeGen: trs.cg.{TARGET}.so
//     TARGET: x86, x86-64, arm, aarch64, nasm_x86, nasm_x86-64, nasm_arm, nasm_aarch64, LLVM, QBE, lisp
int main(int argc, const char** argv){
	const char* target = "nasm_x86";

	if(argc >= 2)
		target = argv[1];
	if(argc >= 3)
		loadSrc(argv[2]);

	trs_CG cg = trs_cgLoad(target);
	if(!cg.hnd)
		return 1;

	const char* src = g_src;
	if(!src)
		src = g_src1;
	
	trs_IR* ir = NULL;
	switch(g_whatToParse){
		case PARSE_LISP:
			ir = trs_parseLisp(src);
			break;
		case PARSE_TRS:
			ir = trs_parse(src);
			break;
	}

	free(g_src);

	if(!ir){
		fprintf(stderr, "ERROR: Failed to compile.\n");
		return 1;
	}
	
	cg.compile(stdout, ir);
	
	trs_freeIR(ir);

	trs_cgUnload(&cg);
	return 0;
}

void loadSrc(const char* path){
	size_t len = strlen(path);
	if(len > 4 && strcmp(path + len - 4, ".trs") == 0){
		g_whatToParse = PARSE_TRS;
	} else if(len > 8 && strcmp(path + len - 8, ".trslisp") == 0){
		g_whatToParse = PARSE_LISP;
	} else{
		fprintf(stderr, "ERROR: Unknown extension: %s\n", path);
		exit(1);
	} 

	FILE* in = fopen(path, "r");
	if(!in){
		fprintf(stderr, "ERROR: Failed to open file: %s\n", path);
		exit(1);
	}
	
	fseek(in, 0, SEEK_END);
	len = ftell(in);
	rewind(in);

	char* src = malloc(len);
	if(!src){
		fprintf(stderr, "ERROR: Out of memory while reading the file: %s\n", path);
		exit(1);
	}

	fread(src, len, 1, in);

	fclose(in);

	g_src = src;
}

