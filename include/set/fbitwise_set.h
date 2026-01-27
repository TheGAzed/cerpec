#ifndef FBITWISE_SET_H
#define FBITWISE_SET_H

#include <cerpec.h>

/// @brief Finite hash set structure.
typedef struct finite_bitwise_set {
    unsigned * bits;
    size_t length, max; // number of indexes and bit-based capacity
    memory_s const * allocator;
} fbitwise_set_s;

/// @brief Creates an empty structure.
/// @param max Maximum length of structure.
/// @return Set structure.
fbitwise_set_s create_fbitwise_set(size_t const max);

/// @brief Creates an empty structure.
/// @param allocator Custom allocator structure.
/// @param max Maximum length of structure.
/// @return Set structure.
fbitwise_set_s make_fbitwise_set(size_t const max, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param set Structure to destroy.
void destroy_fbitwise_set(fbitwise_set_s * const set);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param set Structure to destroy.
void clear_fbitwise_set(fbitwise_set_s * const set);

/// @brief Creates a copy of a structure and all its elements.
/// @param set Structure to copy.
/// @return Stack structure.
fbitwise_set_s copy_fbitwise_set(fbitwise_set_s const * const set);

/// @brief Checks if structure is empty.
/// @param set Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fbitwise_set(fbitwise_set_s const * const set);

/// @brief Checks if structure is full.
/// @param set Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fbitwise_set(fbitwise_set_s const * const set);

/// @brief Inserts unique element into structure.
/// @param set Structure to insert element into.
/// @param index Element to insert.
void insert_fbitwise_set(fbitwise_set_s * const set, size_t const index);

/// @brief Removes unique element from structure.
/// @param set Structure to remove element into.
/// @param index Element to remove.
void remove_fbitwise_set(fbitwise_set_s * const set, size_t const index);

/// @brief Checks if structure contains element.
/// @param set Structure to check.
/// @param index Element to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_fbitwise_set(fbitwise_set_s const * const set, size_t const index);

/// @brief Performs union of two structures and returns a new copy.
/// @param set_one First structure to union.
/// @param set_two Second structure to union.
/// @return Union structure.
fbitwise_set_s union_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two);

/// @brief Performs intersection of two structures and returns a new copy.
/// @param set_one First structure to intersect.
/// @param set_two Second structure to intersect.
/// @return Intersect structure.
fbitwise_set_s intersect_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two);

/// @brief Performs subtraction of two structures and returns a new copy.
/// @param minuend Structure to subtract from.
/// @param subtrahend Structure to subtract with.
/// @return Subtract structure.
fbitwise_set_s subtract_fbitwise_set(fbitwise_set_s const * const minuend, fbitwise_set_s const * const subtrahend);

/// @brief Performs exclusion (symmetric difference) of two structures and returns a new copy.
/// @param set_one First structure to exclude.
/// @param set_two Second structure to exclude.
/// @return Exclude structure.
fbitwise_set_s exclude_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two);

/// @brief Checks if structure is subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is subset, 'false' otherwise.
bool is_subset_fbitwise_set(fbitwise_set_s const * const superset, fbitwise_set_s const * const subset);

/// @brief Checks if structure is proper subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is proper subset, 'false' otherwise.
bool is_proper_subset_fbitwise_set(fbitwise_set_s const * const superset, fbitwise_set_s const * const subset);

/// @brief Checks if structures are disjoint from each other.
/// @param set_one First structure to check.
/// @param set_two Second structure to check.
/// @return 'true' if structures are disjoint, 'false' otherwise.
bool is_disjoint_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two);

/// @brief Iterates over each element in structure.
/// @param set Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
/// @note Function does not change the set indexes.
void each_index_fbitwise_set(fbitwise_set_s const * const set, handle_fn const handle, void * const arguments);

#endif // FBITWISE_SET_H
