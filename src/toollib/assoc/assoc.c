#include <toollib/assoc.h>

#include <toollib/toollib.h>

assoc assoc_new(size_t count){
	size_t cap = count * sizeof(assoc_Key);
	assoc_Header* head = malloc(sizeof(assoc_Header) + cap);
	if(!head)
		return NULL;

	memset(head, 0, sizeof(assoc_Header));
	head->cap = count;

	return head;
}

void* assoc_set(assoc* a, const char* key, size_t size){
	if(!a)
		return NULL;

	assoc_Header* head = *a;

	void* data = malloc(size);
	if(!data)
		return NULL;

	assoc_Key* k = assoc_find(head, key);
	if(!k){
		if(head->count >= head->cap){
			size_t cap = (head->cap + 1) * 2;
			head = realloc(head, sizeof(assoc_Header) + (cap * sizeof(assoc_Key)));
			if(!head){
				free(data);
				return NULL;
			}

			head->cap = cap;
			*a = head;
		}

		char* key_cpy = tl_strcpy(key);
		if(!key_cpy){
			free(data);
			return NULL;
		}

		uint64_t h = tl_hash(key);

		size_t ix = 0;
		while(ix < head->count && head->keys[ix].hash < h)
			ix++;

		if(ix < head->count)
			memmove(&head->keys[ix + 1], &head->keys[ix], (head->count - ix) * sizeof(assoc_Key));
		
		head->keys[ix].hash = h;
		head->keys[ix].key = key_cpy;
		head->keys[ix].data = data;

		return data;
	}

	free(k->data);
	k->data = data;
	return data;
}

assoc_Key* assoc_find(assoc a, const char* key){
	if(!a)
		return NULL;

	assoc_Header* head = a;
	if(!head->count)
		return NULL;

	uint64_t h = tl_hash(key);
	size_t p = 0;
	size_t q = head->count - 1;
	while(p < q){
		size_t c = (p + q) / 2;
		if(head->keys[c].hash < h){
			q = c;
		} else if(head->keys[c].hash > h){
			p = c;
		} else{
			while(c > 0 && head->keys[c].hash == h) 
				c--;

			if(head->keys[c].hash != h)
				c++;

			while(head->keys[c].hash == h){
				if(strcmp(head->keys[c].key, key) == 0)
					return &head->keys[c];
			}

			return NULL;
		}
	}

	if(head->keys[p].hash == h && strcmp(head->keys[p].key, key) == 0)
		return &head->keys[p];

	return NULL;
}

void assoc_free(assoc a){
	if(!a)
		return;

	assoc_Header* head = a;

	for(size_t i = 0; i < head->count; i++){
		free(head->keys[i].key);
		free(head->keys[i].data);
	}
	
	free(a);
}
