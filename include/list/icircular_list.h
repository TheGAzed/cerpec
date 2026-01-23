#ifndef ICIRCULAR_LIST_H
#define ICIRCULAR_LIST_H

#include <cerpec.h>

#if !defined(ICIRCULAR_LIST_CHUNK)
#   define ICIRCULAR_LIST_CHUNK CERPEC_CHUNK
#elif ICIRCULAR_LIST_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#endif

/// @brief Infinite circular list data structure.
typedef struct infinite_circular_list {
    char * elements; // array to store elements
    size_t * next; // array to store next indexes
    size_t size, length, capacity, tail; // list size and head index parameter
    size_t empty; // empty stack's size and head
    memory_s const * allocator;
} icircular_list_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @return List structure.
icircular_list_s create_icircular_list(size_t const size);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param allocator Custom allocator structure.
/// @return List structure.
icircular_list_s make_icircular_list(size_t const size, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, and makes it unusable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_icircular_list(icircular_list_s * const list, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_icircular_list(icircular_list_s * const list, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param list Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
icircular_list_s copy_icircular_list(icircular_list_s const * const list, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param list Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_icircular_list(icircular_list_s const * const list);

/// @brief Inserts a single element at the index of the structure.
/// @param list Structure to insert into.
/// @param element Element buffer to insert.
/// @param index Index to insert at.
void insert_at_icircular_list(icircular_list_s * const restrict list, void const * const restrict element, size_t const index);

/// @brief Gets the element at index without removing it from the structure.
/// @param list Structure to get element from.
/// @param index Index to get element at.
/// @param buffer Element buffer to save gotten element.
void get_icircular_list(icircular_list_s const * const restrict list, size_t const index, void * const restrict buffer);

/// @brief Removes first element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
void remove_first_icircular_list(icircular_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare);

/// @brief Removes element at index in structure.
/// @param list Structure to remove element from.
/// @param index Index to remove element at.
/// @param buffer Element buffer to save removed element.
void remove_at_icircular_list(icircular_list_s * const restrict list, size_t const index, void * const restrict buffer);

/// @brief Reverses element list order.
/// @param list Structure to reverse.
void reverse_icircular_list(icircular_list_s * const list);

/// @brief Shifts head list node by a number of times.
/// @param list Structure to shift.
/// @param shift Value to shift by.
void shift_next_icircular_list(icircular_list_s * const list, size_t const shift);

/// @brief Splices (combines) two structures into one starting at destination's index.
/// @param destination Structure to splice into.
/// @param source Structure to splice with.
/// @param index Start index of destination structure.
void splice_icircular_list(icircular_list_s * const restrict destination, icircular_list_s * const restrict source, size_t const index);

/// @brief Splits structure into two based on starting index and new length.
/// @param list Structure to split.
/// @param index Start index of split.
/// @param length Length to split.
/// @return New split structure.
icircular_list_s split_icircular_list(icircular_list_s * const list, size_t const index, size_t const length);

/// @brief Extracts elements into new structure based on filter function.
/// @param list Structure to extracts from.
/// @param filter Function pointer to check if element meets extract condition.
/// @param arguments Generic void pointer arguments for function pointer.
/// @return New extracted structure.
icircular_list_s extract_icircular_list(icircular_list_s * const restrict list, filter_fn const filter, void * const restrict arguments);

/// @brief Iterates over each element in structure based on their order.
/// @param list Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic void pointer arguments for function pointer.
void map_icircular_list(icircular_list_s const * const restrict list, handle_fn const handle, void * const restrict arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param list Structure to manage.
/// @param process Function pointer to process elements array of structure length.
/// @param arguments Generic void pointer arguments for function pointer.
void apply_icircular_list(icircular_list_s const * const restrict list, process_fn const process, void * const restrict arguments);

#endif // ICIRCULAR_LIST_H
