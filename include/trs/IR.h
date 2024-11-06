#ifndef _IR_H_
#define _IR_H_

#include <stdlib.h>

#define TRS_IR_X_enum_IRCmd \
	X(ERROR)  			    \
	X(LOAD)   			    \
	X(INTVAL) 			    \
	X(SET)    			    \
	X(ADD)    			    \
	X(MUL)

typedef enum{
#define X(Y) TRS_IRCMD_##Y,
	TRS_IR_X_enum_IRCmd
#undef X
} trs_IRCmd;

typedef struct trs_IR trs_IR;
struct trs_IR{
	trs_IRCmd cmd;
	char* text;
	trs_IR* args;
	trs_IR* next;
};

static inline trs_IR* trs_mallocIR(){
	trs_IR* ir = (trs_IR*) malloc(sizeof(trs_IR));
	ir->cmd = TRS_IRCMD_ERROR;
	ir->text = NULL;
	ir->args = NULL;
	ir->next = NULL;
	return ir;
}

static inline void trs_freeIR(trs_IR* ir){
	while(ir){
		trs_IR* next = ir->next;
		free(ir->text);
		trs_freeIR(ir->args);
		free(ir);
		ir = next;
	}
}

static inline const char* trs_IRCmdToString(trs_IRCmd v){
#define X(Y) case TRS_IRCMD_##Y: return #Y;
	switch(v){
		TRS_IR_X_enum_IRCmd
		default: return "???";
	}
#undef X
}

#endif //_IR_H_
