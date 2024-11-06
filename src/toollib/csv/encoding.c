#include <toollib/csv.h>

#include <stdlib.h>

void csv_encode(csv_Row* row, char delimeter){
	for(size_t i = 0; i < row->m_Count; i++){
		size_t len = 0;
		size_t count = 0;
		char* it1 = row->m_Values[i];
		while(*it1){
			if(*it1 == '\n' || *it1 == '\r' || *it1 == '\\' || *it1 == delimeter)
				count++;
			it1++;
			len++;
		}

		if(!count)
			continue;

		char* str = (char*) malloc(len + count + 1);
		
		if(!str)
			return;

		it1 = row->m_Values[i];
		char* it2 = str;
		while(*it1){
			if(*it1 == '\n' || *it1 == '\r' || *it1 == '\\' || *it1 == delimeter){
				*it2 = '\\';
				it2++;
			}

			if(*it1 == '\n')
				*it2 = 'n';
			else if(*it1 == '\r')
				*it2 = 'r';
			else if(*it1 == delimeter)
				*it2 = delimeter;
			else
				*it2 = *it1;

			it2++;
			it1++;
		}

		*it2 = 0;

		free(row->m_Values[i]);
		row->m_Values[i] = str;
	}
}

void csv_decode(csv_Row* row, char delimeter){
	for(size_t i = 0; i < row->m_Count; i++){
		char* src = row->m_Values[i];
		char* dst = row->m_Values[i];

		while(*src){
			if(*src == '\\'){
				src++;
				if(*src == 'n')
					*dst = '\n';
				else if(*src == 'd')
					*dst = delimeter;
				else
					*dst = *src;
			} else{
				*dst = *src;
			}
			
			src++;
			dst++;
		}

		*dst = 0;
	}
}

