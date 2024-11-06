#include <trs/parser.h>

#include <stdio.h>

trs_IR* trs_parse(const char* src){
	trs_Token tok = {0};
	trs_lexNext(&tok, src);

	switch(tok.type){
		case TRS_TT_ID:
		{
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;

			trs_lexLH(&tok, NULL);

			switch(tok.type){
				case TRS_TT_OP_PLUS: ir->cmd = TRS_IRCMD_ADD; break;
				case TRS_TT_OP_MUL: ir->cmd = TRS_IRCMD_MUL; break;
				case TRS_TT_OP_EQ: ir->cmd = TRS_IRCMD_SET; break;
				case TRS_TT_EOE: trs_lexNext(&tok, NULL); return ir; // Consume
				default: return ir;
			}

			// Consume
			trs_lexNext(&tok, NULL);
			
			if(ir->cmd == TRS_IRCMD_SET){
				ir->args = trs_parse(NULL);
				ir->next = trs_parse(NULL);
			} else{
				trs_IR* arg = trs_mallocIR();
				if(!arg) return NULL;
				arg->cmd = TRS_IRCMD_LOAD;
				arg->text = ir->text;
				arg->next = trs_parse(NULL);

				ir->args = arg;
				ir->text = NULL;
			}

			return ir;
		} break;
		case TRS_TT_INT:
		{
			trs_IR* v = trs_mallocIR();
			if(!v) return NULL;
			v->cmd = TRS_IRCMD_INTVAL;
			v->text = tok.text;

			trs_lexLH(&tok, NULL);

			trs_IRCmd cmd;
			switch(tok.type){
				case TRS_TT_OP_PLUS: cmd = TRS_IRCMD_ADD; break;
				case TRS_TT_OP_MUL: cmd = TRS_IRCMD_MUL; break;
				case TRS_TT_OP_EQ: v->cmd = TRS_IRCMD_ERROR; return v; // [int] = ... is invalid
				case TRS_TT_EOE: trs_lexNext(&tok, NULL); return v; // Consume
				default: return v;
			}

			// Consume
			trs_lexNext(&tok, NULL);

			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = cmd;
			ir->args = v;

			v->next = trs_parse(NULL);

			return ir;
		} break;
		case TRS_TT_EOE: 
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
