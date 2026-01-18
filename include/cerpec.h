#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <stddef.h>

// define chunk size to linearly expand and contract all data structures
#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#elif
#   error "Chunk size must be greater than zero."
#endif

typedef void * (*alloc_fn)   (const size_t, void *);
typedef void * (*realloc_fn) (void *, const size_t, void *);
typedef void   (*free_fn)    (void *, void *);

typedef struct memory {
    void * arguments;
    alloc_fn alloc;
    realloc_fn realloc;
    free_fn free;
} memory_s;

extern const memory_s standard;

typedef void   (*set_fn)     (void * const element);
typedef void * (*copy_fn)    (void * const destination, void const * const source);
typedef size_t (*hash_fn)    (void const * const element);
typedef int    (*compare_fn) (void const * const a, void const * const b);
typedef bool   (*filter_fn)  (void const * const element, void * const arguments);
typedef bool   (*handle_fn)  (void * const element, void * const arguments);
typedef void   (*process_fn) (void * const array, size_t const lenght, void * const arguments);
typedef void   (*operate_fn) (void * const result, void const * const a, void const * const b);

memory_s compose_memory(alloc_fn const alloc, realloc_fn const realloc, free_fn const free, void * const arguments);

#endif // CERPEC_H
