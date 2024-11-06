#include <toollib/vec.h>

#include <stdlib.h>
#include <string.h>

static inline vec_Header* vec_realloc(vec_Header* head, size_t size){
	size_t ns = head->cap * 2 + size;
	head = realloc(head, ns + sizeof(vec_Header));
	if(head) 
		head->cap = ns;
	return head;
}

vec vec_new(size_t cap){
	vec_Header* head = malloc(sizeof(vec_Header) + cap);
	head->size = 0;
	head->cap = cap;
	return head->data;
}

void* vec_push(vec* v, size_t size){
	if(!v)
		return NULL;

	vec_Header* head = vec_GET_HEAD(*v);

	if(head->size + size <= head->cap){
		char* data = head->data + head->size;
		head->size += size;
		return data;
	}

	head = vec_realloc(head, size);
	if(!head)
		return NULL;

	char* data = head->data + head->size;
	head->size += size;
	*v = head->data;
	return data;
}

void vec_pop(vec v, size_t size){
	if(!v)
		return;
	
	vec_Header* head = vec_GET_HEAD(v);
	if(head->size < size)
		head->size = 0;
	else
		head->size -= size;
}

void* vec_insert(vec* v, size_t offset, size_t size){
	if(!v)
		return NULL;

	vec_Header* head = vec_GET_HEAD(*v);

	if(offset > head->size)
		return NULL;
	else if(offset == head->size)
		return vec_push(v, size);

	if(head->size + size <= head->cap){
		char* data = head->data + offset;
		memmove(data + size, data, head->size - offset);
		head->cap += size;
		return data;
	}

	head = vec_realloc(head, size);
	if(!head)
		return NULL;

	char* data = head->data + offset;
	memmove(data + size, data, head->size - offset);
	head->size += size;
	*v = head->data;
	return data;
}

void vec_delete(vec v, size_t offset, size_t size){
	if(!v)
		return;

	vec_Header* head = vec_GET_HEAD(v);
	if(head->size < offset || head->size - offset < size)
		return;

	head->size -= size;
	memmove(head->data + offset, head->data + offset + size, head->size - offset);
}

void vec_free(vec v){
	vec_Header* head = vec_GET_HEAD(v);
	free(head);
}
