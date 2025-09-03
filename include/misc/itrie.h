#ifndef ITRIE_H
#define ITRIE_H

#include <cerpec.h>

#if !defined(ITRIE_CHUNK)
#   define ITRIE_CHUNK CERPEC_CHUNK
#endif

/// @brief Infinite hash set structure.
typedef struct infinite_trie {
    hash_fn hash;
    char * elements;
    bool * terminal;
    size_t * nodes;
    size_t * parent;
    size_t length, size, count, root, capacity;
} itrie_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param count Count of all valid elements.
/// @param hash Function pointer to hash element into value.
/// @return Set structure.
itrie_s create_itrie(const size_t size, const size_t count, const hash_fn hash);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_itrie(itrie_s * trie, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param trie Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_itrie(itrie_s * trie, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param trie Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
itrie_s copy_itrie(const itrie_s trie, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param trie Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_itrie(const itrie_s trie);

void insert_itrie(itrie_s * trie, const void * array, const size_t length, const copy_fn copy);

bool contains_itrie(const itrie_s trie, const void * array, const size_t length);

void remove_itrie(itrie_s * trie, const void * array, const size_t length, const destroy_fn destroy);

#endif // ITRIE_H
