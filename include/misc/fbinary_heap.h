#ifndef FBINARY_HEAP_H
#define FBINARY_HEAP_H

#include <cerpec.h>

/// @brief Finite binary heap structure.
typedef struct finite_binary_heap {
    compare_fn compare; // compare function to push, pop and heapify heap
    char * elements;    // elements array
    size_t size, length, max;
    memory_s const * allocator;
} fbinary_heap_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param compare Function pointer to compare elements.
/// @return Heap structure.
fbinary_heap_s create_fbinary_heap(size_t const size, size_t const max, compare_fn const compare);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param compare Function pointer to compare elements.
/// @param allocator Custom allocator structure.
/// @return Heap structure.
fbinary_heap_s make_fbinary_heap(size_t const size, size_t const max, compare_fn const compare, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param heap Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_fbinary_heap(fbinary_heap_s * const heap, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param heap Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_fbinary_heap(fbinary_heap_s * const heap, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param heap Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
fbinary_heap_s copy_fbinary_heap(fbinary_heap_s const * const heap, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param heap Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fbinary_heap(fbinary_heap_s const * const heap);

/// @brief Checks if structure is full.
/// @param heap Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fbinary_heap(fbinary_heap_s const * const heap);

/// @brief Pushes a single element to the top of the structure.
/// @param heap Structure to push into.
/// @param element Element buffer to push.
void push_fbinary_heap(fbinary_heap_s * const heap, void const * const element);

/// @brief Pops a single element from the structure.
/// @param heap Structure to pop from.
/// @param buffer Element buffer to save pop.
void pop_fbinary_heap(fbinary_heap_s * const heap, void * const buffer);

/// @brief Peeps a single element from the structure.
/// @param heap Structure to peep.
/// @param buffer Element buffer to save peep.
void peep_fbinary_heap(fbinary_heap_s const * const heap, void * const buffer);

/// @brief Replace element at array index with parameter.
/// @param heap Structure to replace element in.
/// @param index Array index to element in structure.
/// @param element Element buffer to replace with.
/// @param buffer Element buffer to save replaced.
void replace_fbinary_heap(fbinary_heap_s const * const heap, size_t const index, void const * const element, void * const buffer);

/// @brief Melds (combines) second structure into first while keeping properties.
/// @param destination First structure to meld elements into.
/// @param source Second structure to meld elements from.
void meld_fbinary_heap(fbinary_heap_s * const restrict destination, fbinary_heap_s * const restrict source);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param heap Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void each_fbinary_heap(fbinary_heap_s const * const heap, handle_fn const handle, void * const arguments);

#endif // FBINARY_HEAP_H
