#ifndef IDOUBLE_LIST_H
#define IDOUBLE_LIST_H

#include <cerpec.h>

#if !defined(IDOUBLE_LIST_CHUNK)
#   define IDOUBLE_LIST_CHUNK CERPEC_CHUNK
#endif

#define IDOUBLE_LIST_NODE 2
#define IDOUBLE_LIST_NEXT 0
#define IDOUBLE_LIST_PREV 1
typedef struct infinite_double_list {
    char * elements; // array to store elements
    size_t * node[IDOUBLE_LIST_NODE]; // array to store next indexes
    size_t size, length, capacity, head; // list size and head index parameter
} idouble_list_s;

idouble_list_s create_idouble_list(const size_t size);
void destroy_idouble_list(idouble_list_s * list, const destroy_fn destroy);
void clear_idouble_list(idouble_list_s * list, const destroy_fn destroy);
idouble_list_s copy_idouble_list(const idouble_list_s list, const copy_fn copy);
bool is_empty_idouble_list(const idouble_list_s list);
void insert_at_idouble_list(idouble_list_s * list, const void * element, const size_t index);
void get_idouble_list(const idouble_list_s list, const size_t index, void * buffer);
void remove_first_idouble_list(idouble_list_s * list, const void * element, void * buffer, const compare_fn compare);
void remove_last_idouble_list(idouble_list_s * list, const void * element, void * buffer, const compare_fn compare);
void remove_at_idouble_list(idouble_list_s * list, const size_t index, void * buffer);
void reverse_idouble_list(idouble_list_s * list);
void shift_next_idouble_list(idouble_list_s * list, const size_t shift);
void shift_prev_idouble_list(idouble_list_s * list, const size_t shift);
void splice_idouble_list(idouble_list_s * destination, idouble_list_s * source, const size_t index);
idouble_list_s split_idouble_list(idouble_list_s * list, const size_t index, const size_t length);
idouble_list_s extract_idouble_list(idouble_list_s * list, const filter_fn filter, void * arguments);
void foreach_next_idouble_list(const idouble_list_s list, const operate_fn operate, void * arguments);
void foreach_prev_idouble_list(const idouble_list_s list, const operate_fn operate, void * arguments);
void map_idouble_list(const idouble_list_s list, const manage_fn manage, void * arguments);

#endif // IDOUBLE_LIST_H
