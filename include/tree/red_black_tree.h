#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <cerpec.h>

#if !defined(RED_BLACK_TREE_CHUNK)
#   define RED_BLACK_TREE_CHUNK CERPEC_CHUNK
#endif

#define RED_BLACK_TREE_NODE_COUNT 2
#define RED_BLACK_TREE_LEFT  0
#define RED_BLACK_TREE_RIGHT 1
#define RED_TREE_COLOR true
#define BLACK_TREE_COLOR false
typedef struct red_black_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[RED_BLACK_TREE_NODE_COUNT];
    bool * color;
    size_t size, length, capacity, root;
} red_black_tree_s;

red_black_tree_s create_red_black_tree(const size_t size, const compare_fn compare);
void destroy_red_black_tree(red_black_tree_s * tree, const destroy_fn destroy);
void clear_red_black_tree(red_black_tree_s * tree, const destroy_fn destroy);
red_black_tree_s copy_red_black_tree(const red_black_tree_s tree, const copy_fn copy);
bool is_empty_red_black_tree(const red_black_tree_s tree);
void insert_red_black_tree(red_black_tree_s * tree, const void * element);
void remove_red_black_tree(red_black_tree_s * tree, const void * element, void * buffer);
bool contains_red_black_tree(const red_black_tree_s tree, const void * element);
void get_max_red_black_tree(const red_black_tree_s tree, void * buffer);
void get_min_red_black_tree(const red_black_tree_s tree, void * buffer);
void remove_max_red_black_tree(red_black_tree_s * tree, void * buffer);
void remove_min_red_black_tree(red_black_tree_s * tree, void * buffer);
void inorder_red_black_tree(const red_black_tree_s tree, const operate_fn operate, void * args);
void preorder_red_black_tree(const red_black_tree_s tree, const operate_fn operate, void * args);
void postorder_red_black_tree(const red_black_tree_s tree, const operate_fn operate, void * args);
void level_red_black_tree(const red_black_tree_s tree, const operate_fn operate, void * args);

#endif //RED_BLACK_TREE_H
