#include <trs/horn.h>

#include <stdio.h>
#include <string.h>

static inline trs_IRCmd horn_parseLispCmd(const horn_Token* tok){
	if(tok->type != TRS_TT_ID)
		return TRS_IRCMD_ERROR;
	return assoc_getOrDefault_trs_IRCmd(g_horn_lispKW, tok->text, TRS_IRCMD_ERROR);
}

static inline trs_IR* horn_parseLispOrVal();

trs_IR* horn_parseLispSExp(){
	horn_Token tok;

	trs_IR* ir = trs_mallocIR();
	if(!ir) return NULL;

	horn_next(&tok, NULL);

	trs_IRCmd cmd = horn_parseLispCmd(&tok);
	if(cmd == TRS_IRCMD_ERROR){
		if(tok.type != TRS_TT_ID)
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
		else
			fprintf(stderr, "ERROR: Unknown command: %s\n", tok.text);
		return ir;
	}


	if(cmd == TRS_IRCMD_SET){
		horn_next(&tok, NULL);
		if(tok.type != TRS_TT_ID){
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			return ir;
		}

		ir->text = tok.text;
		ir->args = horn_parseLispOrVal();
	} else{
		trs_IR* args = horn_parseLispOrVal();
		ir->args = args;
		while(args && tok.type != TRS_TT_RB){
			args->next = horn_parseLispOrVal();
			args = args->next;
		}
	}

	horn_next(&tok, NULL);

	ir->cmd = cmd;
	return ir;
}

static inline trs_IR* horn_parseLispOrVal(){
	horn_Token tok;
	horn_LH(&tok, NULL);
	switch(tok.type){
		case TRS_TT_ID:
		{
			horn_next(&tok, NULL);
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;
			return ir;
		} break;
		case TRS_TT_INT:
		{
			horn_next(&tok, NULL);
			trs_IR* v = trs_mallocIR();
			if(!v) return NULL;
			v->cmd = TRS_IRCMD_INTVAL;
			v->text = tok.text;
			return v;
		} break;
		case TRS_TT_LB:
			horn_next(&tok, NULL);
			return horn_parseLispSExp();
		case TRS_TT_RB: 
		case TRS_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}

trs_IR* horn_parseLisp(const char* src){
	horn_Token tok = {0};
	horn_LH(&tok, src);

	switch(tok.type){
		case TRS_TT_LB:
		{
			horn_next(&tok, NULL);
			trs_IR* ir = horn_parseLispSExp();
			if(ir)
				ir->next = horn_parseLisp(NULL);
			return ir;
		} break;
		case TRS_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", horn_TokenTypeToString(tok.type));
			free(tok.text);
		}
	}

	return NULL;
}
