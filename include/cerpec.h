#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

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

typedef void * (*alloc_fn)   (size_t const size, void * arg);
typedef void * (*realloc_fn) (void * pointer, size_t const size, void * arg);
typedef void   (*free_fn)    (void * pointer, void * arg);

typedef struct memory {
    void * arg;
    alloc_fn alloc;
    realloc_fn realloc;
    free_fn free;
} memory_s;

/// @brief Compose custom memory allocator structure.
/// @param alloc Allocate memory based on size and arguments (like malloc).
/// @param realloc Reallocate memory based on initial memory, size and arguments (like realloc).
/// @param free Free memory based on size and arguments (like free).
/// @param arg Arguments for alloc, realloc and free function pointers.
/// @return Memory allocator structure.
memory_s compose_memory(alloc_fn const alloc, realloc_fn const realloc, free_fn const free, void * const arg);

/// @brief Cerpec's default standard memory allocator (just malloc, realloc and free).
extern const memory_s standard;

typedef void   (*set_fn)     (void * const element, void * arg);
typedef void * (*copy_fn)    (void * const destination, void const * const source, void * arg);
typedef size_t (*hash_fn)    (void const * const element, void * arg);
typedef int    (*compare_fn) (void const * const a, void const * const b, void * arg);
typedef bool   (*filter_fn)  (void const * const element, void * arg);
typedef bool   (*handle_fn)  (void * const element, void * arg);
typedef void   (*process_fn) (void * const array, size_t const lenght, void * arg);
typedef void   (*operate_fn) (void * const result, void const * const a, void const * const b, void * arg);

#endif // CERPEC_H
