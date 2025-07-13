#ifndef DEQUE_H
#define DEQUE_H

#include <cerpec.h>

#if !defined(DEQUE_CHUNK)
#   define DEQUE_CHUNK CERPEC_CHUNK
#endif

/// @brief Doubly linked list type node for deque structure.
struct deque_node {
    struct deque_node * next; // next sibling node
    struct deque_node * prev; // previous sibling node
    char * elements;          // elements array with DEQUE_CHUNK size
};

/// @brief Deque data structure.
typedef struct deque {
    struct deque_node * head;
    size_t current, size, length; // current index, element size and structure length
} deque_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @return Deque structure.
deque_s create_deque(const size_t size);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_deque(deque_s * deque, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_deque(deque_s * deque, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param deque Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Deque structure.
deque_s copy_deque(const deque_s deque, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param deque Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_deque(const deque_s deque);

/// @brief Enqueues a single element to the front of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_front_deque(deque_s * deque, const void * buffer);

/// @brief Enqueues a single element to the back of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_back_deque(deque_s * deque, const void * buffer);

/// @brief Dequeues a single element from the front of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_front_deque(deque_s * deque, void * buffer);

/// @brief Dequeues a single element from the back of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_back_deque(deque_s * deque, void * buffer);

/// @brief Peeks a single element from the front of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_front_deque(const deque_s deque, void * buffer);

/// @brief Peeks a single element from the back of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_back_deque(const deque_s deque, void * buffer);

/// @brief Iterates over each element in structure starting from the front.
/// @param deque Structure to iterate over.
/// @param operate Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void foreach_front_deque(const deque_s deque, const operate_fn operate, void * arguments);

/// @brief Iterates over each element in structure starting from the back.
/// @param deque Structure to iterate over.
/// @param operate Function pointer to operate on each element reference using element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void foreach_back_deque(const deque_s deque, const operate_fn operate, void * arguments);

/// @brief Maps each element in structure into an array to manage.
/// @param deque Structure to map.
/// @param manage Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_deque(const deque_s deque, const manage_fn manage, void * arguments);

#endif // DEQUE_H
