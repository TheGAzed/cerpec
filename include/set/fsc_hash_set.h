#ifndef FSC_HASH_SET_H
#define FSC_HASH_SET_H

#include <cerpec.h>

/// @brief Finite hash set structure.
typedef struct finite_separate_chaining_hash_set {
    hash_fn hash;
    void * ah;
    compare_fn compare;
    void * ac;
    char * elements;
    size_t * next, * prev, * head, * hashes;
    size_t size, length, max;
    memory_s const * allocator;
} fsc_hash_set_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param hash Function pointer to hash element into value.
/// @param ah Arguments for hash function pointer.
/// @param compare Function pointer to compare elements.
/// @param ac Arguments for compare function pointer.
/// @return Set structure.
fsc_hash_set_s create_fsc_hash_set(size_t const size, size_t const max, hash_fn const hash, void * const ah, compare_fn const compare, void * const ac);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param max Maximum length of structure.
/// @param hash Function pointer to hash element into value.
/// @param ah Arguments for hash function pointer.
/// @param compare Function pointer to compare elements.
/// @param ac Arguments for compare function pointer.
/// @param allocator Custom allocator structure.
/// @return Set structure.
fsc_hash_set_s make_fsc_hash_set(size_t const size, size_t const max, hash_fn const hash, void * const ah, compare_fn const compare, void * const ac, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void destroy_fsc_hash_set(fsc_hash_set_s * const set, set_fn const destroy, void * const ad);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void clear_fsc_hash_set(fsc_hash_set_s * const set, set_fn const destroy, void * const ad);

/// @brief Creates a copy of a structure and all its elements.
/// @param set Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Set structure.
fsc_hash_set_s copy_fsc_hash_set(fsc_hash_set_s const * const set, copy_fn const copy, void * const ac);

/// @brief Checks if structure is empty.
/// @param set Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fsc_hash_set(fsc_hash_set_s const * const set);

/// @brief Checks if structure is full.
/// @param set Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fsc_hash_set(fsc_hash_set_s const * const set);

/// @brief Inserts unique element into structure.
/// @param set Structure to insert element into.
/// @param element Element to insert.
void insert_fsc_hash_set(fsc_hash_set_s * const set, void const * const element);

/// @brief Removes unique element from structure.
/// @param set Structure to remove element into.
/// @param element Element to remove.
/// @param buffer Element buffer to save removed element.
void remove_fsc_hash_set(fsc_hash_set_s * const set, void const * const element, void * const buffer);

/// @brief Checks if structure contains element.
/// @param set Structure to check.
/// @param element Element to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_fsc_hash_set(fsc_hash_set_s const * const set, void const * const element);

/// @brief Performs union of two structures and returns a new copy.
/// @param set_one First structure to union.
/// @param set_two Second structure to union.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Union structure.
fsc_hash_set_s union_fsc_hash_set(fsc_hash_set_s const * const set_one, fsc_hash_set_s const * const set_two, copy_fn const copy, void * const ac);

/// @brief Performs intersection of two structures and returns a new copy.
/// @param set_one First structure to intersect.
/// @param set_two Second structure to intersect.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Intersect structure.
fsc_hash_set_s intersect_fsc_hash_set(fsc_hash_set_s const * const set_one, fsc_hash_set_s const * const set_two, copy_fn const copy, void * const ac);

/// @brief Performs subtraction of two structures and returns a new copy.
/// @param minuend Structure to subtract from.
/// @param subtrahend Structure to subtract with.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Subtract structure.
fsc_hash_set_s subtract_fsc_hash_set(fsc_hash_set_s const * const minuend, fsc_hash_set_s const * const subtrahend, copy_fn const copy, void * const ac);

/// @brief Performs exclusion (symmetric difference) of two structures and returns a new copy.
/// @param set_one First structure to exclude.
/// @param set_two Second structure to exclude.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Exclude structure.
fsc_hash_set_s exclude_fsc_hash_set(fsc_hash_set_s const * const set_one, fsc_hash_set_s const * const set_two, copy_fn const copy, void * const ac);

/// @brief Checks if structure is subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is subset, 'false' otherwise.
bool is_subset_fsc_hash_set(fsc_hash_set_s const * const superset, fsc_hash_set_s const * const subset);

/// @brief Checks if structure is proper subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is proper subset, 'false' otherwise.
bool is_proper_subset_fsc_hash_set(fsc_hash_set_s const * const superset, fsc_hash_set_s const * const subset);

/// @brief Checks if structures are disjoint from each other.
/// @param set_one First structure to check.
/// @param set_two Second structure to check.
/// @return 'true' if structures are disjoint, 'false' otherwise.
bool is_disjoint_fsc_hash_set(fsc_hash_set_s const * const set_one, fsc_hash_set_s const * const set_two);

/// @brief Iterates over each element in structure.
/// @param set Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void each_fsc_hash_set(fsc_hash_set_s const * const set, handle_fn const handle, void * const ah);

#endif // FSC_HASH_SET_H
