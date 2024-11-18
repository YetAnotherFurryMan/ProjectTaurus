#include <trs/horn.h>

#include <stdio.h>

trs_IR* horn_parseTaurus(const char* src){
	horn_Token tok = {0};
	horn_next(&tok, src);

	switch(tok.type){
		case TRS_TT_ID:
		{
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;

			horn_LH(&tok, NULL);

			switch(tok.type){
				case TRS_TT_OP_PLUS: ir->cmd = TRS_IRCMD_ADD; break;
				case TRS_TT_OP_MUL: ir->cmd = TRS_IRCMD_MUL; break;
				case TRS_TT_OP_EQ: ir->cmd = TRS_IRCMD_SET; break;
				case TRS_TT_EOE: horn_next(&tok, NULL); return ir; // Consume
				default: return ir;
			}

			// Consume
			horn_next(&tok, NULL);
			
			if(ir->cmd == TRS_IRCMD_SET){
				ir->args = horn_parseTaurus(NULL);
				ir->next = horn_parseTaurus(NULL);
			} else{
				trs_IR* arg = trs_mallocIR();
				if(!arg) return NULL;
				arg->cmd = TRS_IRCMD_LOAD;
				arg->text = ir->text;
				arg->next = horn_parseTaurus(NULL);

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

			horn_LH(&tok, NULL);

			trs_IRCmd cmd;
			switch(tok.type){
				case TRS_TT_OP_PLUS: cmd = TRS_IRCMD_ADD; break;
				case TRS_TT_OP_MUL: cmd = TRS_IRCMD_MUL; break;
				case TRS_TT_OP_EQ: v->cmd = TRS_IRCMD_ERROR; return v; // [int] = ... is invalid
				case TRS_TT_EOE: horn_next(&tok, NULL); return v; // Consume
				default: return v;
			}

			// Consume
			horn_next(&tok, NULL);

			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = cmd;
			ir->args = v;

			v->next = horn_parseTaurus(NULL);

			return ir;
		} break;
		case TRS_TT_EOE: 
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
