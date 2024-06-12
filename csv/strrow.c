#define _GNU_SOURCE
#include <csv/csv.h>

#include <stdlib.h>
#include <string.h>

char* csv_strrow(const csv_Row row, char delimeter){
	size_t len = row.m_Count + 1;
	for(size_t i = 0; i < row.m_Count; i++){
		len += strlen(row.m_Values[i]);
	}

	char* str = (char*) malloc(len);
	if(!str)
		return str;

	char* cur = str;
	for(size_t i = 0; i < row.m_Count; i++){
		cur = (char*) mempcpy(cur, row.m_Values[i], strlen(row.m_Values[i]));
		*cur = delimeter;
		cur++;
	}

	cur--;
	*cur = '\n';
	str[len] = 0;

	return str;
}
