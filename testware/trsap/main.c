#include <stdio.h>
#include <string.h>

#include <trsap/trsap.h>

typedef struct Case{
	const char** args;
	const char** lines;
	trsap_ArgStatus endStatus;
} Case;

const char* args[] = {"a", "b", "c", "-a", "-bv", "-b=v", "-b:v", "-c", "-cv", "-c=v", "-c:v", "-dv1:v2", "-dv1=v2", "-d:v1:v2", "-d:v1=v2", "-d=v1:v2", "-d=v1=v2", "-ev1:v2", "-ev1=v2", "-e:v1:v2", "-e:v1=v2", "-e=v1:v2", "-e=v1=v2", "-ev", "-e:v", "-e=v", "--", "--aa", "--bb=v", "--bb:v", "--cc", "--cc=v", "--cc:v", "--dd:v1:v2", "--dd:v1=v2", "--dd=v1:v2", "--dd=v1=v2", "--ee:v1:v2", "--ee:v1=v2", "--ee=v1:v2", "--ee=v1=v2", "--ee:v", "--ee=v"};
const char* lines[] = {
		"Arg[-1]: \"a\" \"(null)\"",
		"Arg[-1]: \"b\" \"(null)\"",
		"Arg[-1]: \"c\" \"(null)\"",
		"Arg[aa]: \"(null)\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[cc]: \"(null)\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[-1]: \"--\" \"(null)\"",
		"Arg[aa]: \"(null)\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[bb]: \"v\" \"(null)\"",
		"Arg[cc]: \"(null)\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[cc]: \"v\" \"(null)\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[dd]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v1\" \"v2\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[ee]: \"v\" \"(null)\"",
		"Arg[aa]: \"(null)\" \"(null)\""
	};

const Case c1 = {
	args,
	lines,
	TRSAP_ARG_STATUS_EOI
};

int main(/*int argc, const char** argv*/ void){
	/* const char* program = trsap_chop(&argc, &argv); */
	/* printf("Program: %s\n", program); */

	trsap_Desc descs[] = {
		{ 0, "-1", 0 },
		{ 'a', "aa", TRSAP_ARG_TYPE_FLAG },
		{ 'b', "bb", TRSAP_ARG_TYPE_VALUE },
		{ 'c', "cc", TRSAP_ARG_TYPE_VALUE_OPTIONAL },
		{ 'd', "dd", TRSAP_ARG_TYPE_VALUE2 },
		{ 'e', "ee", TRSAP_ARG_TYPE_VALUE2_OPTIONAL }
	};

	trsap_Arg arg;

	const char** argv = c1.args;
	int argc = sizeof(args) / sizeof(const  char*);
	const char** line = c1.lines;
	char buff[255] = {0};
	size_t passed = 0;
	do{
		arg = trsap_next(5, descs + 1, &argc, &argv);
		const char* val = (arg.m_ValueLen == 0)?"(null)":arg.m_Value;
		size_t vallen = (arg.m_ValueLen == 0)?6:arg.m_ValueLen;
		sprintf(buff, "Arg[%s]: \"%.*s\" \"%s\"", descs[arg.m_Id + 1].m_Long, vallen, val, arg.m_Value2);
		if(strcmp(buff, *line) == 0)
			passed++;
		else printf("FAILED: %s\nNOTE: %s\n\n", buff, *line);
		line++;
	} while(arg.m_Status == TRSAP_ARG_STATUS_OK);

	if(passed == line - c1.lines && arg.m_Status == c1.endStatus){
		printf("OK\n");
	} else{
		fprintf(stderr, "%ls/%ld\n", passed, (long)(line - c1.lines));
		printf("FAILED\n");
	}
}

