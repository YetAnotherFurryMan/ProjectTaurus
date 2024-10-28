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
"(set A (mul A (mul B 2)))\n";

const char* g_out = "out.nasm";

/* typedef enum{ */
/* 	TT_UNK = 0, */
/* 	TT_EOF, */
/* 	TT_INT, */
/* 	TT_OP_EQ, */
/* 	TT_OP_PLUS, */
/* 	TT_OP_MUL, */
/* 	TT_EOE,     // ';' */
/* 	TT_ID, */
/* 	TT_LB,      // ( */
/* 	TT_RB,      // ) */
/* 	// */
/* } TokenType; */

/* static inline const char* strTT(TokenType tt){ */
/* #define CASE(T) case TT_##T: return #T; */
/* 	switch(tt){ */
/* 		CASE(EOF) */
/* 		CASE(UNK) */
/* 		CASE(INT) */
/* 		CASE(OP_EQ) */
/* 		CASE(OP_PLUS) */
/* 		CASE(OP_MUL) */
/* 		CASE(EOE) */
/* 		CASE(ID) */
/* 		default: */
/* 			return "???"; */
/* 	} */
/* #undef CASE */
/* } */

/* typedef struct Token{ */
/* 	TokenType type; */
/* 	char* text; */
/* } Token; */

/* static inline void freeToken(Token* tok){ */
/* 	free(tok->text); */
/* 	tok->text = 0; */
/* 	tok->type = TT_UNK; */
/* } */

/* Token lexNext(const char* src); */
/* Token lexLookahead(); */

trs_IR* parse(const char* src);

// CodeGen: trs.cg.{TARGET}.so
//     TARGET: x86, x86-64, arm, aarch64, nasm_x86, nasm_x86-64, nasm_arm, nasm_aarch64, LLVM, QBE, lisp
int main(int argc, const char** argv){
	const char* target = "nasm_x86";
	if(argc >= 2)
		target = argv[1];

	trs_CG cg = trs_cgLoad(target);
	if(!cg.hnd)
		return 1;
	
	trs_IR* ir = parse(g_src);
	
	cg.compile(stdout, ir);
	
	trs_freeIR(ir);

	trs_cgUnload(&cg);
	return 0;
}

trs_IR* parse(const char* src){
	trs_Token tok = {0};
	trs_lexNext(&tok, src);

	switch(tok.type){
		case TRS_TT_ID:
		{
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;

			trs_lexLH(&tok);

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
				ir->args = parse(NULL);
				ir->next = parse(NULL);
			} else{
				trs_IR* arg = trs_mallocIR();
				if(!arg) return NULL;
				arg->cmd = TRS_IRCMD_LOAD;
				arg->text = ir->text;
				arg->next = parse(NULL);

				ir->args = arg;
				ir->text = NULL;
				// ir->next = parse(NULL);
			}

			return ir;
		} break;
		case TRS_TT_INT:
		{
			trs_IR* v = trs_mallocIR();
			if(!v) return NULL;
			v->cmd = TRS_IRCMD_INTVAL;
			v->text = tok.text;

			trs_lexLH(&tok);

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

			v->next = parse(NULL);
			// ir->next = parse(NULL);

			return ir;
		} break;
		case TRS_TT_EOE: 
		case TRS_TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", trs_TokenTypeToString(tok.type));
			//freeToken(&tok);
			free(tok.text);
		}
	}

	return NULL;
}

/* Token g_lexLookahead = {0}; */

/* Token lexNext(const char* src){ */
/* 	static const char* s = NULL; */
/* 	if(src){ */
/* 		s = src; */
/* 		g_lexLookahead = (Token){0}; */
/* 	} */

/* 	if(g_lexLookahead.type != TT_UNK){ */
/* 		Token ret = g_lexLookahead; */
/* 		g_lexLookahead = (Token){0}; */
/* 		fprintf(stderr, "INFO: Lookahead(%s): %s\n", strTT(ret.type), ret.text); */
/* 		return ret; */
/* 	} */

/* 	if(!s || !*s) */
/* 		return (Token){TT_EOF, NULL}; */

/* 	while(*s && (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r')) */
/* 		s++; */

/* 	if(!*s){ */
/* 		s = NULL; */
/* 		return (Token){TT_EOF, NULL}; */
/* 	} */

/* 	Token tok = {TT_UNK, NULL}; */

/* 	switch(*s){ */
/* 		case '=': */
/* 			tok.type = TT_OP_EQ; */
/* 			break; */
/* 		case '+': */
/* 			tok.type = TT_OP_PLUS; */
/* 			break; */
/* 		case '*': */
/* 			tok.type = TT_OP_MUL; */
/* 			break; */
/* 		case ';': */
/* 			tok.type = TT_EOE; */
/* 			break; */
/* 		default: */
/* 		{ */
/* 			if(*s >= '0' && *s <= '9'){ */
/* 				tok.type = TT_INT; */

/* 				size_t i = 1; */
/* 				while(s[i] && s[i] >= '0' && s[i] <= '9') */
/* 					i++; */

/* 				char* text = malloc((i + 1) * sizeof(char)); */
/* 				if(!text) */
/* 					return tok; */

/* 				memcpy(text, s, i); */
/* 				text[i] = 0; */
				
/* 				tok.text = text; */
/* 				s += i - 1; */
/* 			} else if(*s == '_' || (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z')){ */
/* 				tok.type = TT_ID; */

/* 				size_t i = 1; */
/* 				while(s[i] && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) */
/* 					i++; */
				
/* 				char* text = malloc((i + 1) * sizeof(char)); */
/* 				if(!text) */
/* 					return tok; */
				
/* 				memcpy(text, s, i); */
/* 				text[i] = 0; */
				
/* 				tok.text = text; */
/* 				s += i - 1; */
/* 			} */
/* 		} break; */
/* 	} */

/* 	if(*s) */
/* 		s++; */
/* 	else */ 
/* 		s = NULL; */

/* 	fprintf(stderr, "INFO: Token(%s): %s\n", strTT(tok.type), tok.text); */
/* 	return tok; */
/* } */

/* Token lexLookahead(){ */
/* 	if(g_lexLookahead.type == TT_UNK) */
/* 		g_lexLookahead = lexNext(NULL); */
/* 	return g_lexLookahead; */
/* } */
