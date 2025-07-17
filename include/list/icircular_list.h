#ifndef ICIRCULAR_LIST_H
#define ICIRCULAR_LIST_H

#include <cerpec.h>

#if !defined(ICIRCULAR_LIST_CHUNK)
#   define ICIRCULAR_LIST_CHUNK CERPEC_CHUNK
#endif

typedef struct infinite_circular_list {
    char * elements; // array to store elements
    size_t * next; // array to store next indexes
    size_t size, length, capacity, tail; // list size and head index parameter
    size_t empty; // empty stack's size and head
} icircular_list_s;

icircular_list_s create_icircular_list(const size_t size);
void destroy_icircular_list(icircular_list_s * list, const destroy_fn destroy);
void clear_icircular_list(icircular_list_s * list, const destroy_fn destroy);
icircular_list_s copy_icircular_list(const icircular_list_s list, const copy_fn copy);
bool is_empty_icircular_list(const icircular_list_s list);
void insert_at_icircular_list(icircular_list_s * restrict list, const void * restrict element, const size_t index);
void get_icircular_list(const icircular_list_s list, const size_t index, void * buffer);
void remove_first_icircular_list(icircular_list_s * restrict list, const void * restrict element, void * restrict buffer, const compare_fn compare);
void remove_at_icircular_list(icircular_list_s * restrict list, const size_t index, void * restrict buffer);
void reverse_icircular_list(icircular_list_s * list);
void shift_next_icircular_list(icircular_list_s * list, const size_t shift);
void splice_icircular_list(icircular_list_s * restrict destination, icircular_list_s * restrict source, const size_t index);
icircular_list_s split_icircular_list(icircular_list_s * list, const size_t index, const size_t length);
icircular_list_s extract_icircular_list(icircular_list_s * list, const filter_fn filter, void * arguments);
void foreach_icircular_list(const icircular_list_s list, const operate_fn operate, void * arguments);
void map_icircular_list(const icircular_list_s list, const manage_fn manage, void * arguments);

#endif // ICIRCULAR_LIST_H
