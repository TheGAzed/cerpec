#ifndef ISTRAIGHT_LIST_H
#define ISTRAIGHT_LIST_H

#include <cerpec.h>

#if !defined(ISTRAIGHT_LIST_CHUNK)
#   define ISTRAIGHT_LIST_CHUNK CERPEC_CHUNK
#elif ISTRAIGHT_LIST_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#endif

/// @brief Infinite straight list data structure.
typedef struct infinite_straight_list {
    char * elements; // array to store elements
    size_t * next; // array to store next indexes
    size_t size, length, capacity, head; // list size and head index parameter
    size_t empty; // empty stack's size and head
    memory_s const * allocator;
} istraight_list_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @return List structure.
istraight_list_s create_istraight_list(size_t const size);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param allocator Custom allocator structure.
/// @return List structure.
istraight_list_s make_istraight_list(size_t const size, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, and makes it unusable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_istraight_list(istraight_list_s * const list, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_istraight_list(istraight_list_s * const list, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param list Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
istraight_list_s copy_istraight_list(istraight_list_s const * const list, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param list Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_istraight_list(istraight_list_s const * const list);

/// @brief Inserts a single element at the index of the structure.
/// @param list Structure to insert into.
/// @param element Element buffer to insert.
/// @param index Index to insert at.
void insert_at_istraight_list(istraight_list_s * const list, void const * const element, size_t const index);

/// @brief Gets the element at index without removing it from the structure.
/// @param list Structure to get element from.
/// @param index Index to get element at.
/// @param buffer Element buffer to save gotten element.
void get_istraight_list(istraight_list_s const * const list, size_t const index, void * const buffer);

/// @brief Removes first element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
void remove_first_istraight_list(istraight_list_s * const list, void const * const element, void * const buffer, compare_fn const compare);

/// @brief Removes element at index in structure.
/// @param list Structure to remove element from.
/// @param index Index to remove element at.
/// @param buffer Element buffer to save removed element.
void remove_at_istraight_list(istraight_list_s * const list, size_t const index, void * const buffer);

/// @brief Reverses element list order.
/// @param list Structure to reverse.
void reverse_istraight_list(istraight_list_s * const list);

/// @brief Splices (combines) two structures into one starting at destination's index.
/// @param destination Structure to splice into.
/// @param source Structure to splice with.
/// @param index Start index of destination structure.
void splice_istraight_list(istraight_list_s * const destination, istraight_list_s * const source, size_t const index);

/// @brief Splits structure into two based on starting index and new length.
/// @param list Structure to split.
/// @param index Start index of split.
/// @param length Length to split.
/// @return New split structure.
istraight_list_s split_istraight_list(istraight_list_s * const list, size_t const index, size_t const length);

/// @brief Extracts elements into new structure based on filter function.
/// @param list Structure to extracts from.
/// @param filter Function pointer to check if element meets extract condition.
/// @param arguments Generic void pointer arguments for function pointer.
/// @return New extracted structure.
istraight_list_s extract_istraight_list(istraight_list_s * const list, filter_fn const filter, void * const arguments);

/// @brief Iterates over each element in structure based on their order.
/// @param list Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic void pointer arguments for function pointer.
void map_istraight_list(istraight_list_s const * const list, handle_fn const handle, void * const arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param list Structure to manage.
/// @param process Function pointer process elements array of structure length.
/// @param arguments Generic void pointer arguments for function pointer.
void apply_istraight_list(istraight_list_s const * const list, process_fn const process, void * const arguments);

#endif // ISTRAIGHT_LIST_H
