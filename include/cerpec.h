#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <stddef.h>

// define chunk size to linearly expand and contract all data structures
#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#endif

typedef void   (*destroy_fn) (void * element, const size_t size);
typedef void * (*copy_fn)    (void * destination, const void * source, const size_t size);
typedef int    (*compare_fn) (const void * one, const void * two, const size_t size);
typedef bool   (*operate_fn) (void * element, const size_t size, void * arguments);
typedef void   (*manage_fn)  (void * array, const size_t count, const size_t size, void * arguments);

#endif // CERPEC_H
