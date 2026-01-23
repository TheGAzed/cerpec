#ifndef FSTACK_H
#define FSTACK_H

#include <cerpec.h>

/// @brief Finite stack data structure.
typedef struct finite_stack {
    char * elements;          // array of elements
    size_t size, length, max; // size of single element, structure length and its maximum
    const memory_s * allocator;
} fstack_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @return Stack structure.
fstack_s create_fstack(size_t const size, size_t const max);

/// @brief Creates a custom empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param allocator Custom allocator structure.
/// @return Stack structure.
fstack_s make_fstack(size_t const size, size_t const max, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_fstack(fstack_s * const stack, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_fstack(fstack_s * const stack, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param stack Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
fstack_s copy_fstack(fstack_s const * const stack, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param stack Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fstack(fstack_s const * const stack);

/// @brief Checks if structure is full.
/// @param stack Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fstack(fstack_s const * const stack);

/// @brief Pushes a single element to the top of the structure.
/// @param stack Structure to push into.
/// @param element Element buffer to push.
void push_fstack(fstack_s * const restrict stack, void const * const restrict element);

/// @brief Pops a single element from the top of the structure.
/// @param stack Structure to pop from.
/// @param buffer Element buffer to save pop.
void pop_fstack(fstack_s * const restrict stack, void * const restrict buffer);

/// @brief Peeps a single element from the top of the structure.
/// @param stack Structure to peep.
/// @param buffer Element buffer to save peep.
void peep_fstack(fstack_s const * const restrict stack, void * const restrict buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param stack Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_fstack(fstack_s const * const restrict stack, handle_fn const handle, void * const restrict arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param stack Structure to map.
/// @param process Function pointer to process array of elements using structure length and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_fstack(fstack_s const * const restrict stack, process_fn const process, void * const restrict arguments);

#endif // FSTACK_H
