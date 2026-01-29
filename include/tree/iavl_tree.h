#ifndef IAVL_TREE_H
#define IAVL_TREE_H

#include <cerpec.h>

#if !defined(IAVL_TREE_CHUNK)
#   define IAVL_TREE_CHUNK CERPEC_CHUNK
#elif
#   error "Chunk size must be greater than zero."
#endif

#define IAVLT_NODE_COUNT 2
#define IAVLT_LEFT  0
#define IAVLT_RIGHT 1
/// Infinite Adelson Velsky and Landis tree structure.
typedef struct infinite_adelson_velsky_landis_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IAVLT_NODE_COUNT];
    size_t * height;
    size_t size, length, capacity, root;
    memory_s const * allocator;
} iavl_tree_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param compare Function pointer to compare elements.
/// @return Tree structure.
iavl_tree_s create_iavl_tree(size_t const size, compare_fn const compare);

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param allocator Custom allocator structure.
/// @param compare Function pointer to compare elements.
/// @return Tree structure.
iavl_tree_s make_iavl_tree(size_t const size, compare_fn const compare, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_iavl_tree(iavl_tree_s * const tree, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_iavl_tree(iavl_tree_s * const tree, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param tree Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
iavl_tree_s copy_iavl_tree(iavl_tree_s const * const tree, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param tree Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_iavl_tree(iavl_tree_s const * const tree);

/// @brief Inserts a single element into the structure.
/// @param tree Structure to insert into.
/// @param element Element buffer to insert.
void insert_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element);

/// @brief Removes specified element from the structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search and remove.
/// @param buffer Buffer to save removed element.
void remove_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Checks if element is contained in structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search.
/// @return 'true' if contained, 'false' if not.
bool contains_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict element);

/// @brief Gets the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Maximum element as specified by compare fucntion pointer.
void get_max_iavl_tree(iavl_tree_s const * const restrict tree, void * const restrict buffer);

/// @brief Gets the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Minimum element as specified by compare fucntion pointer.
void get_min_iavl_tree(iavl_tree_s const * const restrict tree, void * const restrict buffer);

/// @brief Gets and removes the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Maximum element as specified by compare fucntion pointer.
void remove_max_iavl_tree(iavl_tree_s * const restrict tree, void * const restrict buffer);

/// @brief Gets and removes the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Minimum element as specified by compare fucntion pointer.
void remove_min_iavl_tree(iavl_tree_s * const restrict tree, void * const restrict buffer);

/// @brief Gets the floor based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_floor_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the ceil based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_ceil_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the floor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_floor_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the ceil element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_ceil_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the in-order successor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_successor_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the in-order predecessor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_predecessor_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the in-order successor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_successor_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the in-order predecessor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_predecessor_iavl_tree(iavl_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and replaces the previous instance of element with new parameter one.
/// @param tree Structure to get from.
/// @param latter Latter buffer to search by.
/// @param former Former buffer to save replaced element.
/// @note Both 'former' and 'latter' must be equal based on comparison function pointer.
void update_iavl_tree(iavl_tree_s const * const restrict tree, void const * const restrict latter, void * const restrict former);

/// @brief Performs an in-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void in_order_iavl_tree(iavl_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a pre-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void pre_order_iavl_tree(iavl_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a post-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void post_order_iavl_tree(iavl_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a level-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void level_order_iavl_tree(iavl_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

#endif // IAVL_TREE_H
