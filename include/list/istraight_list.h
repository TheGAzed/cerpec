#ifndef ISTRAIGHT_LIST_H
#define ISTRAIGHT_LIST_H

#include <cerpec.h>

#if !defined(ISTRAIGHT_LIST_CHUNK)
#   define ISTRAIGHT_LIST_CHUNK CERPEC_CHUNK
#endif

typedef struct infinite_straight_list {
    char * elements; // array to store elements
    size_t * next; // array to store next indexes
    size_t size, length, capacity, head; // list size and head index parameter
    size_t empty; // empty stack's size and head
} istraight_list_s;

istraight_list_s create_istraight_list(const size_t size);
void destroy_istraight_list(istraight_list_s * list, const destroy_fn destroy);
void clear_istraight_list(istraight_list_s * list, const destroy_fn destroy);
istraight_list_s copy_istraight_list(const istraight_list_s list, const copy_fn copy);
bool is_empty_istraight_list(const istraight_list_s list);
void insert_at_istraight_list(istraight_list_s * list, const void * element, const size_t index);
void get_istraight_list(const istraight_list_s list, const size_t index, void * buffer);
void remove_first_istraight_list(istraight_list_s * list, const void * element, void * buffer, const compare_fn compare);
void remove_at_istraight_list(istraight_list_s * list, const size_t index, void * buffer);
void reverse_istraight_list(istraight_list_s * list);
void splice_istraight_list(istraight_list_s * destination, istraight_list_s * source, const size_t index);
istraight_list_s split_istraight_list(istraight_list_s * list, const size_t index, const size_t length);
istraight_list_s extract_istraight_list(istraight_list_s * list, const filter_fn filter, void * arguments);
void foreach_istraight_list(const istraight_list_s list, const operate_fn operate, void * arguments);
void map_istraight_list(const istraight_list_s list, const manage_fn manage, void * arguments);

#endif // ISTRAIGHT_LIST_H
