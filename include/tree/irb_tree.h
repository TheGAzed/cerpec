#ifndef IRB_TREE_H
#define IRB_TREE_H

#include <cerpec.h>

#if !defined(IRB_TREE_CHUNK)
#   define IRB_TREE_CHUNK CERPEC_CHUNK
#elif IRB_TREE_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (IRB_TREE_CHUNK & (IRB_TREE_CHUNK - 1))
#   error "Chunk size must be a power of 2."
#endif

#define IRBT_NODE_COUNT 2
#define IRBT_LEFT  0
#define IRBT_RIGHT 1
#define IRED_COLOR true
#define IBLACK_COLOR false

/// Infinite red black tree structure.
typedef struct infinite_red_black_tree {
    compare_fn compare;
    void * ac;
    char * elements;
    size_t * parent;
    size_t * node[IRBT_NODE_COUNT];
    bool * color;
    size_t size, length, capacity, root;
    memory_s const * allocator;
} irb_tree_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param compare Function pointer to compare elements.
/// @param ac Arguments for compare function pointer.
/// @return Tree structure.
irb_tree_s create_irb_tree(size_t const size, compare_fn const compare, void * const ac);

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param compare Function pointer to compare elements.
/// @param ac Arguments for compare function pointer.
/// @param allocator Custom allocator structure.
/// @return Tree structure.
irb_tree_s make_irb_tree(size_t const size, compare_fn const compare, void * const ac, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void destroy_irb_tree(irb_tree_s * const tree, set_fn const destroy, void * const ad);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
/// @param ad Arguments for destroy function pointer.
void clear_irb_tree(irb_tree_s * const tree, set_fn const destroy, void * const ad);

/// @brief Creates a copy of a structure and all its elements.
/// @param tree Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @param ac Arguments for copy function pointer.
/// @return Tree structure.
irb_tree_s copy_irb_tree(irb_tree_s const * const tree, copy_fn const copy, void * const ac);

/// @brief Checks if structure is empty.
/// @param tree Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_irb_tree(irb_tree_s const * const tree);

/// @brief Inserts a single element into the structure.
/// @param tree Structure to insert into.
/// @param element Element buffer to insert.
void insert_irb_tree(irb_tree_s * const tree, void const * const element);

/// @brief Removes specified element from the structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search and remove.
/// @param buffer Buffer to save removed element.
void remove_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer);

/// @brief Checks if element is contained.
/// @param tree Structure to remove from.
/// @param element Element buffer to search.
/// @return 'true' if contained, 'false' if not.
bool contains_irb_tree(irb_tree_s const * const tree, void const * const element);

/// @brief Gets the maximum element.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Maximum element as specified by compare fucntion pointer.
void get_max_irb_tree(irb_tree_s const * const tree, void * const buffer);

/// @brief Gets the minimum element.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Minimum element as specified by compare fucntion pointer.
void get_min_irb_tree(irb_tree_s const * const tree, void * const buffer);

/// @brief Gets and removes the maximum element.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Maximum element as specified by compare fucntion pointer.
void remove_max_irb_tree(irb_tree_s * const tree, void * const buffer);

/// @brief Gets and removes the minimum element.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Minimum element as specified by compare fucntion pointer.
void remove_min_irb_tree(irb_tree_s * const tree, void * const buffer);

/// @brief Gets the floor based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_floor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer);

/// @brief Gets the ceil based on parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save element.
void get_ceil_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer);

/// @brief Gets and removes the floor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_floor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer);

/// @brief Gets and removes the ceil element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_ceil_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer);

/// @brief Gets the in-order successor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_successor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer);

/// @brief Gets the in-order predecessor of parameter element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void get_predecessor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer);

/// @brief Gets and removes the in-order successor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_successor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer);

/// @brief Gets and removes the in-order predecessor element.
/// @param tree Structure to get from.
/// @param element Element buffer to search.
/// @param buffer Buffer to save removed element.
void remove_predecessor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer);

/// @brief Gets and replaces the previous instance of element with new parameter one.
/// @param tree Structure to get from.
/// @param latter Latter buffer to search by.
/// @param former Former buffer to save replaced element.
/// @note Both 'former' and 'latter' must be equal based on comparison function pointer.
void update_irb_tree(irb_tree_s const * const tree, void const * const latter, void * const former);

/// @brief Performs an in-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void in_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah);

/// @brief Performs a pre-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void pre_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah);

/// @brief Performs a post-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void post_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah);

/// @brief Performs a level-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void level_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah);

#endif // IRB_TREE_H
