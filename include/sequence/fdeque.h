#ifndef FDEQUE_H
#define FDEQUE_H

#include <cerpec.h>

/// @brief Deque data structure.
typedef struct finite_deque {
    char * elements;
    size_t length, size, current, max;
    memory_s const * allocator;
} fdeque_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param max Maximum length of structure.
/// @return Deque structure.
fdeque_s create_fdeque(size_t const size, size_t const max);

/// @brief Creates a custom empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param allocator Custom allocator structure.
/// @return Deque structure.
fdeque_s make_fdeque(size_t const size, size_t const max, memory_s const * allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_fdeque(fdeque_s * const deque, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param deque Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_fdeque(fdeque_s * const deque, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param deque Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Deque structure.
fdeque_s copy_fdeque(fdeque_s const * const deque, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param deque Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fdeque(fdeque_s const * const deque);

/// @brief Checks if structure is full.
/// @param deque Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fdeque(fdeque_s const * const deque);

/// @brief Enqueues a single element to the front of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_front_fdeque(fdeque_s * const deque, void const * const buffer);

/// @brief Enqueues a single element to the back of the structure.
/// @param deque Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_back_fdeque(fdeque_s * const deque, void const * const buffer);

/// @brief Dequeues a single element from the front of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_front_fdeque(fdeque_s * const deque, void * const buffer);

/// @brief Dequeues a single element from the back of the structure.
/// @param deque Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_back_fdeque(fdeque_s * const deque, void * const buffer);

/// @brief Peeks a single element from the front of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_front_fdeque(fdeque_s const * const deque, void * const buffer);

/// @brief Peeks a single element from the back of the structure.
/// @param deque Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_back_fdeque(fdeque_s const * const deque, void * const buffer);

/// @brief Iterates over each element in structure starting from the front.
/// @param deque Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_front_fdeque(fdeque_s const * const deque, handle_fn const handle, void * const arguments);

/// @brief Iterates over each element in structure starting from the back.
/// @param deque Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_back_fdeque(fdeque_s const * const deque, handle_fn const handle, void * const arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param deque Structure to map.
/// @param process Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_fdeque(fdeque_s const * const deque, process_fn const process, void * const arguments);

#endif // FDEQUE_H
