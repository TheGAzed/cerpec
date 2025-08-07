#ifndef IHASH_SET_H
#define IHASH_SET_H

#include <cerpec.h>

#if !defined(IHASH_SET_CHUNK)
#   define IHASH_SET_CHUNK CERPEC_CHUNK
#endif

typedef struct infinite_hash_set {
    hash_fn hash;
    char * elements;
    size_t * next;
    size_t * head;
    size_t size, empty, length, capacity;
} ihash_set_s;

ihash_set_s create_ihash_set(const size_t size, const hash_fn hash);
void destroy_ihash_set(ihash_set_s * set, const destroy_fn destroy);
void clear_ihash_set(ihash_set_s * set, const destroy_fn destroy);
ihash_set_s copy_ihash_set(const ihash_set_s set, const copy_fn copy);
bool is_empty_ihash_set(const ihash_set_s set);
void insert_ihash_set(ihash_set_s * set, const void * element);
void remove_ihash_set(ihash_set_s * set, const void * element, void * buffer);
bool contains_ihash_set(const ihash_set_s set, const void * element);
ihash_set_s union_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy);
ihash_set_s intersect_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy);
ihash_set_s subtract_ihash_set(const ihash_set_s minuend, const ihash_set_s subtrahend, const copy_fn copy);
ihash_set_s exclude_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy);
bool is_subset_ihash_set(const ihash_set_s super, const ihash_set_s sub);
bool is_proper_subset_ihash_set(const ihash_set_s super, const ihash_set_s sub);
bool is_disjoint_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two);
void foreach_ihash_set(const ihash_set_s set, const operate_fn operate, void * arguments);

#endif // IHASH_SET_H
