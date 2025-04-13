#include <trs/iron.h>

#include <dlfcn.h>

int main(int argc, const char** argv){
	if(argc != 3){
		fprintf(stderr, "ERROR: Expected exacly 2 arguments, got %d.\n", argc);
		printf("USAGE: %s [INPUT].[IRON_EXT] [OUTPUT].trslisp\n", argv[0]);
		return 1;
	}

	char iron[255] = {0};
	snprintf(iron, 255, "trsiron%s.so", strrchr(argv[1], '.'));

	dlerror();
	void* hnd = dlopen(iron, RTLD_LAZY);
	if(!hnd){
		fprintf(stderr, "ERROR: Failed to load iron: %s\n", iron);
		fprintf(stderr, "NOTE: %s\n", dlerror());
		return 1;
	}

	iron_load_t loader = (iron_load_t) dlsym(hnd, "iron_load");

	FILE* in = fopen(argv[1], "r");
	if(!in){
		fprintf(stderr, "ERROR: Failed to open input file %s\n", argv[1]);
		dlclose(hnd);
		return 1;
	}

	fseek(in, 0, SEEK_END);
	long len = ftell(in);
	rewind(in);

	char* src = malloc(len + 1);
	if(!src){
		fprintf(stderr, "ERROR: Buy more RAM...\n");
		fclose(in);
		dlclose(hnd);
		return 1;
	}

	fread(src, 1, len, in);
	src[len] = 0;

	fclose(in);

	horn_Instance instance = {0};

	if(horn_init(&instance)){
		fprintf(stderr, "ERROR: Failed to initialize horn.\n");
		free(src);
		dlclose(hnd);
		return 1;
	}

	if(loader(&instance, src)){
		fprintf(stderr, "ERROR: Failed to iron...\n");
		free(src);
		dlclose(hnd);
	}

	FILE* out = fopen(argv[2], "w");
	if(!out){
		fprintf(stderr, "ERROR: Failed to open output file %s\n", argv[2]);
		horn_freeInstance(&instance);
		free(src);
		dlclose(hnd);
		return 1;
	}

	if(horn_emit(out, &instance)){
		fprintf(stderr, "ERROR: Failed to emit.\n");
		fclose(out);
		horn_freeInstance(&instance);
		free(src);
		dlclose(hnd);
		return 1;
	}

	fclose(out);

	horn_freeInstance(&instance);
	free(src);
		
	dlclose(hnd);
	
	return 0;
}
