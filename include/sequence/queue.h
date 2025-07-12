#ifndef QUEUE_H
#define QUEUE_H

#include <cerpec.h>

#if !defined(QUEUE_CHUNK)
#   define QUEUE_CHUNK CERPEC_CHUNK
#endif

/// @brief Linked list type node for queue structure.
struct queue_node {
    char * elements;          // elements array with QUEUE_CHUNK size
    struct queue_node * next; // next sibling node
};

/// @brief Queue data structure.
typedef struct queue {
    struct queue_node * tail;
    size_t size, current, length; // current index, element size and structure length
} queue_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return Queue structure.
queue_s create_queue(const size_t size);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_queue(queue_s * queue, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_queue(queue_s * queue, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param queue Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Queue structure.
queue_s copy_queue(const queue_s queue, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param queue Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_queue(const queue_s queue);

/// @brief Enqueues a single element to the end of the structure.
/// @param queue Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue(queue_s * queue, const void * buffer);

/// @brief Dequeues a single element from the start of the structure.
/// @param queue Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue(queue_s * queue, void * buffer);

/// @brief Peeks a single element from the start of the structure.
/// @param queue Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_queue(const queue_s queue, void * buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param queue Structure to iterate over.
/// @param operate Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void foreach_queue(const queue_s queue, const operate_fn operate, void * arguments);

/// @brief Maps each element in structure into an array to manage.
/// @param queue Structure to map.
/// @param manage Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_queue(const queue_s queue, const manage_fn manage, void * arguments);

#endif // QUEUE_H
