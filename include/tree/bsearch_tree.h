#ifndef BSEARCH_TREE_H
#define BSEARCH_TREE_H

#include <cerpec.h>

#if !defined(BSEARCH_TREE_CHUNK)
#   define BSEARCH_TREE_CHUNK CERPEC_CHUNK
#endif

#define BSEARCH_TREE_NODE_COUNT 2
#define BSEARCH_TREE_LEFT  0
#define BSEARCH_TREE_RIGHT 1
typedef struct bsearch_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[BSEARCH_TREE_NODE_COUNT];
    size_t size, length, capacity, root;
} bsearch_tree_s;

bsearch_tree_s create_bsearch_tree(const size_t size, const compare_fn compare);
void destroy_bsearch_tree(bsearch_tree_s * tree, const destroy_fn destroy);
void clear_bsearch_tree(bsearch_tree_s * tree, const destroy_fn destroy);
bsearch_tree_s copy_bsearch_tree(const bsearch_tree_s tree, const copy_fn copy);
bool is_empty_bsearch_tree(const bsearch_tree_s tree);
void insert_bsearch_tree(bsearch_tree_s * tree, const void * element);
void remove_bsearch_tree(bsearch_tree_s * tree, const void * element, void * buffer);
bool contains_bsearch_tree(const bsearch_tree_s tree, const void * element);
void get_max_bsearch_tree(const bsearch_tree_s tree, void * buffer);
void get_min_bsearch_tree(const bsearch_tree_s tree, void * buffer);
void remove_max_bsearch_tree(bsearch_tree_s * tree, void * buffer);
void remove_min_bsearch_tree(bsearch_tree_s * tree, void * buffer);
void inorder_bsearch_tree(const bsearch_tree_s tree, const operate_fn operate, void * args);
void preorder_bsearch_tree(const bsearch_tree_s tree, const operate_fn operate, void * args);
void postorder_bsearch_tree(const bsearch_tree_s tree, const operate_fn operate, void * args);
void level_bsearch_tree(const bsearch_tree_s tree, const operate_fn operate, void * args);

#endif // BSEARCH_TREE_H
