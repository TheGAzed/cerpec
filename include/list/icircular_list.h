#ifndef ICIRCULAR_LIST_H
#define ICIRCULAR_LIST_H

#include <cerpec.h>

#if !defined(ICIRCULAR_LIST_CHUNK)
#   define ICIRCULAR_LIST_CHUNK CERPEC_CHUNK
#elif ICIRCULAR_LIST_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (ICIRCULAR_LIST_CHUNK & (ICIRCULAR_LIST_CHUNK - 1))
#   error "Chunk size must be a power of 2."
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

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void destroy_icircular_list(icircular_list_s * const list, set_fn const destroy, void * const ad);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void clear_icircular_list(icircular_list_s * const list, set_fn const destroy, void * const ad);

/// @brief Creates a copy of a structure and all its elements.
/// @param list Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return List structure.
icircular_list_s copy_icircular_list(icircular_list_s const * const list, copy_fn const copy, void * const ac);

/// @brief Checks if structure is empty.
/// @param list Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_icircular_list(icircular_list_s const * const list);

/// @brief Inserts a single element at the index of the structure.
/// @param list Structure to insert into.
/// @param element Element buffer to insert.
/// @param index Index to insert at.
void insert_at_icircular_list(icircular_list_s * const list, void const * const element, size_t const index);

/// @brief Gets the element at index without removing it from the structure.
/// @param list Structure to get element from.
/// @param index Index to get element at.
/// @param buffer Element buffer to save gotten element.
void get_icircular_list(icircular_list_s const * const list, size_t const index, void * const buffer);

/// @brief Removes first element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
/// @param ac Arguments for compare function pointer.
void remove_first_icircular_list(icircular_list_s * const list, void const * const element, void * const buffer, compare_fn const compare, void * const ac);

/// @brief Removes element at index in structure.
/// @param list Structure to remove element from.
/// @param index Index to remove element at.
/// @param buffer Element buffer to save removed element.
void remove_at_icircular_list(icircular_list_s * const list, size_t const index, void * const buffer);

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
void splice_icircular_list(icircular_list_s * const destination, icircular_list_s * const source, size_t const index);

/// @brief Slices structure into two based on starting index and new length.
/// @param list Structure to slice.
/// @param index Start index of slice.
/// @param length Length to slice.
/// @return New sliced structure.
icircular_list_s slice_icircular_list(icircular_list_s * const list, size_t const index, size_t const length);

/// @brief Splits structure into two based on starting index.
/// @param list Structure to split.
/// @param index Start index of split.
/// @return New split structure.
icircular_list_s split_icircular_list(icircular_list_s * const list, size_t const index);

/// @brief Extracts elements into new structure based on filter function.
/// @param list Structure to extracts from.
/// @param filter Function pointer to check if element meets extract condition.
/// @param af Arguments for filter function pointer.
/// @return New extracted structure.
icircular_list_s extract_icircular_list(icircular_list_s * const list, filter_fn const filter, void * const af);

/// @brief Iterates over each element in structure based on their order.
/// @param list Structure to iterate over.
/// @param manage Function pointer to handle each element reference using generic arguments.
/// @param am Generic void pointer arguments for function pointer.
void each_icircular_list(icircular_list_s const * const list, manage_fn const manage, void * const am);

/// @brief Apply each element in structure into an array to manage.
/// @param list Structure to manage.
/// @param process Function pointer to process elements array of structure length.
/// @param ap Generic void pointer arguments for function pointer.
void apply_icircular_list(icircular_list_s const * const list, process_fn const process, void * const ap);

#endif // ICIRCULAR_LIST_H
