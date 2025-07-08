#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <stddef.h>

// define chunk size to linearly expand and contract all data structures
#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#endif

typedef void   (*destroy_fn) (void * e, const size_t size);
typedef void * (*copy_fn)    (void * dest, const void * src, const size_t size);
typedef int    (*compare_fn) (const void * a, const void * b, const size_t size);
typedef bool   (*operate_fn) (void * e, const size_t size, void * args);
typedef void   (*manage_fn)  (void * arr, const size_t n, const size_t size, void * args);

#endif // CERPEC_H
