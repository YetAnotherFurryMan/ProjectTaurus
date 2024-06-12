#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <csv/csv.h>

typedef struct{
	const char* line;
	csv_Row row;
} Case;

const char* c1v[3] = {"a", "b", "c"};
const Case c1 = {
	"a|b|c\n",
	{
		3,
		c1v
	}
};

int main(void){
	csv_Row r = csv_parseRow(c1.line, '|');
	if(r.m_Count == c1.row.m_Count){
		size_t passed = 0;
		for(size_t i = 0; i < r.m_Count; i++){
			if(strcmp(r.m_Values[i], c1.row.m_Values[i]) == 0)
				passed++;
		}
		char* str = csv_strrow(r, '|');
		if(passed == r.m_Count && strcmp(str, c1.line) == 0)
			printf("OK\n");
		else
			printf("%s%d/%d\nFAILED\n", str, passed, r.m_Count);
		free(str);
	} else{
		printf("FAILED\n");
	}
	csv_freeRow(r);
	return 0;
}
