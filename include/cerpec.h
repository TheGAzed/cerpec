#ifndef CERPEC_H
#define CERPEC_H

#include <stdbool.h>

// define length type as unsigned int instead of size_t, which can improve performance since memory is tailored to int
#if defined(FAST_CERPEC_LENGTH)
#   include <limits.h>
typedef unsigned length_t;
#   define LENGTH_MAX UINT_MAX
#else
#include <stddef.h>
typedef size_t length_t;
#   define LENGTH_MAX ((length_t)(-1))
#endif

#if !defined(CERPEC_CHUNK)
#   define CERPEC_CHUNK 256
#endif

typedef void   (*destroy_fn) (void * element, const length_t size);
typedef void * (*copy_fn)    (void * destination, const void * source, const length_t size);
typedef int    (*compare_fn) (const void * one, const void * two, const length_t size);
typedef bool   (*operate_fn) (void * element, const length_t size, void * arguments);
typedef void   (*manage_fn)  (void * array, const length_t count, const length_t size, void * arguments);

#endif // CERPEC_H
