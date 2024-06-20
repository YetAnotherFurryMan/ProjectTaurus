#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <csv/csv.h>

typedef struct{
	const char delimeter;
	const char* line;
	const size_t row_count;
	const csv_Row* rows;
} Case;

const Case cases[3] = {
	{'|', "a|b|c\n", 1,
		(const csv_Row[]){
			{3, (char*[]){"a", "b", "c"}}
		}
	}, {';', "a;b;d\n", 1, 
		(const csv_Row[]){
			{3, (char*[]){"a", "b", "d"}}
		}
	}, {'\t', "ala\tma\tbialego\tkota\na\tbialy\tkot\tma\tale\n", 2,
		(const csv_Row[]){
			{4, (char*[]){"ala", "ma", "bialego", "kota"}},
			{5, (char*[]){"a", "bialy", "kot", "ma", "ale"}}
		}
	}
};

#define CASE_COUNT (sizeof(cases) / sizeof(Case))

int main(int argc, const char** argv){
	char* tmp_path = malloc(strlen(argv[0]) + 5);
	*tmp_path = 0;

	strcat(tmp_path, argv[0]);
	strcat(tmp_path, ".csv");

	size_t passed = 0;
	for(size_t casei = 0; casei < CASE_COUNT; casei++){
		char delim = cases[casei].delimeter;
		const csv_Row* rows = cases[casei].rows;
		const char* line = cases[casei].line;
		size_t line_len = strlen(line);
		
		FILE* tmp = fopen(tmp_path, "w");
		if(!tmp){
			fprintf(stderr, "FAILED(io): Cannot open tmp file '%s': %s\n", tmp_path, strerror(errno));
			printf("FAILED\n");
			return -1;
		}

		fputs(line, tmp);
		fclose(tmp);

		FILE* file = fopen(tmp_path, "r");
		if(!file){
			fprintf(stderr, "FAILED(io): Cannot open file '%s': %s\n", tmp_path, strerror(errno));
			printf("FAILED\n");
			return -1;
		}

		char* str = malloc(line_len + 1);
		*str = 0;

		size_t passed_rows = 0;

		csv_Row r = csv_fgetrow(file, delim);
		size_t rowi = 0;
		while(r.m_Values){
			if(r.m_Count == rows[rowi].m_Count){
				size_t passed_cols = 0;
				for(size_t i = 0; i < r.m_Count; i++){
					if(strcmp(r.m_Values[i], rows[rowi].m_Values[i]) == 0)
						passed_cols++;
					else
						fprintf(stderr, "FAILED(case %ld): Column dismatch \'%s\' != \'%s\'\n", casei, r.m_Values[i], rows[rowi].m_Values[i]);
				}

				char* str2 = csv_strrow(r, delim);
				if(strlen(str2) + strlen(str) > line_len)
					fprintf(stderr, "FAILED(case %ld): csv_strrow length dismatch: '%s', '%s'\n", casei, str, str2);
				else
					strcat(str, str2);
				free(str2);

				if(passed_cols == r.m_Count)
					passed_rows++;
			} else {
				fprintf(stderr, "FAILED(case %ld): Column count dismatch %ld != %ld\n", casei, r.m_Count, rows[rowi].m_Count);
				csv_freeRow(r);
				break;
			}

			csv_freeRow(r);
			r = csv_fgetrow(file, delim);
			rowi++;
		}

		fclose(file);

		if(strcmp(str, line) == 0){
			if(passed_rows == cases[casei].row_count)
				passed++;
		} else{
			fprintf(stderr, "FAILED(case %ld): csv_strrow dismatch \'%s\' != \'%s\'\n", casei, str, line);
		}
		free(str);
	}

	free(tmp_path);

	if(passed != CASE_COUNT)
		printf("FAILED\n");
	else
		printf("OK\n");

	return 0;
}
