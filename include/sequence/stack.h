#ifndef STACK_H
#define STACK_H

#include <cerpec.h>

#if !defined(STACK_CHUNK)
#   define STACK_CHUNK CERPEC_CHUNK
#endif

/// @brief Stack data structure.
typedef struct stack {
    char * elements;
    size_t size, length, capacity;
} stack_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return Stack structure.
stack_s create_stack(const size_t size);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_stack(stack_s * stack, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param stack Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_stack(stack_s * stack, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param stack Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
stack_s copy_stack(const stack_s stack, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param stack Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_stack(const stack_s stack);

/// @brief Pushes a single element to the top of the structure.
/// @param stack Structure to enqueue into.
/// @param element Element buffer to enqueue.
void push_stack(stack_s * stack, const void * element);

/// @brief Pops a single element from the top of the structure.
/// @param stack Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void pop_stack(stack_s * stack, void * buffer);

/// @brief Peeps a single element from the top of the structure.
/// @param stack Structure to peep.
/// @param buffer Element buffer to save peek.
void peep_stack(const stack_s stack, void * buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param stack Structure to iterate over.
/// @param operate Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments);

/// @brief Maps each element in structure into an array to manage.
/// @param stack Structure to map.
/// @param manage Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_stack(const stack_s stack, const manage_fn manage, void * arguments);

#endif // STACK_H
