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
typedef struct infinite_red_black_tree {
    compare_fn compare;
    char * elements;
    size_t * parent;
    size_t * node[IRB_TREE_NODE_COUNT];
    bool * color;
    size_t size, length, capacity, root;
} irb_tree_s;

irb_tree_s create_irb_tree(const size_t size, const compare_fn compare);
void destroy_irb_tree(irb_tree_s * tree, const destroy_fn destroy);
void clear_irb_tree(irb_tree_s * tree, const destroy_fn destroy);
irb_tree_s copy_irb_tree(const irb_tree_s tree, const copy_fn copy);
bool is_empty_irb_tree(const irb_tree_s tree);
void insert_irb_tree(irb_tree_s * tree, const void * element);
void remove_irb_tree(irb_tree_s * tree, const void * element, void * buffer);
bool contains_irb_tree(const irb_tree_s tree, const void * element);
void get_max_irb_tree(const irb_tree_s tree, void * buffer);
void get_min_irb_tree(const irb_tree_s tree, void * buffer);
void remove_max_irb_tree(irb_tree_s * tree, void * buffer);
void remove_min_irb_tree(irb_tree_s * tree, void * buffer);
void inorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);
void preorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);
void postorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);
void level_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments);

#endif // IRB_TREE_H
