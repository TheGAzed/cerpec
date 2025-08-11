#ifndef IDOUBLE_LIST_H
#define IDOUBLE_LIST_H

#include <cerpec.h>

#if !defined(IDOUBLE_LIST_CHUNK)
#   define IDOUBLE_LIST_CHUNK CERPEC_CHUNK
#endif

#define IDOUBLE_LIST_NODE 2
#define IDOUBLE_LIST_NEXT 0
#define IDOUBLE_LIST_PREV 1
/// @brief Infinite doubly linked list data structure.
typedef struct infinite_double_list {
    char * elements; // array to store elements
    size_t * node[IDOUBLE_LIST_NODE]; // array to store next indexes
    size_t size, length, capacity, head; // list size and head index parameter
} idouble_list_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return List structure.
idouble_list_s create_idouble_list(const size_t size);

/// @brief Destroys a structure and its elements, and makes it unusable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_idouble_list(idouble_list_s * list, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_idouble_list(idouble_list_s * list, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param list Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
idouble_list_s copy_idouble_list(const idouble_list_s list, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param list Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_idouble_list(const idouble_list_s list);

/// @brief Inserts a single element at the index of the structure.
/// @param list Structure to insert into.
/// @param element Element buffer to insert.
/// @param index Index to insert at.
void insert_at_idouble_list(idouble_list_s * list, const void * element, const size_t index);

/// @brief Gets the element at index without removing it from the structure.
/// @param list Structure to get element from.
/// @param index Index to get element at.
/// @param buffer Element buffer to save gotten element.
void get_idouble_list(const idouble_list_s list, const size_t index, void * buffer);

/// @brief Removes first element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
void remove_first_idouble_list(idouble_list_s * list, const void * element, void * buffer, const compare_fn compare);

/// @brief Removes last element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
void remove_last_idouble_list(idouble_list_s * list, const void * element, void * buffer, const compare_fn compare);

/// @brief Removes element at index in structure.
/// @param list Structure to remove element from.
/// @param index Index to remove element at.
/// @param buffer Element buffer to save removed element.
void remove_at_idouble_list(idouble_list_s * list, const size_t index, void * buffer);

/// @brief Reverses element list order.
/// @param list Structure to reverse.
void reverse_idouble_list(idouble_list_s * list);

/// @brief Shifts head list node by next number of times.
/// @param list Structure to shift.
/// @param shift Value to shift by.
void shift_next_idouble_list(idouble_list_s * list, const size_t shift);

/// @brief Shifts head list node by previous number of times.
/// @param list Structure to shift.
/// @param shift Value to shift by.
void shift_prev_idouble_list(idouble_list_s * list, const size_t shift);

/// @brief Splices (combines) two structures into one starting at destination's index.
/// @param destination Structure to splice into.
/// @param source Structure to splice with.
/// @param index Start index of destination structure.
void splice_idouble_list(idouble_list_s * destination, idouble_list_s * source, const size_t index);

/// @brief Splits structure into two based on starting index and new length.
/// @param list Structure to split.
/// @param index Start index of split.
/// @param length Length to split.
/// @return New split structure.
idouble_list_s split_idouble_list(idouble_list_s * list, const size_t index, const size_t length);

/// @brief Extracts elements into new structure based on filter function.
/// @param list Structure to extracts from.
/// @param filter Function pointer to check if element meets extract condition.
/// @param arguments Generic void pointer arguments for function pointer.
/// @return New extracted structure.
idouble_list_s extract_idouble_list(idouble_list_s * list, const filter_fn filter, void * arguments);

/// @brief Iterates over each next element in structure based on their order.
/// @param list Structure to iterate over.
/// @param operate Function pointer to operate on each element.
/// @param arguments Generic void pointer arguments for function pointer.
void foreach_next_idouble_list(const idouble_list_s list, const operate_fn operate, void * arguments);

/// @brief Iterates over each previous element in structure based on their order.
/// @param list Structure to iterate over.
/// @param operate Function pointer to operate on each element.
/// @param arguments Generic void pointer arguments for function pointer.
void foreach_prev_idouble_list(const idouble_list_s list, const operate_fn operate, void * arguments);

/// @brief Maps each element in structure into an array to manage.
/// @param list Structure to manage.
/// @param manage Function pointer to operate on elements array of structure length.
/// @param arguments Generic void pointer arguments for function pointer.
void map_idouble_list(const idouble_list_s list, const manage_fn manage, void * arguments);

#endif // IDOUBLE_LIST_H
