#ifndef IDEQUE_H
#define IDEQUE_H

#include <cerpec.h>

#if !defined(IDEQUE_CHUNK)
#   define IDEQUE_CHUNK CERPEC_CHUNK
#elif IDEQUE_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#endif

/// @brief Doubly linked list type node for deque structure.
struct infinite_deque_node {
    struct infinite_deque_node * next; // next sibling node
    struct infinite_deque_node * prev; // previous sibling node
    char * elements;                   // elements array with IDEQUE_CHUNK size
};

/// @brief Deque data structure.
typedef struct infinite_deque {
    struct infinite_deque_node * head;
    size_t current, size, length; // current index, element size and structure length
    memory_s const * allocator;
} ideque_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return Deque structure.
ideque_s create_ideque(size_t const size);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param allocator Custom allocator structure.
/// @return Stack structure.
ideque_s make_ideque(size_t const size, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_ideque(ideque_s * const deque, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_ideque(ideque_s * const deque, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param deque Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Deque structure.
ideque_s copy_ideque(ideque_s const * const deque, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param deque Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_ideque(ideque_s const * const deque);

/// @brief Enqueues a single element to the front of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_front_ideque(ideque_s * const deque, void const * const buffer);

/// @brief Enqueues a single element to the back of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_back_ideque(ideque_s * const deque, void const * const buffer);

/// @brief Dequeues a single element from the front of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_front_ideque(ideque_s * const deque, void * const buffer);

/// @brief Dequeues a single element from the back of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_back_ideque(ideque_s * const deque, void * const buffer);

/// @brief Peeks a single element from the front of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_front_ideque(ideque_s const * const deque, void * const buffer);

/// @brief Peeks a single element from the back of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_back_ideque(ideque_s const * const deque, void * const buffer);

/// @brief Iterates over each element in structure starting from the front.
/// @param deque Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_front_ideque(ideque_s const * const deque, handle_fn const handle, void * const arguments);

/// @brief Iterates over each element in structure starting from the back.
/// @param deque Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_back_ideque(ideque_s const * const deque, handle_fn const handle, void * const arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param deque Structure to map.
/// @param process Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_ideque(ideque_s const * const deque, process_fn const process, void * const arguments);

#endif // IDEQUE_H
