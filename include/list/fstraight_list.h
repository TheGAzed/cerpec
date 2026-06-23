#ifndef FSTRAIGHT_LIST_H
#define FSTRAIGHT_LIST_H

#include <cerpec.h>

/// @brief Finite straight list data structure.
typedef struct finite_straight_list {
    char * elements; // array to store elements
    size_t * next; // array to store next indexes
    size_t size, length, max, head; // list size and head index parameter
    size_t empty; // empty stack's size and head
    memory_s const * allocator;
} fstraight_list_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @return List structure.
fstraight_list_s create_fstraight_list(size_t const size, size_t const max);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param allocator Custom allocator structure.
/// @return List structure.
fstraight_list_s make_fstraight_list(size_t const size, size_t const max, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void destroy_fstraight_list(fstraight_list_s * const list, set_fn const destroy, void * const ad);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param list Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void clear_fstraight_list(fstraight_list_s * const list, set_fn const destroy, void * const ad);

/// @brief Creates a copy of a structure and all its elements.
/// @param list Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return List structure.
fstraight_list_s copy_fstraight_list(fstraight_list_s const * const list, copy_fn const copy, void * const ac);

/// @brief Checks if structure is empty.
/// @param list Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fstraight_list(fstraight_list_s const * const list);

/// @brief Checks if structure is full.
/// @param list Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fstraight_list(fstraight_list_s const * const list);

/// @brief Inserts a single element at the index of the structure.
/// @param list Structure to insert into.
/// @param element Element buffer to insert.
/// @param index Index to insert at.
void insert_at_fstraight_list(fstraight_list_s * const list, void const * const element, size_t const index);

/// @brief Gets the element at index without removing it from the structure.
/// @param list Structure to get element from.
/// @param index Index to get element at.
/// @param buffer Element buffer to save gotten element.
void get_fstraight_list(fstraight_list_s const * const list, size_t const index, void * const buffer);

/// @brief Removes first element equal to parameter one, based on compare function pointer.
/// @param list Structure to remove element from.
/// @param element Element to search for.
/// @param buffer Element buffer to save removed element.
/// @param compare Function pointer to compare parameter element with list's.
/// @param ac Arguments for compare function pointer.
void remove_first_fstraight_list(fstraight_list_s * const list, void const * const element, void * const buffer, compare_fn const compare, void * const ac);

/// @brief Removes element at index in structure.
/// @param list Structure to remove element from.
/// @param index Index to remove element at.
/// @param buffer Element buffer to save removed element.
void remove_at_fstraight_list(fstraight_list_s * const list, size_t const index, void * const buffer);

/// @brief Reverses element list order.
/// @param list Structure to reverse.
void reverse_fstraight_list(fstraight_list_s * const list);

/// @brief Splices (combines) two structures into one starting at destination's index.
/// @param destination Structure to splice into.
/// @param source Structure to splice with.
/// @param index Start index of destination structure.
void splice_fstraight_list(fstraight_list_s * const destination, fstraight_list_s * const source, size_t const index);

/// @brief Slices structure into two based on starting index and new length.
/// @param list Structure to slice.
/// @param index Start index of slice.
/// @param length Length to slice.
/// @param list_max Maximum length of original structure.
/// @param slice_max Maximum length of sliced structure.
/// @return New sliced structure.
fstraight_list_s slice_fstraight_list(fstraight_list_s * const list, size_t const index, size_t const length, size_t const list_max, size_t const slice_max);

/// @brief Splits structure into two based on starting index.
/// @param list Structure to split.
/// @param index Start index of split.
/// @param list_max Maximum length of original structure.
/// @param split_max Maximum length of split structure.
/// @return New split structure.
fstraight_list_s split_fstraight_list(fstraight_list_s * const list, size_t const index, size_t const list_max, size_t const split_max);

/// @brief Extracts elements into new structure based on filter function.
/// @param list Structure to extracts from.
/// @param filter Function pointer to check if element meets extract condition.
/// @param af Arguments for filter function pointer.
/// @param list_max Maximum length of original structure.
/// @param extract_max Maximum length of extracted structure.
/// @return New extracted structure.
fstraight_list_s extract_fstraight_list(fstraight_list_s * const list, filter_fn const filter, void * const af, size_t const list_max, size_t const extract_max);

/// @brief Iterates over each element in structure based on their order.
/// @param list Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic void pointer arguments for function pointer.
void each_fstraight_list(fstraight_list_s const * const list, handle_fn const handle, void * const ah);

/// @brief Apply each element in structure into an array to manage.
/// @param list Structure to manage.
/// @param process Function pointer process elements array of structure length.
/// @param ap Generic void pointer arguments for function pointer.
void apply_fstraight_list(fstraight_list_s const * const list, process_fn const process, void * const ap);

#endif // FSTRAIGHT_LIST_H
