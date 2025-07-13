#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cerpec.h>

#if !defined(AVL_TREE_CHUNK)
#   define AVL_TREE_CHUNK CERPEC_CHUNK
#endif

#define AVL_TREE_NODE_COUNT 2
#define AVL_TREE_LEFT  0
#define AVL_TREE_RIGHT 1
typedef struct avl_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[AVL_TREE_NODE_COUNT];
    size_t * height;
    size_t size, length, capacity, root;
} avl_tree_s;

avl_tree_s create_avl_tree(const size_t size, const compare_fn compare);
void destroy_avl_tree(avl_tree_s * tree, const destroy_fn destroy);
void clear_avl_tree(avl_tree_s * tree, const destroy_fn destroy);
avl_tree_s copy_avl_tree(const avl_tree_s tree, const copy_fn copy);
bool is_empty_avl_tree(const avl_tree_s tree);
void insert_avl_tree(avl_tree_s * tree, const void * element);
void remove_avl_tree(avl_tree_s * tree, const void * element, void * buffer);
bool contains_avl_tree(const avl_tree_s tree, const void * element);
void get_max_avl_tree(const avl_tree_s tree, void * buffer);
void get_min_avl_tree(const avl_tree_s tree, void * buffer);
void remove_max_avl_tree(avl_tree_s * tree, void * buffer);
void remove_min_avl_tree(avl_tree_s * tree, void * buffer);
void inorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args);
void preorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args);
void postorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args);
void level_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args);

#endif // AVL_TREE_H
