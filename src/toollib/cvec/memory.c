#include <toollib/cvec.h>

#include <malloc.h>

cvec cvec_new(size_t element, size_t n){
	size_t basesize = element * n;

	if(basesize == 0)
		return NULL;

	cvec_Header* head = malloc(basesize + sizeof(cvec_Header));
	if(!head)
		return NULL;

	head->m_Count = 0;
	head->m_Element = element;
	head->m_Size = basesize;

	return head->m_Data;
}

void cvec_free(cvec vec){
	if(!vec) 
		return;

	cvec_Header* head = &((cvec_Header*)vec)[-1];
	free(head);
}

cassoc cassoc_new(size_t element, size_t n){
	size_t basesize = element * n;

	if(basesize == 0)
		return NULL;

	cassoc_Header* head = malloc(basesize + sizeof(cassoc_Header));
	if(!head)
		return NULL;

	head->m_Keys = cvec_new(sizeof(cassoc_Key), n);
	if(!head->m_Keys){
		free(head);
		return NULL;
	}

	head->m_Count = 0;
	head->m_Element = element;
	head->m_Size = basesize;

	return head->m_Data;
}

void cassoc_free(cassoc vec){
	if(!vec)
		return;

	cassoc_Header* head = &((cassoc_Header*)vec)[-1];
	cvec_free(head->m_Keys);
	free(head);
}

#define _ensure(T) {                                            \
	if(!vec)                                                    \
		return NULL;                                            \
	if(!cvec_needsRealloc(vec, n))                              \
		return vec;                                             \
	T* head = &((T*)vec)[-1];                                   \
	size_t nn = n > head->m_Count ? n : head->m_Count;          \
	size_t newBasesize = head->m_Size + (nn * head->m_Element); \
	head = realloc(head, newBasesize + sizeof(T));              \
	head->m_Size = newBasesize;                                 \
	return head->m_Data;                                        \
}

cvec cvec_ensure(cvec vec, size_t n) _ensure(cvec_Header)
cassoc cassov_ensure(cassoc vec, size_t n) _ensure(cassoc_Header)

