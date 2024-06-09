#include <csv/csv.h>

#include <stdlib.h>
#include <string.h>

csv_Row csv_parseRow(const char* line, char delimeter){
	csv_Row row = {0};
	
	size_t len = strlen(line);
	char* str = (char*) malloc(len + 1);
	
	if(!str)
		return row;

	strcpy(str, line);

	row = csv_parseRow2(str, delimeter);

	free(str);

	return row;
}

csv_Row csv_parseRow2(char* line, char delimeter){
	csv_Row row = {0};

	char* it = line;
	while(*it){
		if(*it == delimeter){
			row.m_Count++;
		} else if(*it == '\n'){
			*it = 0;
			break;
		}

		it++;
	}

	row.m_Count++;
	row.m_Values = (char**) malloc(row.m_Count * sizeof(char**));

	if(!row.m_Values)
		return row;

	char delimeters[] = {delimeter, '\n', 0};
	char* tok = strtok(line, delimeters);

	size_t i = 0;
	while(tok){
		size_t len = strlen(tok);
		char* val = (char*) malloc(len + 1);

		if(!val)
			break;

		strcpy(val, tok);

		row.m_Values[i] = val;

		tok = strtok(NULL, delimeters);
		i++;
	}

	return row;
}
