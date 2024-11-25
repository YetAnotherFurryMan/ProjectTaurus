# Taurus Toollib PGM

This is the documentation of pgm library that is a simple implementation of area/page allocator for C.

How does it work:
 First you must create pgm handle (pgm_new), it will allocate one page of memory (PGM_PAGE_SIZE).
Then, when you need some memory, you may ask pgm for the amount you need, it will return a pointer
to your memory and allocate new page if needed. You can clean pgm if the data is no longer needed,
but you want to reuse pgm itself or free pgm otherwise.

## Header

1. Type pgm is an equivalent for void* and should be used for storing return value of pgm_new.

``` C
typedef void* pgm;	
```

2. struct pgm_Header is a header of every page of pgm. It stores information about size of allocation, capacity of page, pointer to next page (or NULL), and data. It is always the first data allocated in pgm.

``` C
struct pgm_Header{
	size_t size;
	size_t cap;
	pgm_Header* next;
	char data[];
};
```

3. pgm_new is a function that allocates the first page of memory and returns newly created pgm. On failure returns null.

``` C
pgm pgm_new(void);
```

4. pgm_alloc is a function that tries to find memory in already allocated pages and return it. If it fails, it is allocating a new page (PGM_PAGE_SIZE or more if needed) and returns pointer to it or null.

``` C
void* pgm_alloc(pgm p, size_t size);
```

5. pgm_clean is a function that unmarks any user-requested memory from being allocated.

``` C
void pgm_clean(pgm p);
```

6. pgm_free is a function that permanently frees every page in the pgm.

``` C
void pgm_free(pgm p);
```

7. pgm_allocT(N) are macros that call pgm_alloc and casts result to a pointer to T. The variant with N allocates memory for N elements of type T.

``` C
#define pgm_allocT(P, T) ((T*) pgm_alloc(P, sizeof(T)))
#define pgm_allocTN(P, T, N) ((T*) pgm_alloc(P, sizeof(T) * N))
```
