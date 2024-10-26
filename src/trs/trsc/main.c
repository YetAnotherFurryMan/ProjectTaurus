#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <trs/IR.h>
#include <trs/cg.h>

const char* g_src = 
"A = 1 + 2 + 1;\n"
"B = A * 8;\n"
"A = A * B * 2;\n";

const char* g_src2 = 
"(set A (add 1 (add 2 1)))\n"
"(set B (mul A 8))\n"
"(set A (mul A (mul B 2)))\n";

const char* g_out = "out.nasm";

typedef enum{
	TT_UNK = 0,
	TT_EOF,
	TT_INT,
	TT_OP_EQ,
	TT_OP_PLUS,
	TT_OP_MUL,
	TT_EOE,     // ';'
	TT_ID,
} TokenType;

static inline const char* strTT(TokenType tt){
#define CASE(T) case TT_##T: return #T;
	switch(tt){
		CASE(EOF)
		CASE(UNK)
		CASE(INT)
		CASE(OP_EQ)
		CASE(OP_PLUS)
		CASE(OP_MUL)
		CASE(EOE)
		CASE(ID)
		default:
			return "???";
	}
#undef CASE
}

typedef struct Token{
	TokenType type;
	char* text;
} Token;

static inline void freeToken(Token* tok){
	free(tok->text);
	tok->text = 0;
	tok->type = TT_UNK;
}

Token lexNext(const char* src);
Token lexLookahead();

trs_IR* parse(const char* src);

void cgCompile(FILE* out, trs_IR* ir);

// CodeGen: trs.cg.{TARGET}.so
//     TARGET: x86, x86-64, arm, aarch64, nasm_x86, nasm_x86-64, nasm_arm, nasm_aarch64, LLVM, QBE, lisp
int main(int argc, const char** argv){
	const char* target = "nasm_x86";
	if(argc >= 2)
		target = argv[1];

	// Load codegen for nasm_x86
	trs_CG cg_nasm_x86 = trs_cgLoad(target);
	if(!cg_nasm_x86.hnd)
		return 1;
	
	trs_IR* ir = parse(g_src);
	
	//cgCompile(stdout, ir);
	cg_nasm_x86.compile(stdout, ir);
	
	trs_freeIR(ir);

	trs_cgUnload(&cg_nasm_x86);
	return 0;
}

void cgCompileCmd(FILE* out, trs_IR* ir){
	switch(ir->cmd){
		case TRS_IRCMD_LOAD:
		{
			// Load a value to eax
			fprintf(out, "\tmov eax, dword [%s]\n", ir->text);
		} break;
		case TRS_IRCMD_INTVAL:
		{
			// Put the intval into eax
			fprintf(out, "\tmov eax, %s\n", ir->text);
		} break;
		case TRS_IRCMD_SET:
		{
			// Compile arg into eax and then save to destination
			if(!ir->args){
				fprintf(stderr, "ERROR: SET expects argument!\n");
				return;
			}

			cgCompileCmd(out, ir->args);
			fprintf(out, "\tmov dword [%s], eax\n", ir->text);
		} break;
		case TRS_IRCMD_ADD:
		{
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, add eax and ebx
			if(!ir->args || !ir->args->next){
				fprintf(stderr, "ERROR: ADD expects 2 arguments!\n");
				return;
			}

			cgCompileCmd(out, ir->args->next);
			fputs("\tmov ebx, eax\n", out);
			cgCompileCmd(out, ir->args);
			fputs("\tadd eax, ebx\n", out);
		} break;
		case TRS_IRCMD_MUL:
		{
			// Compile 2nd arg into eax, move eax to ebx, compile 1st arg to eax, mul by ebx
			if(!ir->args || !ir->args->next){
				fprintf(stderr, "ERROR: MUL expects 2 arguments!\n");
				return;
			}

			cgCompileCmd(out, ir->args->next);
			fputs("\tmov ebx, eax\n", out);
			cgCompileCmd(out, ir->args);
			fputs("\tmul ebx\n", out);
		} break;
		default:
			fprintf(stderr, "ERROR: Unexpected %s\n", trs_IRCmdToString(ir->cmd));
			break;
	}
}

// ECX EDX
void cgCompile(FILE* out, trs_IR* ir){
	// Runtime
	fputs("section .data\n", out);
	fputs("\tA dd 0\n", out);
	fputs("\tB dd 0\n", out);
	fputs("\n", out);
	fputs("section .text\n", out);
	fputs("\tglobal _start\n", out);
	fputs("\tglobal main\n", out);
	fputs("\tglobal hex32tos\n", out);
	fputs("\tglobal put_hex\n", out);
	fputs("\tglobal ln\n", out);
	fputs("_start:\n", out);
	fputs("\tcall main\n", out);
	// write A and B
	
	fputs("\tmov ecx, dword [A]\n", out);
	fputs("\tcall put_hex\n", out);
	fputs("\tcall ln\n", out);

	fputs("\tmov ecx, dword [B]\n", out);
	fputs("\tcall put_hex\n", out);
	fputs("\tcall ln\n", out);

	// end
	fputs("\tmov eax, 1\n", out);
	fputs("\tmov ebx, 0\n", out);
	fputs("\tint 80h\n", out);
	fputs("\n", out);
	fputs("put_hex:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov edx, ecx\n", out);
	fputs("\tlea ecx, [ebp - 11]\n", out);
	fputs("\tcall hex32tos\n", out);
	fputs("\tmov eax, 4\n", out);
	fputs("\tmov ebx, 1\n", out);
	fputs("\tlea ecx, [ebp - 11]\n", out);
	fputs("\tmov edx, 10\n", out);
	fputs("\tint 80h\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);
	fputs("hex32tos:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov dword [ebp - 4], 8\n", out);
	fputs("\tmov word [ecx], '0x'\n", out);
	fputs("\tadd ecx, 2\n", out);
	fputs("\tmov dword [ecx], 0\n", out);
	fputs("\tmov dword [ecx + 4], 0\n", out);
	fputs(".L3:\n", out);
	fputs("\tsub dword [ebp - 4], 1\n", out);
	fputs("\tmov eax, dword [ebp - 4]\n", out);
	fputs("\tmov ebx, edx\n", out);
	fputs("\tand ebx, 15\n", out);
	fputs("\tshr edx, 4\n", out);
	fputs("\tcmp ebx, 9\n", out);
	fputs("\tjg .L1\n", out);
	fputs("\tadd bl, '0'\n", out);
	fputs("\tjmp .L2\n", out);
	fputs(".L1:\n", out);
	fputs("\tadd bl, 'A' - 10\n", out);
	fputs(".L2:\n", out);
	fputs("\tmov byte [ecx + eax], bl\n", out);
	fputs("\tcmp eax, 0\n", out);
	fputs("\tjne .L3\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);
	fputs("ln:\n", out);
	fputs("\tpush ebp\n", out);
	fputs("\tmov ebp, esp\n", out);
	fputs("\tsub esp, 16\n", out);
	fputs("\tmov byte [ebp - 1], 10\n", out);
	fputs("\tmov eax, 4\n", out);
	fputs("\tmov ebx, 1\n", out);
	fputs("\tlea ecx, [ebp - 1]\n", out);
	fputs("\tmov edx, 1\n", out);
	fputs("\tint 80h\n", out);
	fputs("\tmov esp, ebp\n", out);
	fputs("\tpop ebp\n", out);
	fputs("\tret\n", out);

	// Code
	fputs("main:\n", out);

	while(ir){
		fprintf(stderr, "INFO: IR(%s)\n", trs_IRCmdToString(ir->cmd));
		cgCompileCmd(out, ir);
		ir = ir->next;
	}

	// Exit
	fputs("\tret\n", out);
}

trs_IR* parse(const char* src){
	Token tok = lexNext(src);
	switch(tok.type){
		case TT_ID:
		{
			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = TRS_IRCMD_LOAD;
			ir->text = tok.text;

			tok = lexLookahead();

			switch(tok.type){
				case TT_OP_PLUS: ir->cmd = TRS_IRCMD_ADD; break;
				case TT_OP_MUL: ir->cmd = TRS_IRCMD_MUL; break;
				case TT_OP_EQ: ir->cmd = TRS_IRCMD_SET; break;
				case TT_EOE: lexNext(NULL); return ir; // Consume
				default: return ir;
			}

			// Consume
			lexNext(NULL);
			
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
		case TT_INT:
		{
			trs_IR* v = trs_mallocIR();
			if(!v) return NULL;
			v->cmd = TRS_IRCMD_INTVAL;
			v->text = tok.text;

			tok = lexLookahead(NULL);

			trs_IRCmd cmd;
			switch(tok.type){
				case TT_OP_PLUS: cmd = TRS_IRCMD_ADD; break;
				case TT_OP_MUL: cmd = TRS_IRCMD_MUL; break;
				case TT_OP_EQ: v->cmd = TRS_IRCMD_ERROR; return v; // [int] = ... is invalid
				case TT_EOE: lexNext(NULL); return v; // Consume
				default: return v;
			}

			// Consume
			lexNext(NULL);

			trs_IR* ir = trs_mallocIR();
			if(!ir) return NULL;
			ir->cmd = cmd;
			ir->args = v;

			v->next = parse(NULL);
			// ir->next = parse(NULL);

			return ir;
		} break;
		case TT_EOE: 
		case TT_EOF:
			break;
		default:
		{
			fprintf(stderr, "ERROR: Unexpected token: %s\n", strTT(tok.type));
			freeToken(&tok);
		}
	}

	return NULL;
}

Token g_lexLookahead = {0};

Token lexNext(const char* src){
	static const char* s = NULL;
	if(src){
		s = src;
		g_lexLookahead = (Token){0};
	}

	if(g_lexLookahead.type != TT_UNK){
		Token ret = g_lexLookahead;
		g_lexLookahead = (Token){0};
		fprintf(stderr, "INFO: Lookahead(%s): %s\n", strTT(ret.type), ret.text);
		return ret;
	}

	if(!s || !*s)
		return (Token){TT_EOF, NULL};

	while(*s && (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r'))
		s++;

	if(!*s){
		s = NULL;
		return (Token){TT_EOF, NULL};
	}

	Token tok = {TT_UNK, NULL};

	switch(*s){
		case '=':
			tok.type = TT_OP_EQ;
			break;
		case '+':
			tok.type = TT_OP_PLUS;
			break;
		case '*':
			tok.type = TT_OP_MUL;
			break;
		case ';':
			tok.type = TT_EOE;
			break;
		default:
		{
			if(*s >= '0' && *s <= '9'){
				tok.type = TT_INT;

				size_t i = 1;
				while(s[i] && s[i] >= '0' && s[i] <= '9')
					i++;

				char* text = malloc((i + 1) * sizeof(char));
				if(!text)
					return tok;

				memcpy(text, s, i);
				text[i] = 0;
				
				tok.text = text;
				s += i - 1;
			} else if(*s == '_' || (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z')){
				tok.type = TT_ID;

				size_t i = 1;
				while(s[i] && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9')))
					i++;
				
				char* text = malloc((i + 1) * sizeof(char));
				if(!text)
					return tok;
				
				memcpy(text, s, i);
				text[i] = 0;
				
				tok.text = text;
				s += i - 1;
			}
		} break;
	}

	if(*s)
		s++;
	else 
		s = NULL;

	fprintf(stderr, "INFO: Token(%s): %s\n", strTT(tok.type), tok.text);
	return tok;
}

Token lexLookahead(){
	if(g_lexLookahead.type == TT_UNK)
		g_lexLookahead = lexNext(NULL);
	return g_lexLookahead;
}
