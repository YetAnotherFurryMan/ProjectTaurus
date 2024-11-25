# Taurus Toollib ASSOC

This is the documentation of assoc library that is a simple implementation of associative arrays for C.

How does it work:
 It works similar to vec in many ways. You can use basic assoc_* functions if you do not care for types,
or you can use assoc_GEN_FOR_TYPE to generate function for specific types.

## Header

1. Type assoc is an equivalent for void* and should be used for storing return value of assoc_new.

``` C
typedef void* assoc;
```

2. struct assoc_Key holds data about a single key and its value;

``` C
struct assoc_Key{
	uint64_t hash;
	char* key;
	void* data;
};
```

3. struct assoc_Header is a header of every assoc storing information about capacity, amount (count) of elements and an array of keys.

``` C
struct assoc_Header{
	size_t count;
	size_t cap;
	assoc_Key keys[];
};
```

4. assoc_new is a function that allocates a new assoc and its initial capacity.

``` C
assoc assoc_new(size_t count);
```

5. assoc_set is a function that allocates a new value for given key, appends new if already not found. Returns a pointer to allocated data.

``` C
void* assoc_set(assoc* a, const char* key, size_t size);
```

6. assoc_find gives needed functionality for more commonly used functions by simply searching for given key and returning pointer to it or null.

``` C
assoc_Key* assoc_find(assoc a, const char* key);
```

7. assoc_free frees all allocated by assoc_* function data.

``` C
void assoc_free(assoc a);
```

8. assoc_get is an inline function that returns a pointer to value associated with the given key or null.

``` C
static inline void* assoc_get(assoc a, const char* key);
```

9. assoc_has returns true is assoc has given key.

``` C
static inline bool assoc_has(assoc a, const char* key);
```

10. assoc_keyAt is a function that may be used for iterating assoc's keys. Returns a pointer to the key or null.

``` C
static inline assoc_Key* assoc_keyAt(assoc a, size_t index);
```

11. assoc_valueAt is a similar to assoc_keyAt function but returns pointer to the value or null.

``` C
static inline void* assoc_valueAt(assoc a, size_t index);
```

12. assoc_GEN_FOR_TYPE is a macro that generates assoc functions (assoc_new_T, assoc_set_T, assoc_find_T, assoc_free_T, assoc_get(OrDefault)_T, assoc_has_T, assoc_keyAt_T, assoc_valueAt(OrDefault)_T) and assoc_KeyT structure for given type T, where OrDefault variands returns T or def parameter instead or null.

``` C
#define assoc_GEN_FOR_TYPE(T) ...
```
