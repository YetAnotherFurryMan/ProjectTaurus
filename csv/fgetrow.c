#define _GNU_SOURCE
#include <csv/csv.h>

#include <stdlib.h>

csv_Row csv_fgetrow(FILE* f, char delimeter){
	char* line = 0;
	size_t len = 0;
	getline(&line, &len, f);

	csv_Row row = csv_parseRow2(line, delimeter);
	
	free(line);

	return row;
}
