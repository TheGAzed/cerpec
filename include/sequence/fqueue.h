#ifndef FQUEUE_H
#define FQUEUE_H

#include <cerpec.h>

/// @brief Queue data structure.
typedef struct infinite_queue {
    char * elements;
    size_t size, current, length, max; // current index, element size, maximum length and structure length
} fqueue_s;

/// @brief Creates an empty structure.
/// @param max Maximum length of structure.
/// @param size Size of a single element
/// @return Queue structure.
fqueue_s create_fqueue(const size_t max, const size_t size);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_fqueue(fqueue_s * queue, const set_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param queue Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_fqueue(fqueue_s * queue, const set_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param queue Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Queue structure.
fqueue_s copy_fqueue(const fqueue_s * queue, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param queue Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fqueue(const fqueue_s * queue);

/// @brief Checks if structure is full.
/// @param queue Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fqueue(const fqueue_s * queue);

/// @brief Enqueues a single element to the end of the structure.
/// @param queue Structure to enqueue into.
/// @param buffer Element buffer to enqueue.
void enqueue_fqueue(fqueue_s * queue, const void * buffer);

/// @brief Dequeues a single element from the start of the structure.
/// @param queue Structure to dequeue from.
/// @param buffer Element buffer to save dequeue.
void dequeue_fqueue(fqueue_s * queue, void * buffer);

/// @brief Peeks a single element from the start of the structure.
/// @param queue Structure to peek.
/// @param buffer Element buffer to save peek.
void peek_fqueue(const fqueue_s * queue, void * buffer);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param queue Structure to iterate over.
/// @param handle Function pointer to operate on each element reference using element size and generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_fqueue(const fqueue_s * queue, const handle_fn handle, void * arguments);

/// @brief Apply each element in structure into an array to manage.
/// @param queue Structure to map.
/// @param process Function pointer to manage array of elements using strucuter length, element size and arguments.
/// @param arguments Generic arguments to use in function pointer.
void apply_fqueue(const fqueue_s * queue, const process_fn process, void * arguments);

#endif // FQUEUE_H
