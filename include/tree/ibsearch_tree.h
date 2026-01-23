#ifndef IBSEARCH_TREE_H
#define IBSEARCH_TREE_H

#include <cerpec.h>

#if !defined(IBSEARCH_TREE_CHUNK)
#   define IBSEARCH_TREE_CHUNK CERPEC_CHUNK
#elif
#   error "Chunk size must be greater than zero."
#endif

#define IBSEARCH_TREE_NODE_COUNT 2
#define IBSEARCH_TREE_LEFT  0
#define IBSEARCH_TREE_RIGHT 1

/// Infinite binary search tree structure.
typedef struct infinite_binary_search_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IBSEARCH_TREE_NODE_COUNT];
    size_t size, length, capacity, root;
    memory_s const * allocator;
} ibsearch_tree_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param compare Function pointer to compare elements.
/// @return Tree structure.
ibsearch_tree_s create_ibsearch_tree(size_t const size, compare_fn const compare);

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param allocator Custom allocator structure.
/// @param compare Function pointer to compare elements.
/// @return Tree structure.
ibsearch_tree_s make_ibsearch_tree(size_t const size, compare_fn const compare, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_ibsearch_tree(ibsearch_tree_s * const tree, set_fn const destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_ibsearch_tree(ibsearch_tree_s * const tree, set_fn const destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param tree Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
ibsearch_tree_s copy_ibsearch_tree(ibsearch_tree_s const * const tree, copy_fn const copy);

/// @brief Checks if structure is empty.
/// @param tree Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_ibsearch_tree(ibsearch_tree_s const * const tree);

/// @brief Inserts a single element into the structure.
/// @param tree Structure to insert into.
/// @param element Element buffer to insert.
void insert_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element);

/// @brief Removes specified element from the structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search and remove.
/// @param buffer Buffer to save removed element.
void remove_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Checks if element is contained in structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search.
/// @return 'true' if contained, 'false' if not.
bool contains_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict element);

/// @brief Gets the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Maximum element as specified by compare fucntion pointer.
void get_max_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void * const restrict buffer);

/// @brief Gets the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Minimum element as specified by compare fucntion pointer.
void get_min_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void * const restrict buffer);

/// @brief Gets and removes the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Maximum element as specified by compare fucntion pointer.
void remove_max_ibsearch_tree(ibsearch_tree_s * const restrict tree, void * const restrict buffer);

/// @brief Gets and removes the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Minimum element as specified by compare fucntion pointer.
void remove_min_ibsearch_tree(ibsearch_tree_s * const restrict tree, void * const restrict buffer);

/// @brief Gets the floor based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_floor_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the ceil based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_ceil_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the floor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_floor_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the ceil element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_ceil_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the in-order successor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_successor_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets the in-order predecessor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_predecessor_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the in-order successor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_successor_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and removes the in-order predecessor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_predecessor_ibsearch_tree(ibsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer);

/// @brief Gets and replaces the previous instance of element with new parameter one.
/// @param tree Structure to get from.
/// @param latter Latter buffer to search by.
/// @param former Former buffer to save replaced element.
/// @note Both 'former' and 'latter' must be equal based on comparison function pointer.
void update_ibsearch_tree(ibsearch_tree_s const * const restrict tree, void const * const restrict latter, void * const restrict former);

/// @brief Performs an in-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void inorder_ibsearch_tree(ibsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a pre-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void preorder_ibsearch_tree(ibsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a post-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void postorder_ibsearch_tree(ibsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

/// @brief Performs a level-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void levelorder_ibsearch_tree(ibsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments);

#endif // IBSEARCH_TREE_H
