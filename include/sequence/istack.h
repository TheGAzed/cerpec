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
istack_s create_istack(const size_t size);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_istack(istack_s * stack, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_istack(istack_s * stack, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param stack Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
istack_s copy_istack(const istack_s * stack, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param stack Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_istack(const istack_s * stack);

/// @brief Pushes a single element to the top of the structure.
/// @param stack Structure to push into.
/// @param element Element buffer to push.
void push_istack(istack_s * stack, const void * element);

/// @brief Pops a single element from the top of the structure.
/// @param stack Structure to pop from.
/// @param buffer Element buffer to save pop.
void pop_istack(istack_s * stack, void * buffer);

/// @brief Peeps a single element from the top of the structure.
/// @param stack Structure to peep.
/// @param buffer Element buffer to save peep.
void peep_istack(const istack_s * stack, void * buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param stack Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_istack(const istack_s * stack, const handle_fn handle, void * arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param stack Structure to map.
/// @param process Function pointer to process array of elements using structure length and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_istack(const istack_s * stack, const process_fn process, void * arguments);

#endif // ISTACK_H
