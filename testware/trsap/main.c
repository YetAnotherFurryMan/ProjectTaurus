#include <stdio.h>

#include <trsap/trsap.h>

int main(int argc, const char** argv){
	const char* program = trsap_chop(&argc, &argv);
	printf("Program: %s\n", program);

	trsap_Desc descs[] = {
		{ 0, "-1", 0 },
		{ 'a', "aa", TRSAP_ARG_TYPE_FLAG },
		{ 'b', "bb", TRSAP_ARG_TYPE_VALUE },
		{ 'c', "cc", TRSAP_ARG_TYPE_VALUE_OPTIONAL },
		{ 'd', "dd", TRSAP_ARG_TYPE_VALUE2 },
		{ 'e', "ee", TRSAP_ARG_TYPE_VALUE2_OPTIONAL }
	};

	trsap_Arg arg;

	do{
		arg = trsap_next(5, descs + 1, &argc, &argv);
		printf("Arg[%s]: \"%s\" \"%s\"\n", descs[arg.m_Id + 1].m_Long, arg.m_Value, arg.m_Value2);
	} while(arg.m_Status == TRSAP_ARG_STATUS_OK);

	printf("END[%d]\n", arg.m_Status);
}

