#ifndef _CSV_H_
#define _CSV_H_

#include <stdio.h>
#include <stddef.h>

typedef struct csv_Row csv_Row;

struct csv_Row{
	size_t m_Count;
	char** m_Values;
};

csv_Row csv_fgetrow(FILE* f, char delimeter);
csv_Row csv_parseRow(const char* line, char delimeter);
csv_Row csv_parseRow2(char* line, char delimeter);
void csv_freeRow(csv_Row row);

void csv_encode(csv_Row* row, char delimeter);
void csv_decode(csv_Row* row, char delimeter);

char* csv_srtrow(csv_Row row, char delimeter);

#endif // _CSV_H_
