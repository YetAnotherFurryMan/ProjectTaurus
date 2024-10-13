#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>

#include <toollib/ap/ap.h>
#include <toollib/cvec/cvec.h>
#include <toollib/carea/carea.h>

#include <trs/error.h>
#include <trs/state.h>
#include <trs/eval.h>
#include <trs/ast.h>
#include <trs/std.h>

// TODO: Use libffi to call functions from dl
// TODO: Remember to implement some way to at runtime declare the type of smt so libffi will not be needed in Taurus

#define MAX_FILES 1000

State g_state = {0};

#define YELL(...) do {printf("%s:%d: yell:\n", __FILE__, __LINE__); yell(__VA_ARGS__);}while(0)

void help(State*);

void fn_println_impl(AST* arg){
	while(arg){
		if(arg->m_Type == AST_STR){
			printf("%s", ((ASTStr*)arg)->m_Value);
		} else if(arg->m_Type == AST_NUM){
			printf("%" PRIu64, ((ASTNum*)arg)->m_Value);
		}
		arg = arg->m_Next;
	}
	printf("\n");
}

void fn_print_impl(AST* arg){
	while(arg){
		if(arg->m_Type == AST_STR){
			printf("%s", ((ASTStr*)arg)->m_Value);
		} else if(arg->m_Type == AST_NUM){
			printf("%" PRIu64, ((ASTNum*)arg)->m_Value);
		}
		arg = arg->m_Next;
	}
}

int main(int argc, const char** argv){
	g_state.m_Program = ap_chop(&argc, &argv);

	if(!argc){
		help(&g_state);
		YELL(FE_NO_INPUT_FILES);
	}

	const char** files = cvec_new(sizeof(char*), 1);
	if(!files)
		YELL(FE_OUT_OF_MEMORY);

	ap_Arg arg = ap_next(0, 0, &argc, &argv);
	while(arg.m_Status == AP_ARG_STATUS_OK){
		if(arg.m_Id < 0){
			const char* file = arg.m_Value;
			size_t len = strlen(file);
			if(len < 5 || strcmp(file + len - 4, ".trs"))
				YELL(FE_BAD_INPUT_FILE, file);
			
			cvec_addN(files, &file);
		} else{
			fprintf(stderr, "ERROR: Bad id.\n");
		}

		arg = ap_next(0, 0, &argc, &argv);
	}

	if(cvec_length(files) > MAX_FILES)
		YELL(FE_TOO_MANY_FILES);

	void* area = carea_new();
	if(!area)
		YELL(FE_OUT_OF_MEMORY);

	g_state.m_Area = area;

	Def* defs = cassoc_new(sizeof(Def), 10);
	if(!defs)
		YELL(FE_OUT_OF_MEMORY);

	g_state.m_Defs = defs;

	stdLoad(&g_state);

	printf("DEBUG: %ld\n", cvec_length(files));
	for(int i = (int)cvec_length(files) - 1; i >= 0; i--){
		printf("DEBUG: %s\n", files[i]);
		
		FILE* file = fopen(files[i], "r");
		if(!file)
			YELL(FE_FAILED_TO_OPEN_FILE, files[i], strerror(errno));

		g_state.m_FileName = files[i];

		AST* ast = parsef(&g_state, file);
		if(g_state.m_Errno){
			if(g_state.m_Errno < E_LEVEL_MIN)
				YELL(g_state.m_Errno, g_state.m_ErrorArgs[0], g_state.m_ErrorArgs[1]);
			else
				yellp(g_state.m_Errno, g_state.m_FileName, g_state.m_LineNo, g_state.m_ColNo, g_state.m_ErrorArgs[0], g_state.m_ErrorArgs[1]);
		}

		evalAST(&g_state, ast);

		fclose(file);

		carea_free(area);
	}

	cvec_free(files);

	freeState(&g_state);

	return 0;
}
