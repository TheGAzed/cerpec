#ifndef IBINARY_HEAP_H
#define IBINARY_HEAP_H

#include <cerpec.h>

#if !defined(IBINARY_HEAP_CHUNK)
#   define IBINARY_HEAP_CHUNK CERPEC_CHUNK
#endif

/// @brief Inifinite binary heap structure.
typedef struct infinite_binary_heap {
    compare_fn compare; // compare function to push, pop and heapify heap
    char * elements;    // elements array
    size_t size, length, capacity;
} ibinary_heap_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param compare Function pointer to compare elements.
/// @return Heap structure.
ibinary_heap_s create_ibinary_heap(const size_t size, const compare_fn compare);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param heap Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_ibinary_heap(ibinary_heap_s * heap, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param heap Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_ibinary_heap(ibinary_heap_s * heap, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param heap Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
ibinary_heap_s copy_ibinary_heap(const ibinary_heap_s heap, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param heap Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_ibinary_heap(const ibinary_heap_s heap);

/// @brief Pushes a single element to the top of the structure.
/// @param heap Structure to push into.
/// @param element Element buffer to push.
void push_ibinary_heap(ibinary_heap_s * heap, const void * element);

/// @brief Pops a single element from the structure.
/// @param heap Structure to pop from.
/// @param buffer Element buffer to save pop.
void pop_ibinary_heap(ibinary_heap_s * heap, void * buffer);

/// @brief Peeps a single element from the structure.
/// @param heap Structure to peep.
/// @param buffer Element buffer to save peep.
void peep_ibinary_heap(const ibinary_heap_s heap, void * buffer);

/// @brief Replace element at array index with parameter.
/// @param heap Structure to replace element in.
/// @param index Array index to element in structure.
/// @param element Element buffer to replace with.
/// @param buffer Element buffer to save replaced.
void replace_ibinary_heap(const ibinary_heap_s heap, const size_t index, const void * element, void * buffer);

/// @brief Melds (combines) second structure into first while keeping properties.
/// @param destination First structure to meld elements into.
/// @param source Second structure to meld elements from.
void meld_ibinary_heap(ibinary_heap_s * restrict destination, ibinary_heap_s * restrict source);

/// @brief Patches up structure if its properties were broken.
/// @param heap Structure to patch.
void patch_ibinary_heap(const ibinary_heap_s heap);

/// @brief Iterates over each element in structure starting from the beginning.
/// @param heap Structure to iterate over.
/// @param operate Function pointer to operate on each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void foreach_istack(const ibinary_heap_s heap, const operate_fn operate, void * arguments);

#endif // IBINARY_HEAP_H
