#include <trs/parser.h>

#include <stdio.h>
#include <string.h>

static inline trs_IRCmd trs_parseLispCmd(const trs_Token* tok){
	// TODO: use assoc
	
	if(tok->type != TRS_TT_ID)
		return TRS_IRCMD_ERROR;

#define CASE(X, Y) if(strcmp(tok->text, #X) == 0) return TRS_IRCMD_##Y;
	CASE(load, LOAD)
	else CASE(set, SET)
	else CASE(add, ADD)
	else CASE(mul, MUL)
	else return TRS_IRCMD_ERROR;
#undef CASE
}

static inline trs_IR* trs_parseLispOrVal();

trs_IR* trs_parseLispSExp(){
	trs_Token tok;

	trs_IR* ir = trs_mallocIR();
	if(!ir) return NULL;

	trs_lexNext(&tok, NULL);

	trs_IRCmd cmd = trs_parseLispCmd(&tok);
	if(cmd == TRS_IRCMD_ERROR){
		if(tok.type != TRS_TT_ID)
			fprintf(stderr, "ERROR: Unexpected token: %s\n", trs_TokenTypeToString(tok.type));
		else
			fprintf(stderr, "ERROR: Unknown command: %s\n", tok.text);
		return ir;
	}


	if(cmd == TRS_IRCMD_SET){
		trs_lexNext(&tok, NULL);
		if(tok.type != TRS_TT_ID){
			fprintf(stderr, "ERROR: Unexpected token: %s\n", trs_TokenTypeToString(tok.type));
			return ir;
		}

		ir->text = tok.text;
		ir->args = trs_parseLispOrVal();
	} else{
		trs_IR* args = trs_parseLispOrVal();
		ir->args = args;
		while(args && tok.type != TRS_TT_RB){
			args->next = trs_parseLispOrVal();
			args = args->next;
		}
	}

	trs_lexNext(&tok, NULL);

	ir->cmd = cmd;
	return ir;
}

static inline trs_IR* trs_parseLispOrVal(){
	trs_Token tok;
	trs_lexLH(&tok, NULL);
	switch(tok.type){
		case TRS_TT_ID:
		{
			trs_lexNext(&tok, NULL);
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;
			return ir;
		} break;
		case TRS_TT_INT:
		{
			trs_lexNext(&tok, NULL);
			trs_IR* v = trs_mallocIR();
			if(!v) return NULL;
			v->cmd = TRS_IRCMD_INTVAL;
			v->text = tok.text;
			return v;
		} break;
		case TRS_TT_LB:
			trs_lexNext(&tok, NULL);
			return trs_parseLispSExp();
		case TRS_TT_RB: 
		case TRS_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", trs_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}

trs_IR* trs_parseLisp(const char* src){
	trs_Token tok = {0};
	trs_lexLH(&tok, src);

	switch(tok.type){
		case TRS_TT_LB:
		{
			trs_lexNext(&tok, NULL);
			trs_IR* ir = trs_parseLispSExp();
			if(ir)
				ir->next = trs_parseLisp(NULL);
			return ir;
		} break;
		case TRS_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", trs_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}
