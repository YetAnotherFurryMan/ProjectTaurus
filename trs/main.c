#include <stdio.h>
#include <string.h>

#include <toollib/ap/ap.h>
#include <toollib/cvec/cvec.h>

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

	printf("DEBUG: %ld\n", cvec_length(files));
	for(size_t i = cvec_length(files); i > 0; i--){
		printf("DEBUG: %s\n", files[i - 1]);
	}

	cvec_free(files);

	return 0;
}
