#include <toollib/cvec/cvec.h>

#include <string.h>

void* cvec_add(void* vec, void* val){
	vec = cvec_ensure(vec, 1);

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	memcpy(vec + (head->m_Count * head->m_Element), val, head->m_Element);
	head->m_Count++;

	return vec;
}

void* cvec_addArray(void* vec, void* arr, size_t n){
	vec = cvec_ensure(vec, n);

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	memcpy(vec + (head->m_Count * head->m_Element), arr, head->m_Element * n);
	head->m_Count += n;

	return vec;
}

