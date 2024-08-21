#ifndef _CVEC_H_
#define _CVEC_H_

#include <stddef.h>
#include <stdbool.h>

typedef void* cvec;

typedef struct cvec_Header{
	size_t m_Count;
	size_t m_Element;
	size_t m_Size;
	char m_Data[];
} cvec_Header;

typedef struct cassoc{
	cvec m_Keys;
	cvec m_Values;
} cassoc;

cvec cvec_new(size_t element, size_t n);
cvec cvec_ensure(cvec vec, size_t n);
void cvec_free(cvec vec);

cvec cvec_addArray(cvec vec, void* arr, size_t n);
cvec cvec_insertArray(cvec vec, size_t index, void* arr, size_t n);
void cvec_remove(cvec vec, size_t index);

static inline size_t cvec_length(cvec vec){
	if(!vec) return 0;
	return ((cvec_Header*)vec)[-1].m_Count;
}

static inline size_t cvec_element(cvec vec){
	if(!vec) return 0;
	return ((cvec_Header*)vec)[-1].m_Element;
}

static inline size_t cvec_size(cvec vec){
	if(!vec) return 0;
	return ((cvec_Header*)vec)[-1].m_Size;
}

static inline bool cvec_needsRealloc(cvec vec, size_t n){
	cvec_Header head = ((cvec_Header*)vec)[-1];
	return (head.m_Size - (head.m_Element * head.m_Count)) < (head.m_Element * n);
}

static inline cvec cvec_add(cvec vec, void* val){
	return cvec_addArray(vec, val, 1);
}

static inline cvec cvec_insert(cvec vec, size_t index, void* val){
	return cvec_insertArray(vec, index, val, 1);
}

static inline void* cvec_get(cvec vec, size_t index){
	return ((char*)vec) + (((cvec_Header*)vec)[-1].m_Element * index);
}

#define cvec_ensureN(VEC, N) VEC = cvec_ensure(VEC, N)
#define cvec_addN(VEC, VAL) VEC = cvec_add(VEC, VAL)
#define cvec_addArrayN(VEC, ARR, N) VEC = cvec_addArray(VEC, ARR, N)
#define cvec_insertN(VEC, IDX, VAL) VEC = cvec_insert(VEC, IDX, VAL)
#define cvec_insertArrayN(VEC, IDX, ARR, N) VEC = cvec_insertArray(VEC, IDX, ARR, N)

#endif // _CVEC_H_
