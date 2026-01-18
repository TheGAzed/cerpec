#ifndef IQUEUE_H
#define IQUEUE_H

#include <cerpec.h>

#if !defined(IQUEUE_CHUNK)
#   define IQUEUE_CHUNK CERPEC_CHUNK
#endif

/// @brief Circular linked list type node for queue structure.
struct infinite_queue_node {
    char * elements;                   // elements array with QUEUE_CHUNK size
    struct infinite_queue_node * next; // next sibling node
};

/// @brief Queue data structure.
typedef struct infinite_queue {
    struct infinite_queue_node * tail; // tail node to append next elements while enqueue-ing
    size_t size, current, length;      // current index, element size and structure length
    memory_s const * allocator;
} iqueue_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return Queue structure.
iqueue_s create_iqueue(size_t const size);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param allocator Custom allocator structure.
/// @return Stack structure.
iqueue_s make_iqueue(size_t const size, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_iqueue(iqueue_s * const queue, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_iqueue(iqueue_s * const queue, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param queue Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Queue structure.
iqueue_s copy_iqueue(iqueue_s const * const queue, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param queue Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_iqueue(iqueue_s const * const queue);

/// @brief Enqueues a single element to the end of the structure.
/// @param queue Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_iqueue(iqueue_s * const queue, void const * const buffer);

/// @brief Dequeues a single element from the start of the structure.
/// @param queue Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_iqueue(iqueue_s * const queue, void * const buffer);

/// @brief Peeks a single element from the start of the structure.
/// @param queue Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_iqueue(iqueue_s const * const queue, void * const buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param queue Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_iqueue(iqueue_s const * const queue, handle_fn const handle, void * const arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param queue Structure to map.
/// @param process Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_iqueue(iqueue_s const * const queue, process_fn const process, void * const arguments);

#endif // IQUEUE_H
