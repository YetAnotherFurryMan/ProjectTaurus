#ifndef _ASSOC_H_
#define _ASSOC_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void* assoc;
typedef struct assoc_Key assoc_Key;
typedef struct assoc_Header assoc_Header;

struct assoc_Key{
	uint64_t hash;
	char* key;
	void* data;
};

struct assoc_Header{
	size_t count;
	size_t cap;
	assoc_Key keys[];
};

assoc assoc_new(size_t count);
void* assoc_set(assoc* a, const char* key, size_t size);
assoc_Key* assoc_find(assoc a, const char* key);
void assoc_free(assoc a);

static inline void* assoc_get(assoc a, const char* key){
	assoc_Key* k = assoc_find(a, key);
	if(!k)
		return NULL;

	return k->data;
}

static inline bool assoc_has(assoc a, const char* key){
	return assoc_find(a, key) != NULL;
}

static inline assoc_Key* assoc_keyAt(assoc a, size_t index){
	if(!a)
		return NULL;

	assoc_Header* head = (assoc_Header*)a;
	if(head->count <= index)
		return NULL;

	return &head->keys[index];
}

static inline void* assoc_valueAt(assoc a, size_t index){
	if(!a)
		return NULL;

	assoc_Header* head = (assoc_Header*)a;
	if(head->count <= index)
		return NULL;

	return head->keys[index].data;
}

// TODO: assoc_GEN_FOR_TYPE

#endif // _ASSOC_H_
