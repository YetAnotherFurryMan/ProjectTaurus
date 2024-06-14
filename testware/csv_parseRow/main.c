#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <csv/csv.h>

typedef struct{
	const char delimeter;
	const char* line;
	const csv_Row row;
	int strcmp;
} Case;

char* case0v[3] = {"a", "b", "c"};
char* case1v[3] = {"a", "b", "d"};
char* case2v[4] = {"ala", "ma", "bialego", "kota"};

const Case cases[3] = {
	{'|', "a|b|c\n", {3, case0v}, 0},
	{';', "a;b;d\n", {3, case1v}, 0},
	{'\t', "ala\tma\tbialego\tkota\na\tbialy\tkot\tma\tale\n", {4, case2v}, -'a'}
};

#define CASE_COUNT (sizeof(cases) / sizeof(Case))

int main(void){
	size_t passed = 0;
	for(size_t casei = 0; casei < CASE_COUNT; casei++){
		csv_Row r = csv_parseRow(cases[casei].line, cases[casei].delimeter);
		if(r.m_Count == cases[casei].row.m_Count){
			size_t passed_cols = 0;
			for(size_t i = 0; i < r.m_Count; i++){
				if(strcmp(r.m_Values[i], cases[casei].row.m_Values[i]) == 0)
					passed_cols++;
				else
					fprintf(stderr, "FAILED(case %ld): Column dismatch \'%s\' != \'%s\'\n", casei, r.m_Values[i], cases[casei].row.m_Values[i]);
			}

			char* str = csv_strrow(r, cases[casei].delimeter);
			if(strcmp(str, cases[casei].line) == cases[casei].strcmp){
				if(passed_cols == r.m_Count)
					passed++;
			} else{
				fprintf(stderr, "FAILED(case %ld): csv_strrow dismatch \'%s\' != \'%s\'\n", casei, str, cases[casei].line);
			}

			free(str);
		} else {
			fprintf(stderr, "FAILED(case %ld): Column count dismatch %ld != %ld\n", casei, r.m_Count, cases[casei].row.m_Count);
		}

		csv_freeRow(r);
	}

	if(passed != CASE_COUNT)
		printf("FAILED\n");
	else
		printf("OK\n");

	return 0;
}
