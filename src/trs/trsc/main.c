#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* g_src = 
"A = 1 + 2 + 1;\n"
"B = A * 8;\n"
"A = A * B * 2;\n";

const char* g_src2 = 
"(set A (add 1 1))\n"
"(set B (mul A 2))\n"
"(set A (mul A B))";

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

typedef enum{
	IRCMD_ERROR = 0,
	IRCMD_LOAD,
	IRCMD_INTVAL,
	IRCMD_SET,
	IRCMD_ADD,
	IRCMD_MUL,
} IRCmd;

static inline const char* strIRCmd(IRCmd v){
#define CASE(X) case IRCMD_##X: return #X;
	switch(v){
		CASE(ERROR)
		CASE(LOAD)
		CASE(INTVAL)
		CASE(SET)
		CASE(ADD)
		CASE(MUL)
		default: return "???";
	}
#undef CASE
}

typedef struct IR{
	IRCmd cmd;
	struct IR* args;
	struct IR* next;
	char* text;
} IR;

static inline void freeIR(IR* ir){
	while(ir){
		IR* i = ir;
		ir = i->next;
		freeIR(i->args);
		free(i->text);
		free(i);
	}
}

IR* parse(const char* src);

void cgCompile(FILE* out, IR* ir);

int main(int argc, const char** argv){
	(void) argc;
	(void) argv;

	IR* ir = parse(g_src);
	
	cgCompile(stdout, ir);
	
	freeIR(ir);
	return 0;
}

void cgCompileCmd(FILE* out, IR* ir){
	switch(ir->cmd){
		case IRCMD_LOAD:
		{
			// Load a value to eax
			fprintf(out, "\tmov eax, dword [%s]\n", ir->text);
		} break;
		case IRCMD_INTVAL:
		{
			// Put the intval into eax
			fprintf(out, "\tmov eax, %s\n", ir->text);
		} break;
		case IRCMD_SET:
		{
			// Compile arg into eax and then save to destination
			if(!ir->args){
				fprintf(stderr, "ERROR: SET expects argument!\n");
				return;
			}

			cgCompileCmd(out, ir->args);
			fprintf(out, "\tmov dword [%s], eax\n", ir->text);
		} break;
		case IRCMD_ADD:
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
		case IRCMD_MUL:
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
			fprintf(stderr, "ERROR: Unexpected %s\n", strIRCmd(ir->cmd));
			break;
	}
}

// ECX EDX
void cgCompile(FILE* out, IR* ir){
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
		fprintf(stderr, "INFO: IR(%s)\n", strIRCmd(ir->cmd));
		cgCompileCmd(out, ir);
		ir = ir->next;
	}

	// Exit
	fputs("\tret\n", out);
}

static inline IR* mallocIR(){
	IR* ir = malloc(sizeof(IR));
	ir->cmd = IRCMD_ERROR;
	ir->args = NULL;
	ir->text = NULL;
	ir->next = NULL;
	return ir;
}

IR* parse(const char* src){
	Token tok = lexNext(src);
	switch(tok.type){
		case TT_ID:
		{
			IR* ir = mallocIR();
			if(!ir) return NULL;
			ir->cmd = IRCMD_LOAD;
			ir->text = tok.text;

			tok = lexLookahead();

			switch(tok.type){
				case TT_OP_PLUS: ir->cmd = IRCMD_ADD; break;
				case TT_OP_MUL: ir->cmd = IRCMD_MUL; break;
				case TT_OP_EQ: ir->cmd = IRCMD_SET; break;
				case TT_EOE: lexNext(NULL); return ir; // Consume
				default: return ir;
			}

			// Consume
			lexNext(NULL);
			
			if(ir->cmd == IRCMD_SET){
				ir->args = parse(NULL);
				ir->next = parse(NULL);
			} else{
				IR* arg = mallocIR();
				if(!arg) return NULL;
				arg->cmd = IRCMD_LOAD;
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
			IR* v = mallocIR();
			if(!v) return NULL;
			v->cmd = IRCMD_INTVAL;
			v->text = tok.text;

			tok = lexLookahead(NULL);

			IRCmd cmd;
			switch(tok.type){
				case TT_OP_PLUS: cmd = IRCMD_ADD; break;
				case TT_OP_MUL: cmd = IRCMD_MUL; break;
				case TT_OP_EQ: v->cmd = IRCMD_ERROR; return v; // [int] = ... is invalid
				case TT_EOE: lexNext(NULL); return v; // Consume
				default: return v;
			}

			// Consume
			lexNext(NULL);

			IR* ir = mallocIR();
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
