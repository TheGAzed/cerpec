#include <tree/avl_tree.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL ((size_t)(-1))

/// Returns the height of the node, or 0 if node is NIL.
/// @param tree Structure to get height from.
/// @param node Index of node to get height.
/// @return '0' if node is NIL, else a natural number height.
size_t _avl_tree_get_height(const avl_tree_s tree, const size_t node);

/// Left tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _avl_tree_left_rotate(avl_tree_s * tree, const size_t node);

/// Right tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _avl_tree_right_rotate(avl_tree_s * tree, const size_t node);

/// Rebalances the tree based on height to guarantee AVL property.
/// @param tree Structure to rebalance.
/// @param node Index of node to start rebalancing upwards.
void _avl_tree_rebalance(avl_tree_s * tree, const size_t node);

/// Binary search tree node removal fixup.
/// @param tree Structure to fix.
/// @param node Index reference to removed node.
/// @return Index of hole left behind by fixup.
size_t _avl_tree_remove_fixup(const avl_tree_s tree, size_t * node);

/// Fills the hole left after removing an element in the tree's arrays, puts rightmost element into hole.
/// @param tree Structure to fill.
/// @param hole Index of hole in structure's arrays.
void _avl_tree_fill_hole(avl_tree_s * tree, const size_t hole);

/// Resizes (reallocates) tree parameter arrays based on changed capacity.
/// @param tree Structure to resize.
void _avl_tree_resize(avl_tree_s * tree);

avl_tree_s create_avl_tree(const size_t size, const compare_fn compare) {
    assert(compare && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    return (avl_tree_s) { .root = NIL, .compare = compare, .size = size, };
}

void destroy_avl_tree(avl_tree_s * tree, const destroy_fn destroy) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size), tree->size);
    }
    free(tree->elements);
    free(tree->height);
    free(tree->parent);
    free(tree->node[AVL_TREE_LEFT]);
    free(tree->node[AVL_TREE_RIGHT]);

    memset(tree, 0, sizeof(avl_tree_s));
}

void clear_avl_tree(avl_tree_s * tree, const destroy_fn destroy) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size), tree->size);
    }
    free(tree->elements);
    free(tree->height);
    free(tree->parent);
    free(tree->node[AVL_TREE_LEFT]);
    free(tree->node[AVL_TREE_RIGHT]);

    tree->elements = NULL;
    tree->parent = tree->node[AVL_TREE_LEFT] = tree->node[AVL_TREE_RIGHT] = NULL;

    tree->root = NIL;
    tree->capacity = 0;
}

avl_tree_s copy_avl_tree(const avl_tree_s tree, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const avl_tree_s replica = {
        .elements = malloc(tree.capacity * tree.size),
        .height = malloc(tree.capacity * sizeof(size_t)),
        .parent = malloc(tree.capacity * sizeof(size_t)),
        .node[AVL_TREE_LEFT] = malloc(tree.capacity * sizeof(size_t)),
        .node[AVL_TREE_RIGHT] = malloc(tree.capacity * sizeof(size_t)),

        .capacity = tree.capacity, .root = tree.root, .length = tree.length, .compare = tree.compare, .size = tree.size,
    };
    assert((!tree.capacity || tree.elements) && "[ERROR] Memory allocation failed.");
    assert((!tree.capacity || tree.parent) && "[ERROR] Memory allocation failed.");
    assert((!tree.capacity || tree.node[AVL_TREE_LEFT]) && "[ERROR] Memory allocation failed.");
    assert((!tree.capacity || tree.node[AVL_TREE_RIGHT]) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < tree.length; ++i) {
        copy(replica.elements + (i * tree.size), tree.elements + (i * tree.size), replica.size);
    }
    memcpy(replica.height, tree.height, tree.length * sizeof(size_t));
    memcpy(replica.parent, tree.parent, tree.length * sizeof(size_t));
    memcpy(replica.node[AVL_TREE_LEFT], tree.node[AVL_TREE_LEFT], tree.length * sizeof(size_t));
    memcpy(replica.node[AVL_TREE_RIGHT], tree.node[AVL_TREE_RIGHT], tree.length * sizeof(size_t));

    return replica;
}

bool is_empty_avl_tree(const avl_tree_s tree) {
    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !tree.length;
}

void insert_avl_tree(avl_tree_s * tree, const void * element) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    if (tree->length == tree->capacity) {
        tree->capacity += AVL_TREE_CHUNK;
        _avl_tree_resize(tree);
    }

    size_t previous = NIL; // initially invalid for the head case when tree is empty
    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + ((*node) * tree->size), tree->size);
        const size_t node_index = comparison <= 0 ? AVL_TREE_LEFT : AVL_TREE_RIGHT;

        previous = (*node); // change parent to child
        node = tree->node[node_index] + (*node); // change child to proper gradnchild
    }

    (*node) = tree->length; // change child index from invalid value to next empty index in array
    tree->parent[(*node)] = previous; // make child's parent into parent
    tree->node[AVL_TREE_LEFT][(*node)] = tree->node[AVL_TREE_RIGHT][(*node)] = NIL; // make child's left and right indexes invalid

    memcpy(tree->elements + ((*node) * tree->size), element, tree->size);
    tree->length++;

    _avl_tree_rebalance(tree, (*node));
}

void remove_avl_tree(avl_tree_s * tree, const void * element, void * buffer) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + ((*node) * tree->size), tree->size);
        if (!comparison) {
            break;
        }

        const size_t node_index = comparison <= 0 ? AVL_TREE_LEFT : AVL_TREE_RIGHT;

        // change parent to child and go to next child node
        node = tree->node[node_index] + (*node);
    }

    if (NIL == (*node)) {
        // element was NOT found, thus return an error
        assert(false && "[ERROR] Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*node) * tree->size), tree->size);
    tree->length--;

    const size_t hole = _avl_tree_remove_fixup((*tree), node);
    _avl_tree_fill_hole(tree, hole);
    _avl_tree_rebalance(tree, (*node));

    if (tree->length == tree->capacity - AVL_TREE_CHUNK) {
        tree->capacity -= AVL_TREE_CHUNK;
        _avl_tree_resize(tree);
    }
}

bool contains_avl_tree(const avl_tree_s tree, const void * element) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    for (size_t node = tree.root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree.compare(element, tree.elements + (node * tree.size), tree.size);
        if (!comparison) {
            return true;
        }

        const size_t node_index = comparison <= 0 ? AVL_TREE_LEFT : AVL_TREE_RIGHT;
        node = tree.node[node_index][node]; // go to next child node
    }

    return false;
}

void get_max_avl_tree(const avl_tree_s tree, void * buffer) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    size_t maximum_node = tree.root;
    for (size_t i = tree.node[AVL_TREE_RIGHT][maximum_node]; NIL != i; i = tree.node[AVL_TREE_RIGHT][i]) {
        maximum_node = i;
    }

    memcpy(buffer, tree.elements + (maximum_node * tree.size), tree.size);
}

void get_min_avl_tree(const avl_tree_s tree, void * buffer) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    size_t minimum_node = tree.root;
    for (size_t i = tree.node[AVL_TREE_LEFT][minimum_node]; NIL != i; i = tree.node[AVL_TREE_LEFT][i]) {
        minimum_node = i;
    }

    memcpy(buffer, tree.elements + (minimum_node * tree.size), tree.size);
}

void remove_max_avl_tree(avl_tree_s * tree, void * buffer) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t * maximum_node = &(tree->root);
    for (size_t * i = tree->node[AVL_TREE_RIGHT] + (*maximum_node); NIL != (*i); i = tree->node[AVL_TREE_RIGHT] + (*i)) {
        maximum_node = i;
    }

    memcpy(buffer, tree->elements + ((*maximum_node) * tree->size), tree->size);
    tree->length--;

    const size_t hole = (*maximum_node);

    const size_t left_node = tree->node[AVL_TREE_LEFT][(*maximum_node)];
    if (NIL != left_node) {
        tree->parent[left_node] = tree->parent[(*maximum_node)];
    }
    (*maximum_node) = left_node;

    _avl_tree_fill_hole(tree, hole);
    _avl_tree_rebalance(tree, (*maximum_node));

    if (tree->length == tree->capacity - AVL_TREE_CHUNK) {
        tree->capacity -= AVL_TREE_CHUNK;
        _avl_tree_resize(tree);
    }
}

void remove_min_avl_tree(avl_tree_s * tree, void * buffer) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[AVL_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t * minimum_node = &(tree->root);
    for (size_t * i = tree->node[AVL_TREE_LEFT] + (*minimum_node); NIL != (*i); i = tree->node[AVL_TREE_LEFT] + (*i)) {
        minimum_node = i;
    }

    memcpy(buffer, tree->elements + ((*minimum_node) * tree->size), tree->size);
    tree->length--;

    const size_t hole = (*minimum_node);

    const size_t right_node = tree->node[AVL_TREE_RIGHT][(*minimum_node)];
    if (NIL != right_node) {
        tree->parent[right_node] = tree->parent[(*minimum_node)];
    }
    (*minimum_node) = right_node;

    _avl_tree_fill_hole(tree, hole);
    _avl_tree_rebalance(tree, (*minimum_node));

    if (tree->length == tree->capacity - AVL_TREE_CHUNK) {
        tree->capacity -= AVL_TREE_CHUNK;
        _avl_tree_resize(tree);
    }
}

void inorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    bool left_done = false;
    size_t node = tree.root;
    while (NIL != node) {
        while (!left_done && NIL != tree.node[AVL_TREE_LEFT][node]) {
            node = tree.node[AVL_TREE_LEFT][node];
        }

        if (!operate(tree.elements + (node * tree.size), tree.size, args)) {
            break;
        }

        left_done = true;
        if (NIL != tree.node[AVL_TREE_RIGHT][node]) {
            left_done = false;
            node = tree.node[AVL_TREE_RIGHT][node];
        } else if (NIL != tree.parent[node]) {
            while (NIL != tree.parent[node] && node == tree.node[AVL_TREE_RIGHT][tree.parent[node]]) {
                node = tree.parent[node];
            }

            if (NIL == tree.parent[node]) {
                break;
            }

            node = tree.parent[node];
        } else {
            break;
        }
    }
}

void preorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct pre_stack { size_t length; size_t * elements; } stack = {
        .length = 0, .elements = malloc(tree.length * sizeof(size_t)),
    };
    assert(!tree.length || stack.elements && "[ERROR] Memory allocation failed.");

    if (tree.length) {
        stack.elements[stack.length++] = tree.root;
    }

    while (stack.length && operate(tree.elements + (stack.elements[stack.length - 1] * tree.size), tree.size, args)) {
        const size_t node = stack.elements[--stack.length];

        const size_t right_child = tree.node[AVL_TREE_RIGHT][node];
        if (NIL != right_child) {
            stack.elements[stack.length++] = right_child;
        }

        const size_t left_child = tree.node[AVL_TREE_LEFT][node];
        if (NIL != left_child) {
            stack.elements[stack.length++] = left_child;
        }
    }

    free(stack.elements);
}

void postorder_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct post_stack { size_t length; size_t * elements; } stack = {
        .length = 0, .elements = malloc(tree.length * sizeof(size_t)),
    };
    assert(!tree.length || stack.elements && "[ERROR] Memory allocation failed.");

    // push root node onto stack and initially save it into variable
    size_t node = tree.root;
    size_t last = NIL;
    while (stack.length || NIL != node) { // while stack is not empty OR node is valid
        if (NIL != node) { // if node is valid push it onto the stack and go to node's left child
            stack.elements[stack.length++] = node;
            node = tree.node[AVL_TREE_LEFT][node];
        } else { // else node is invalid, thus pop a new node from the stack, operate on element, and go to node's right child
            const size_t peek = stack.elements[stack.length - 1];

            const size_t peek_right = tree.node[AVL_TREE_RIGHT][peek];
            if (NIL != peek_right && peek_right != last) {
                node = peek_right;
            } else {
                if (!operate(tree.elements + (node * tree.size), tree.size, args)) {
                    break;
                }

                last = stack.elements[--stack.length];
            }
        }
    }

    free(stack.elements);
}

void level_avl_tree(const avl_tree_s tree, const operate_fn operate, void * args) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // create simple queue to manage breath first level order traversal of node indexes
    struct level_queue { size_t length, current; size_t * elements; } queue = {
        .length = 0, .current = 0, .elements = malloc(tree.length * sizeof(size_t)),
    };
    assert(!tree.length || queue.elements && "[ERROR] Memory allocation failed.");

    if (tree.length) { // if tree isn't empty push root node
        queue.elements[queue.current + queue.length++] = tree.root;
    }

    // while queue isn't empty operate on element, pop parent and push valid children
    while (queue.length && operate(tree.elements + (queue.elements[queue.current] * tree.size), tree.size, args)) {
        // pop index
        const size_t node = queue.elements[queue.current++];
        queue.length--;

        // push left child of popped parent to the top of the queue
        const size_t left_child = tree.node[AVL_TREE_LEFT][node];
        if (NIL != left_child) {
            queue.elements[queue.current + queue.length++] = left_child;
        }

        // push right child of popped parent to the top of the queue
        const size_t right_child = tree.node[AVL_TREE_RIGHT][node];
        if (NIL != right_child) {
            queue.elements[queue.current + queue.length++] = right_child;
        }
    }

    free(queue.elements);
}

size_t _avl_tree_get_height(const avl_tree_s tree, const size_t node) {
    return (NIL == node ? 0 : tree.height[node]);
}

void _avl_tree_left_rotate(avl_tree_s * tree, const size_t node) {
    const size_t x = node, y = tree->node[AVL_TREE_RIGHT][x], z = tree->node[AVL_TREE_LEFT][y];

    tree->node[AVL_TREE_RIGHT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[AVL_TREE_LEFT][tree->parent[x]]) {
        tree->node[AVL_TREE_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[AVL_TREE_RIGHT][tree->parent[x]] = y;
    }

    tree->node[AVL_TREE_LEFT][y] = x;
    tree->parent[x] = y;

    const size_t x_left_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][x]);
    const size_t x_right_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][x]);
    tree->height[x] = 1 + (x_right_height > x_left_height ? x_right_height : x_left_height);

    const size_t y_left_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][y]);
    const size_t y_right_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][y]);
    tree->height[y] = 1 + (y_right_height > y_left_height ? y_right_height : y_left_height);
}

void _avl_tree_right_rotate(avl_tree_s * tree, const size_t node) {
    const size_t x = node, y = tree->node[AVL_TREE_LEFT][x], z = tree->node[AVL_TREE_RIGHT][y];

    tree->node[AVL_TREE_LEFT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[AVL_TREE_LEFT][tree->parent[x]]) {
        tree->node[AVL_TREE_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[AVL_TREE_RIGHT][tree->parent[x]] = y;
    }

    tree->node[AVL_TREE_RIGHT][y] = x;
    tree->parent[x] = y;

    const size_t x_left_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][x]);
    const size_t x_right_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][x]);
    tree->height[x] = 1 + (x_right_height > x_left_height ? x_right_height : x_left_height);

    const size_t y_left_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][y]);
    const size_t y_right_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][y]);
    tree->height[y] = 1 + (y_right_height > y_left_height ? y_right_height : y_left_height);
}

void _avl_tree_rebalance(avl_tree_s * tree, const size_t node) {
    for (size_t n = node, p = tree->parent[n]; NIL != n; n = p, p = tree->parent[n]) {
        // calculate left child's height
        const size_t left = tree->node[AVL_TREE_LEFT][n];
        const size_t left_height = _avl_tree_get_height((*tree), left);

        // calculate right child's height
        const size_t right = tree->node[AVL_TREE_RIGHT][n];
        const size_t right_height = _avl_tree_get_height((*tree), right);

        // set new height for each element popped from stack
        tree->height[n] = 1 + (left_height > right_height ? left_height : right_height);

        // calculate absolute difference of left and right child's heights
        const size_t abs_balance = left_height > right_height ? left_height - right_height : right_height - left_height;

        if (abs_balance < 2) { // if tree is balanced continue, else perform rotation/s
            continue;
        }

        if (left_height < right_height) {
            const size_t left_grand_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][right]);
            const size_t right_grand_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][right]);

            if (left_grand_height > right_grand_height) {
                _avl_tree_right_rotate(tree, right);
            }
            _avl_tree_left_rotate(tree, n);
        }

        if (left_height > right_height) {
            const size_t left_grand_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_LEFT][left]);
            const size_t right_grand_height = _avl_tree_get_height((*tree), tree->node[AVL_TREE_RIGHT][left]);

            if (left_grand_height > right_grand_height) {
                _avl_tree_left_rotate(tree, left);
            }
            _avl_tree_right_rotate(tree, n);
        }
    }
}

size_t _avl_tree_remove_fixup(const avl_tree_s tree, size_t * node) {
    // calculate the rightmost depth of the left child
    size_t left_depth = 0, * left_node = node;
    for (size_t * l = tree.node[AVL_TREE_LEFT] + (*left_node); NIL != (*l); l = tree.node[AVL_TREE_RIGHT] + (*l)) {
        left_depth++;
        left_node = l;
    }

    // calculate the leftmost depth of the right child
    size_t right_depth = 0, * right_node = node;
    for (size_t * r = tree.node[AVL_TREE_RIGHT] + (*right_node); NIL != (*r); r = tree.node[AVL_TREE_LEFT] + (*r)) {
        right_depth++;
        right_node = r;
    }

    const size_t hole = left_depth > right_depth ? (*left_node) : (*right_node);
    memcpy(tree.elements + (*node), tree.elements + hole, tree.size);
    if (left_depth > right_depth) {
        if (NIL != tree.node[AVL_TREE_LEFT][(*left_node)]) { // if right child exists cut off parent
            tree.parent[tree.node[AVL_TREE_LEFT][(*left_node)]] = tree.parent[(*left_node)];
        }
        (*left_node) = tree.node[AVL_TREE_LEFT][(*left_node)]; // cut off new hole index
    } else {
        if (NIL != tree.node[AVL_TREE_RIGHT][(*right_node)]) { // if left child exists cut off parent
            tree.parent[tree.node[AVL_TREE_RIGHT][(*right_node)]] = tree.parent[(*right_node)];
        }
        (*right_node) = tree.node[AVL_TREE_RIGHT][(*right_node)]; // cut off new hole index
    }

    return hole;
}

void _avl_tree_fill_hole(avl_tree_s * tree, const size_t hole) {
    if (tree->length && tree->root == tree->length) { // if head node is last array element then change index to removed one
        tree->root = hole;
    }

    // cut hole node from the rest of the tree
    tree->node[AVL_TREE_LEFT][hole] = tree->node[AVL_TREE_RIGHT][hole] = tree->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memcpy(tree->elements + (hole * tree->size), tree->elements + (tree->length * tree->size), tree->size);
    tree->elements[hole] = tree->elements[tree->length];
    tree->node[AVL_TREE_LEFT][hole] = tree->node[AVL_TREE_LEFT][tree->length];
    tree->node[AVL_TREE_RIGHT][hole] = tree->node[AVL_TREE_RIGHT][tree->length];
    tree->parent[hole] = tree->parent[tree->length];

    // redirect left child of rightmost array node if they don't overlap with removed index
    const size_t left_last = tree->node[AVL_TREE_LEFT][tree->length];
    if (NIL != left_last) {
        tree->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    const size_t right_last = tree->node[AVL_TREE_RIGHT][tree->length];
    if (NIL != right_last) {
        tree->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    const size_t parent_last = tree->parent[tree->length];
    if (NIL != parent_last) {
        const int comparison = tree->compare(tree->elements + (tree->length * tree->size), tree->elements + (parent_last * tree->size), tree->size);
        const size_t node_index = comparison <= 0 ? AVL_TREE_LEFT : AVL_TREE_RIGHT;
        tree->node[node_index][parent_last] = hole;
    }
}

void _avl_tree_resize(avl_tree_s * tree) {
    tree->elements = realloc(tree->elements, tree->capacity * tree->size);
    tree->height = realloc(tree->height, tree->capacity * sizeof(size_t));
    tree->parent = realloc(tree->parent, tree->capacity * sizeof(size_t));
    tree->node[AVL_TREE_LEFT] = realloc(tree->node[AVL_TREE_LEFT], tree->capacity * sizeof(size_t));
    tree->node[AVL_TREE_RIGHT] = realloc(tree->node[AVL_TREE_RIGHT], tree->capacity * sizeof(size_t));

    assert((!tree->capacity || tree->elements) && "[ERROR] Memory allocation failed.");
    assert((!tree->capacity || tree->height) && "[ERROR] Memory allocation failed.");
    assert((!tree->capacity || tree->parent) && "[ERROR] Memory allocation failed.");
    assert((!tree->capacity || tree->node[AVL_TREE_LEFT]) && "[ERROR] Memory allocation failed.");
    assert((!tree->capacity || tree->node[AVL_TREE_RIGHT]) && "[ERROR] Memory allocation failed.");
}
