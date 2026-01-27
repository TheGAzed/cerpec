#ifndef IRB_SET_H
#define IRB_SET_H

#include <cerpec.h>

#if !defined(IRB_SET_CHUNK)
#   define IRB_SET_CHUNK CERPEC_CHUNK
#elif IRB_SET_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#endif

#define IRB_SET_NODE_COUNT 2
#define IRB_SET_LEFT  0
#define IRB_SET_RIGHT 1
#define IRED_SET_COLOR true
#define IBLACK_SET_COLOR false

/// @brief Infinite red-black set structure.
typedef struct infinite_red_black_set {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IRB_SET_NODE_COUNT];
    bool * color;
    size_t size, length, capacity, root;
    memory_s const * allocator;
} irb_set_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param compare Function pointer to compare elements.
/// @return Set structure.
irb_set_s create_irb_set(size_t const size, compare_fn const compare);

/// @brief Creates an empty structure.
/// @param size Size of a single element.
/// @param compare Function pointer to compare elements.
/// @param allocator Custom allocator structure.
/// @return Set structure.
irb_set_s make_irb_set(size_t const size, compare_fn const compare, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_irb_set(irb_set_s * const set, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param set Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_irb_set(irb_set_s * const set, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param set Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
irb_set_s copy_irb_set(irb_set_s const * const set, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param set Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_irb_set(irb_set_s const * const set);

/// @brief Inserts unique element into structure.
/// @param set Structure to insert element into.
/// @param element Element to insert.
void insert_irb_set(irb_set_s * const set, void const * const element);

/// @brief Removes unique element from structure.
/// @param set Structure to remove element into.
/// @param element Element to remove.
/// @param buffer Element buffer to save removed element.
void remove_irb_set(irb_set_s * const set, void const * const element, void * const buffer);

/// @brief Checks if structure contains element.
/// @param set Structure to check.
/// @param element Element to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_irb_set(irb_set_s const * const set, void const * const element);

/// @brief Performs union of two structures and returns a new copy.
/// @param set_one First structure to union.
/// @param set_two Second structure to union.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Union structure.
irb_set_s union_irb_set(irb_set_s const * const set_one, irb_set_s const * const set_two, copy_fn const copy);

/// @brief Performs intersection of two structures and returns a new copy.
/// @param set_one First structure to intersect.
/// @param set_two Second structure to intersect.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Intersect structure.
irb_set_s intersect_irb_set(irb_set_s const * const set_one, irb_set_s const * const set_two, copy_fn const copy);

/// @brief Performs subtraction of two structures and returns a new copy.
/// @param minuend Structure to subtract from.
/// @param subtrahend Structure to subtract with.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Subtract structure.
irb_set_s subtract_irb_set(irb_set_s const * const minuend, irb_set_s const * const subtrahend, copy_fn const copy);

/// @brief Performs exclusion (symmetric difference) of two structures and returns a new copy.
/// @param set_one First structure to exclude.
/// @param set_two Second structure to exclude.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Exclude structure.
irb_set_s exclude_irb_set(irb_set_s const * const set_one, irb_set_s const * const set_two, copy_fn const copy);

/// @brief Checks if structure is subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is subset, 'false' otherwise.
bool is_subset_irb_set(irb_set_s const * const superset, irb_set_s const * const subset);

/// @brief Checks if structure is proper subset of another structure.
/// @param superset Superset structure to check with.
/// @param subset Subset structure to check from.
/// @return 'true' if structure is proper subset, 'false' otherwise.
bool is_proper_subset_irb_set(irb_set_s const * const superset, irb_set_s const * const subset);

/// @brief Checks if structures are disjoint from each other.
/// @param set_one First structure to check.
/// @param set_two Second structure to check.
/// @return 'true' if structures are disjoint, 'false' otherwise.
bool is_disjoint_irb_set(irb_set_s const * const set_one, irb_set_s const * const set_two);

/// @brief Iterates over each element in structure.
/// @param set Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void each_irb_set(irb_set_s const * const set, handle_fn const handle, void * const arguments);

#endif // IRB_SET_H
