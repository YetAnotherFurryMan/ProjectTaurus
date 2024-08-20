#include <toollib/cvec/cvec.h>

#include <malloc.h>

void* cvec_new(size_t element, size_t n){
	size_t basesize = element * n;

	if(basesize == 0)
		return NULL;

	cvec_Header* head = malloc(basesize + sizeof(cvec_Header));
	head->m_Count = 0;
	head->m_Element = element;
	head->m_Size = basesize;

	return head->m_Data;
}

void cvec_free(void* vec){
	if(!vec) 
		return;

	cvec_Header* head = &((cvec_Header*)vec)[-1];
	free(head);
}

void* cvec_ensure(void* vec, size_t n){
	if(!vec)
		return NULL;

	if(!cvec_needsRealloc(vec, n))
		return vec;

	cvec_Header* head = &((cvec_Header*)vec)[-1];
	
	size_t nn = n > head->m_Count ? n : head->m_Count;
	size_t newBasesize = head->m_Size + (nn * head->m_Element);

	head = realloc(head, newBasesize + sizeof(cvec_Header));
	head->m_Size = newBasesize;

	return head->m_Data;
}

