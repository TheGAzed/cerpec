#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define CERPEC_FACTOR 2

// define chunk size to expand and contract all data structures
#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#elif CERPEC_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (CERPEC_CHUNK & (CERPEC_CHUNK - 1))
#   error "Chunk size must be a power of 2."
#endif



#ifdef NDEBUG
#   define NVALID
#   define NERROR
#endif

#ifndef NVALID
#   define valid(condition) assert(condition)
#else
#   define valid(condition) (void)(0)
#endif

#ifndef NERROR
#   define error(condition) assert(condition)
#else
#   define error(condition) (void)(0)
#endif

typedef void * (*alloc_fn)   (size_t const, void *);
typedef void * (*realloc_fn) (void *, size_t const, void *);
typedef void   (*free_fn)    (void *, void *);

typedef struct memory {
    void * arguments;
    alloc_fn alloc;
    realloc_fn realloc;
    free_fn free;
} memory_s;

memory_s compose_memory(alloc_fn const alloc, realloc_fn const realloc, free_fn const free, void * const arguments);

extern const memory_s standard;

typedef void   (*set_fn)     (void * const element);
typedef void * (*copy_fn)    (void * const destination, void const * const source);
typedef size_t (*hash_fn)    (void const * const element);
typedef int    (*compare_fn) (void const * const a, void const * const b);
typedef bool   (*filter_fn)  (void const * const element);
typedef bool   (*handle_fn)  (void * const element, void * const arguments);
typedef void   (*process_fn) (void * const array, size_t const lenght, void * const arguments);
typedef void   (*operate_fn) (void * const result, void const * const a, void const * const b);

#define INVALID_ITERATOR (size_t)(-1)

typedef struct uni_directional_iterator {
    void const * structure;
    uintptr_t meta;
    size_t index;
} uniter_s;

typedef struct bi_directional_iterator {
    void * structure;
    uintptr_t meta;
    size_t index;
} biter_s;

bool is_valid_uniter(uniter_s const * const iterator);

bool is_valid_biter(biter_s const * const iterator);

#endif // CERPEC_H
