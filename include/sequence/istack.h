#ifndef ISTACK_H
#define ISTACK_H

#include <cerpec.h>

#if !defined(ISTACK_CHUNK)
#   define ISTACK_CHUNK CERPEC_CHUNK
#endif

/// @brief Stack data structure.
typedef struct infinite_stack {
    char * elements;               // array of elements
    size_t size, length, capacity; // size of single element, structure length and its capacity
    memory_s const * allocator;
} istack_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @return Stack structure.
istack_s create_istack(size_t const size);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param allocator Custom allocator structure.
/// @return Stack structure.
istack_s make_istack(size_t const size, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_istack(istack_s * const stack, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_istack(istack_s * const stack, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param stack Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
istack_s copy_istack(istack_s const * const stack, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param stack Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_istack(istack_s const * const stack);

/// @brief Pushes a single element to the top of the structure.
/// @param stack Structure to push into.
/// @param element Element buffer to push.
void push_istack(istack_s * const stack, void const * const element);

/// @brief Pops a single element from the top of the structure.
/// @param stack Structure to pop from.
/// @param buffer Element buffer to save pop.
void pop_istack(istack_s * const stack, void * const buffer);

/// @brief Peeps a single element from the top of the structure.
/// @param stack Structure to peep.
/// @param buffer Element buffer to save peep.
void peep_istack(istack_s const * const stack, void * const buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param stack Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_istack(istack_s const * const stack, handle_fn const handle, void * const arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param stack Structure to map.
/// @param process Function pointer to process array of elements using structure length and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_istack(istack_s const * const stack, process_fn const process, void * const arguments);

#endif // ISTACK_H
