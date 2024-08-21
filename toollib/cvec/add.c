#include <toollib/cvec/cvec.h>

#include <string.h>

cvec cvec_addArray(cvec vec, void* arr, size_t n){
	vec = cvec_ensure(vec, n);

	if(!vec)
		return NULL;

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	if(arr)
		memcpy(((char*)vec) + (head->m_Count * head->m_Element), arr, head->m_Element * n);

	head->m_Count += n;

	return vec;
}

cvec cvec_insertArray(cvec vec, size_t index, void* arr, size_t n){
	vec = cvec_ensure(vec, n);

	if(!vec)
		return NULL;

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	if(head->m_Count <= index){
		if(arr)
			memcpy(((char*)vec) + (head->m_Count * head->m_Element), arr, head->m_Element * n);
	} else{
		memcpy(((char*)vec) + ((index + n) * head->m_Element), ((char*)vec) + (index * head->m_Element), head->m_Element * (head->m_Count - index));

		if(arr)
			memcpy(((char*)vec) + (index * head->m_Element), arr, head->m_Element * n);
	}

	head->m_Count += n;

	return vec;
}

void cvec_remove(cvec vec, size_t index){
	if(!vec)
		return;

	cvec_Header* head = &((cvec_Header*)vec)[-1];

	if(head->m_Count <= index)
		return;

	if(index < head->m_Count - 1)
		memcpy(((char*)vec) + (index * head->m_Element), ((char*)vec) + ((index + 1) * head->m_Element), head->m_Element * (head->m_Count - index - 1));

	head->m_Count--;
}

void cassoc_set(cassoc vec, const char* key, void* val){
	// TODO
}

void* cassoc_get(cassoc vec, const char* key){
	// TODO
	return NULL;
}

