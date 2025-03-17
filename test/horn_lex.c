#include <stdio.h>

#include <trs/horn.h>

#define FAIL_TYPE(t, e) printf("FAILED: Bad types: %s != %s\n", horn_TokenTypeToString(t.type), horn_TokenTypeToString(e.type));
#define FAIL_BEGIN(t, e) printf("FAILED: Bad begin: %p != %p\n", t.begin, e.begin);
#define FAIL_END(t, e) printf("FAILED: Bad end: %p != %p\n", t.end, e.end);

const char* src = "a aa _a 2421 - + = ()";
horn_Token* expect ;

int main(void){
	expect = (horn_Token[]){
		(horn_Token){HORN_TT_ID, src + 0, src + 1},
		(horn_Token){HORN_TT_ID, src + 2, src + 4},
		(horn_Token){HORN_TT_ID, src + 5, src + 7},
		(horn_Token){HORN_TT_INT, src + 8, src + 12},
		(horn_Token){HORN_TT_OP_MINUS, src + 13, src + 14},
		(horn_Token){HORN_TT_OP_PLUS, src + 15, src + 16},
		(horn_Token){HORN_TT_OP_EQ, src + 17, src + 18},
		(horn_Token){HORN_TT_LP, src + 19, src + 20},
		(horn_Token){HORN_TT_RP, src + 20, src + 21},
		(horn_Token){HORN_TT_EOF, NULL, NULL}
	};

	horn_init();

	horn_State state = {0};
	horn_resetState(&state, src);

	horn_Token tok = {0};
	for(size_t i = 0; i < 10; i++){
		horn_next(&state, &tok);
		if(tok.type != expect[i].type){
			FAIL_TYPE(tok, expect[i])
			return 1;
		}

		if(tok.begin != expect[i].begin){
			FAIL_BEGIN(tok, expect[i])
			return 1;
		}

		if(tok.end != expect[i].end){
			FAIL_END(tok, expect[i])
			return 1;
		}
	}

	horn_terminate();

	printf("OK\n");
	return 0;
}
