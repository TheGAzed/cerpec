#ifndef ISC_HASH_SET_H
#define ISC_HASH_SET_H

#include <cerpec.h>

#if !defined(ISC_HASH_SET_CHUNK)
#   define ISC_HASH_SET_CHUNK CERPEC_CHUNK
#elif ISC_HASH_SET_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (ISC_HASH_SET_CHUNK & (ISC_HASH_SET_CHUNK - 1))
#   error "Chunk size must be a power of 2."
#endif

/// @brief Infinite hash set structure.
typedef struct infinite_separate_chaining_hash_set {
    hash_fn hash;
    compare_fn compare;
    char * elements;
    size_t * next, * prev, * head, * hashes;
    size_t size, length, capacity;
    memory_s const * allocator;
} isc_hash_set_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param compare Function pointer to compare elements.
/// @param hash Function pointer to hash element into value.
/// @return Set structure.
isc_hash_set_s create_isc_hash_set(size_t const size, hash_fn const hash, compare_fn const compare);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param hash Function pointer to hash element into value.
/// @param compare Function pointer to compare elements.
/// @param allocator Custom allocator structure.
/// @return Set structure.
isc_hash_set_s make_isc_hash_set(size_t const size, hash_fn const hash, compare_fn const compare, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_isc_hash_set(isc_hash_set_s * const set, set_fn const destroy);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_isc_hash_set(isc_hash_set_s * const set, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param set Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Set structure.
isc_hash_set_s copy_isc_hash_set(isc_hash_set_s const * const set, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param set Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_isc_hash_set(isc_hash_set_s const * const set);

/// @brief Inserts unique element into structure.
/// @param set Structure to insert element into.
/// @param element Element to insert.
void insert_isc_hash_set(isc_hash_set_s * const set, void const * const element);

/// @brief Removes unique element from structure.
/// @param set Structure to remove element into.
/// @param element Element to remove.
/// @param buffer Element buffer to save removed element.
void remove_isc_hash_set(isc_hash_set_s * const set, void const * const element, void * const buffer);

/// @brief Checks if structure contains element.
/// @param set Structure to check.
/// @param element Element to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_isc_hash_set(isc_hash_set_s const * const set, void const * const element);

/// @brief Performs union of two structures and returns a new copy.
/// @param set_one First structure to union.
/// @param set_two Second structure to union.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Union structure.
isc_hash_set_s union_isc_hash_set(isc_hash_set_s const * const set_one, isc_hash_set_s const * const set_two, copy_fn const copy);

/// @brief Performs intersection of two structures and returns a new copy.
/// @param set_one First structure to intersect.
/// @param set_two Second structure to intersect.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Intersect structure.
isc_hash_set_s intersect_isc_hash_set(isc_hash_set_s const * const set_one, isc_hash_set_s const * const set_two, copy_fn const copy);

/// @brief Performs subtraction of two structures and returns a new copy.
/// @param minuend Structure to subtract from.
/// @param subtrahend Structure to subtract with.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Subtract structure.
isc_hash_set_s subtract_isc_hash_set(isc_hash_set_s const * const minuend, isc_hash_set_s const * const subtrahend, copy_fn const copy);

/// @brief Performs exclusion (symmetric difference) of two structures and returns a new copy.
/// @param set_one First structure to exclude.
/// @param set_two Second structure to exclude.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Exclude structure.
isc_hash_set_s exclude_isc_hash_set(isc_hash_set_s const * const set_one, isc_hash_set_s const * const set_two, copy_fn const copy);

/// @brief Checks if structure is subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is subset, 'false' otherwise.
bool is_subset_isc_hash_set(isc_hash_set_s const * const superset, isc_hash_set_s const * const subset);

/// @brief Checks if structure is proper subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is proper subset, 'false' otherwise.
bool is_proper_subset_isc_hash_set(isc_hash_set_s const * const superset, isc_hash_set_s const * const subset);

/// @brief Checks if structures are disjoint from each other.
/// @param set_one First structure to check.
/// @param set_two Second structure to check.
/// @return 'true' if structures are disjoint, 'false' otherwise.
bool is_disjoint_isc_hash_set(isc_hash_set_s const * const set_one, isc_hash_set_s const * const set_two);

/// @brief Iterates over each element in structure.
/// @param set Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void each_isc_hash_set(isc_hash_set_s const * const set, handle_fn const handle, void * const arguments);

#endif // ISC_HASH_SET_H
