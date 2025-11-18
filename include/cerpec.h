#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>
#include <stddef.h>

// define chunk size to linearly expand and contract all data structures
#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#endif

typedef void   (*destroy_fn) (void * element);
typedef void * (*copy_fn)    (void * destination, const void * source);
typedef size_t (*hash_fn)    (const void * element);
typedef int    (*compare_fn) (const void * a, const void * b);
typedef bool   (*filter_fn)  (const void * element, void * arguments);
typedef bool   (*handle_fn) (void * element, void * arguments);
typedef void   (*process_fn)  (void * array, const size_t lenght, void * arguments);
typedef void   (*operate_fn)  (void * a, void * b, const void * result);

#endif // CERPEC_H
