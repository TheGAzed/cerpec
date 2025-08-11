#ifndef IRB_TREE_H
#define IRB_TREE_H

#include <cerpec.h>

#if !defined(IRB_TREE_CHUNK)
#   define IRB_TREE_CHUNK CERPEC_CHUNK
#endif

#define IRB_TREE_NODE_COUNT 2
#define IRB_TREE_LEFT  0
#define IRB_TREE_RIGHT 1
#define IRED_TREE_COLOR true
#define IBLACK_TREE_COLOR false

/// Infinite red black tree structure.
typedef struct infinite_red_black_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IRB_TREE_NODE_COUNT];
    bool * color;
    size_t size, length, capacity, root;
} irb_tree_s;

/// @brief Creates an empty structure.
/// @param size Size of a single element
/// @param compare Function pointer to compare elements.
/// @return Tree structure.
irb_tree_s create_irb_tree(const size_t size, const compare_fn compare);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void destroy_irb_tree(irb_tree_s * tree, const destroy_fn destroy);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param tree Structure to destroy.
/// @param destroy Function pointer to destroy a single element.
void clear_irb_tree(irb_tree_s * tree, const destroy_fn destroy);

/// @brief Creates a copy of a structure and all its elements.
/// @param tree Structure to copy.
/// @param copy Function pointer to create a deep/shallow copy of a single element.
/// @return Stack structure.
irb_tree_s copy_irb_tree(const irb_tree_s tree, const copy_fn copy);

/// @brief Checks if structure is empty.
/// @param tree Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_irb_tree(const irb_tree_s tree);

/// @brief Inserts a single element into the structure.
/// @param tree Structure to insert into.
/// @param element Element buffer to insert.
void insert_irb_tree(irb_tree_s * tree, const void * element);

/// @brief Removes specified element from the structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search and remove.
/// @param buffer Buffer to save removed element.
void remove_irb_tree(irb_tree_s * tree, const void * element, void * buffer);

/// @brief Checks if element is contained in structure.
/// @param tree Structure to remove from.
/// @param element Element buffer to search.
/// @return 'true' if contained, 'false' if not.
bool contains_irb_tree(const irb_tree_s tree, const void * element);

/// @brief Gets the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Maximum element as specified by compare fucntion pointer.
void get_max_irb_tree(const irb_tree_s tree, void * buffer);

/// @brief Gets the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save element.
/// @note Minimum element as specified by compare fucntion pointer.
void get_min_irb_tree(const irb_tree_s tree, void * buffer);

/// @brief Gets and removes the maximum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Maximum element as specified by compare fucntion pointer.
void remove_max_irb_tree(irb_tree_s * tree, void * buffer);

/// @brief Gets and removes the minimum element in structure.
/// @param tree Structure to get from.
/// @param buffer Buffer to save removed element.
/// @note Minimum element as specified by compare fucntion pointer.
void remove_min_irb_tree(irb_tree_s * tree, void * buffer);

/// @brief Performs an in-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param operate Function pointer to operate on each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void inorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);

/// @brief Performs a pre-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param operate Function pointer to operate on each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void preorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);

/// @brief Performs a post-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param operate Function pointer to operate on each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void postorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);

/// @brief Performs a level-order traversal of structure using operate function and generic arguments.
/// @param tree Structure to traverse.
/// @param operate Function pointer to operate on each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void level_order_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);

#endif // IRB_TREE_H
