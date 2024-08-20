#include <toollib/cvec/cvec.h>

#include <string.h>

void* cvec_add(void* vec, void* val){
	vec = cvec_ensure(vec, 1);

	if(!vec)
		return NULL;

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	if(val)
		memcpy(((char*)vec) + (head->m_Count * head->m_Element), val, head->m_Element);
	
	head->m_Count++;

	return vec;
}

void* cvec_addArray(void* vec, void* arr, size_t n){
	vec = cvec_ensure(vec, n);

	if(!vec)
		return NULL;

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	if(arr)
		memcpy(((char*)vec) + (head->m_Count * head->m_Element), arr, head->m_Element * n);

	head->m_Count += n;

	return vec;
}

