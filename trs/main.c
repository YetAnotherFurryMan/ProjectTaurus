#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <toollib/ap/ap.h>
#include <toollib/cvec/cvec.h>
#include <toollib/carea/carea.h>

#include <trs/ast.h>

#define MAX_FILES 1000

const char* g_program;

void help();

int main(int argc, const char** argv){
	g_program = ap_chop(&argc, &argv);

	if(!argc){
		help();
		fprintf(stderr, "ERROR: No input files.\n");
		return 1;
	}

	const char** files = cvec_new(sizeof(char*), 1);
	if(!files){
		fprintf(stderr, "ERROR: Out of memeory.\n");
		return 1;
	}

	ap_Arg arg = ap_next(0, 0, &argc, &argv);
	while(arg.m_Status == AP_ARG_STATUS_OK){
		if(arg.m_Id < 0){
			const char* file = arg.m_Value;
			size_t len = strlen(file);
			if(len < 5){
				fprintf(stderr, "ERROR: File name to short (%s).\n", file);
				return -1;
			} else if(strcmp(file + len - 4, ".trs")){
				fprintf(stderr, "ERROR: Unknoen extension (%s).\n", file);
				return -1;
			}
			cvec_addN(files, &file);
		} else{
			fprintf(stderr, "ERROR: Bad id.\n");
		}

		arg = ap_next(0, 0, &argc, &argv);
	}

	if(cvec_length(files) > MAX_FILES){
		fprintf(stderr, "ERROR: Too many files.\n");
		return 1;
	}

	void* area = carea_new();
	if(!area){
		fprintf(stderr, "ERROR: Out of memory.\n");
		return 1;
	}

	printf("DEBUG: %ld\n", cvec_length(files));
	for(int i = (int)cvec_length(files) - 1; i >= 0; i--){
		printf("DEBUG: %s\n", files[i]);
		
		FILE* file = fopen(files[i], "r");
		if(!file){
			fprintf(stderr, "ERROR: Failed to open file %s - %s\n", files[i], strerror(errno));
			return 1;
		}

		AST* ast = parsef(area, file);
		ASTBlock* block = (ASTBlock*)ast;
		ast = block->m_Insides;
		while(ast){
			if(ast->m_Type == AST_CALL){
				ASTCall* call = (ASTCall*)ast;
				if(strcmp(call->m_Name, "println") == 0){
					AST* arg = call->m_Args;
					while(arg){
						if(arg->m_Type == AST_STR){
							printf("%s", ((ASTStr*)arg)->m_Value);
						}
						arg = arg->m_Next;
					}
					printf("\n");
				}
			} else{
				printf("DEBUG: %d\n", ast->m_Type);
			}
			ast = ast->m_Next;
		}
		// int result = eval(&state, ast);

		fclose(file);

		carea_free(area);
	}

	carea_freeHard(area);
	cvec_free(files);

	return 0;
}
