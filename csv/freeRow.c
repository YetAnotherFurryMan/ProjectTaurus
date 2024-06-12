#include <csv/csv.h>

#include <stdlib.h>

void csv_freeRow(csv_Row row){
	for(size_t i = 0; i < row.m_Count; i++){
		free(row.m_Values[i]);
	}

	free(row.m_Values);

	// Invalidate
	row.m_Count = 0;
	row.m_Values = 0;
}
