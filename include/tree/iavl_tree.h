#ifndef IAVL_TREE_H
#define IAVL_TREE_H

#include <cerpec.h>

#if !defined(IAVL_TREE_CHUNK)
#   define IAVL_TREE_CHUNK CERPEC_CHUNK
#endif

#define IAVL_TREE_NODE_COUNT 2
#define IAVL_TREE_LEFT  0
#define IAVL_TREE_RIGHT 1
typedef struct infinite_adelson_velsky_landis_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IAVL_TREE_NODE_COUNT];
    size_t * height;
    size_t size, length, capacity, root;
} iavl_tree_s;

iavl_tree_s create_iavl_tree(const size_t size, const compare_fn compare);
void destroy_iavl_tree(iavl_tree_s * tree, const destroy_fn destroy);
void clear_iavl_tree(iavl_tree_s * tree, const destroy_fn destroy);
iavl_tree_s copy_iavl_tree(const iavl_tree_s tree, const copy_fn copy);
bool is_empty_iavl_tree(const iavl_tree_s tree);
void insert_iavl_tree(iavl_tree_s * tree, const void * element);
void remove_iavl_tree(iavl_tree_s * tree, const void * element, void * buffer);
bool contains_iavl_tree(const iavl_tree_s tree, const void * element);
void get_max_iavl_tree(const iavl_tree_s tree, void * buffer);
void get_min_iavl_tree(const iavl_tree_s tree, void * buffer);
void remove_max_iavl_tree(iavl_tree_s * tree, void * buffer);
void remove_min_iavl_tree(iavl_tree_s * tree, void * buffer);
void inorder_iavl_tree(const iavl_tree_s tree, const operate_fn operate, void * args);
void preorder_iavl_tree(const iavl_tree_s tree, const operate_fn operate, void * args);
void postorder_iavl_tree(const iavl_tree_s tree, const operate_fn operate, void * args);
void level_iavl_tree(const iavl_tree_s tree, const operate_fn operate, void * args);

#endif // IAVL_TREE_H
