#include <toollib/cvec.h>

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

static unsigned int hash(const char* str){
	// DJB2
	unsigned long h = 5381;

	char c;
	while((c = *str)){
		h = (h << 5) + h + c; // h * 33 * c
		str++;
	}

	return (unsigned int)(h % -1L);
}

void cassoc_set(cassoc vec, const char* key, void* val){
	if(!vec)
		return;

	cassoc_Header* head = &((cassoc_Header*)vec)[-1];

	unsigned int h = hash(key);

	// TODO: use binary search maybe
	size_t index = 0;
	while(index < head->m_Count && h > head->m_Keys[index].m_Hash)
		index++;
	
	while(index < head->m_Count && h == head->m_Keys[index].m_Hash && strcmp(key, head->m_Keys[index].m_Key) != 0)
		index++;

	if(index < head->m_Count && h == head->m_Keys[index].m_Hash){
		index = head->m_Keys[index].m_Index;
		memcpy(cvec_get(vec, index), val, cvec_element(vec));
	} else{
		cassoc_Key k = {h, key, cvec_length(vec)};
		cvec_insertN(head->m_Keys, index, &k);
		cvec_addN(vec, val);
	}
}

void* cassoc_get(cassoc vec, const char* key){
	if(!vec)
		return NULL;

	cassoc_Header* head = &((cassoc_Header*)vec)[-1];

	unsigned int h = hash(key);

	// TODO: use binary search here too, if it has sense
	size_t index = 0;
	while(index < head->m_Count && h > head->m_Keys[index].m_Hash)
		index++;
	
	while(index < head->m_Count && h == head->m_Keys[index].m_Hash && strcmp(key, head->m_Keys[index].m_Key) != 0)
		index++;

	if(index >= head->m_Count || head->m_Keys[index].m_Hash != h || strcmp(head->m_Keys[index].m_Key, key) != 0)
		return NULL;

	index = head->m_Keys[index].m_Index;
	return cvec_get(vec, index);
}

