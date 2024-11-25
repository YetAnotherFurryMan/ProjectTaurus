# Taurus Toollib VEC

This is the documentation of vec library that is a simple vector implementation for C.

How does it work:
 If you want to use vec as a some kind of stack or untyped dynamic array then you will use vec_* functions as they are.
Otherwise, on top of your src file you should use vec_GEN_FOR_TYPE macro that will wrap raw vec_* functions into pretty static inlines for given type.

## Header

1. Type vec is an equivalent for void* and should be used for storing return value of vec_new.

``` C
typedef void* vec;
```

2. struct vec_Header is a header of any vec storing data about its size and capacity. It is located always before returned from vec_new pointer.

``` C
typedef struct vec_Header vec_Header;
struct vec_Header{
	size_t size;
	size_t cap;
 	char data[];
};
```

3. vec_new is a function that allocates on the heap a new vec_Header plus initial capacity that is passed as argument. Returns null on error.

``` C
vec vec_new(size_t cap);
```

4. vec_push is a function that ensures and reserves memory at the end of vec and returns a pointer to the beginning of that memory. Return null on error.

``` C
void* vec_push(vec* v, size_t size);
```

5. vec_pop is a function that frees memory at the end of vec. If the size of vec is smaller that the given size it will become null.

``` C
void vec_pop(vec v, size_t size);
```

6. vec_insert is a function that ensures and reserves memory in the middle of vec. If given offset points at the end of vec, calls vec_push, if offset is smaller than vec or the function had other problems it will return null.

``` C
void* vec_insert(vec* v, size_t offset, size_t size);
```

7. vec_delete is a function that frees memory in the middle of vec (moves data). On error does nothing.

``` C
void vec_delete(vec v, size_t offset, size_t size);
```

8. vec_free is a function that frees vector's memory. After calling this function data is permanently lost. You should call this function always in pair with vec_new.

``` C
void vec_free(vec v);
```

9. vec_GET_HEAD is a macro that takes vec and returns a pointer to its vec_Header.

``` C
#define vec_GET_HEADER(V) ...
```

10. vec_GEN_FOR_TYPE is a macro that takes a type (must be one token) and defines wrappers: vec_new_T, vec_push(N)_T, vec_pop(N)_T, vec_inser(N)_T, vec_delete(N)_T and vec_free_T, where T is the given type and (N) is optional N that means the function will take n as an argument that indicates the amount of elements to work on.

``` C
#define vec_GEN_FOR_TYPE(T) ...
```
