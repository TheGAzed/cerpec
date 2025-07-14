#ifndef IBSEARCH_TREE_H
#define IBSEARCH_TREE_H

#include <cerpec.h>

#if !defined(IBSEARCH_TREE_CHUNK)
#   define IBSEARCH_TREE_CHUNK CERPEC_CHUNK
#endif

#define IBSEARCH_TREE_NODE_COUNT 2
#define IBSEARCH_TREE_LEFT  0
#define IBSEARCH_TREE_RIGHT 1
typedef struct infinite_binary_search_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IBSEARCH_TREE_NODE_COUNT];
    size_t size, length, capacity, root;
} ibsearch_tree_s;

ibsearch_tree_s create_ibsearch_tree(const size_t size, const compare_fn compare);
void destroy_ibsearch_tree(ibsearch_tree_s * tree, const destroy_fn destroy);
void clear_ibsearch_tree(ibsearch_tree_s * tree, const destroy_fn destroy);
ibsearch_tree_s copy_ibsearch_tree(const ibsearch_tree_s tree, const copy_fn copy);
bool is_empty_ibsearch_tree(const ibsearch_tree_s tree);
void insert_ibsearch_tree(ibsearch_tree_s * tree, const void * element);
void remove_ibsearch_tree(ibsearch_tree_s * tree, const void * element, void * buffer);
bool contains_ibsearch_tree(const ibsearch_tree_s tree, const void * element);
void get_max_ibsearch_tree(const ibsearch_tree_s tree, void * buffer);
void get_min_ibsearch_tree(const ibsearch_tree_s tree, void * buffer);
void remove_max_ibsearch_tree(ibsearch_tree_s * tree, void * buffer);
void remove_min_ibsearch_tree(ibsearch_tree_s * tree, void * buffer);
void inorder_ibsearch_tree(const ibsearch_tree_s tree, const operate_fn operate, void * args);
void preorder_ibsearch_tree(const ibsearch_tree_s tree, const operate_fn operate, void * args);
void postorder_ibsearch_tree(const ibsearch_tree_s tree, const operate_fn operate, void * args);
void level_ibsearch_tree(const ibsearch_tree_s tree, const operate_fn operate, void * args);

#endif // IBSEARCH_TREE_H
