#include <tree/irb_tree.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL (0)

/// Left tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_tree_left_rotate(irb_tree_s * tree, const size_t node);

/// Right tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_tree_right_rotate(irb_tree_s * tree, const size_t node);

/// Replace one subtree with another.
/// @param tree Structure to replace subtrees in.
/// @param u First subtree.
/// @param v Second subtree.
void _irb_tree_transplant(irb_tree_s * tree, const size_t u, const size_t v);

/// Finds the minimum node in subtree.
/// @param tree Structure to search.
/// @param node Root of subtree.
/// @return Minimum node.
size_t _irb_tree_minimum(const irb_tree_s tree, const size_t node);

/// Red black tree fixup function for tree insert.
/// @param tree Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_tree_insert_fixup(irb_tree_s * tree, const size_t node);

/// Red black tree fixup function for tree removal.
/// @param tree Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_tree_remove_fixup(irb_tree_s * tree, const size_t node);

/// Fills the hole left after removing an element in the tree's arrays, puts rightmost element into hole.
/// @param tree Structure to fill.
/// @param hole Index of hole in structure's arrays.
void _irb_tree_fill_hole(irb_tree_s * tree, const size_t hole);

/// Resizes (reallocates) tree parameter arrays based on changed capacity.
/// @param tree Structure to resize.
void _irb_tree_resize(irb_tree_s * tree);

irb_tree_s create_irb_tree(const size_t size, const compare_fn compare) {
    assert(compare && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    const irb_tree_s tree = {
        .root = NIL, .compare = compare, .size = size,
        .elements = malloc(size),
        .color = malloc(sizeof(bool)),
        .parent = malloc(sizeof(size_t)),
        .node[IRB_TREE_LEFT] = malloc(sizeof(size_t)),
        .node[IRB_TREE_RIGHT] = malloc(sizeof(size_t)),
    };

    assert(tree.elements && "[ERROR] Memory allocation failed.");
    assert(tree.color && "[ERROR] Memory allocation failed.");
    assert(tree.parent && "[ERROR] Memory allocation failed.");
    assert(tree.node[IRB_TREE_LEFT] && "[ERROR] Memory allocation failed.");
    assert(tree.node[IRB_TREE_RIGHT] && "[ERROR] Memory allocation failed.");

    // set NIL node since the tree uses special NIL nodes
    tree.color[NIL] = IBLACK_TREE_COLOR;
    tree.parent[NIL] = tree.node[IRB_TREE_LEFT][NIL] = tree.node[IRB_TREE_RIGHT][NIL] = NIL;

    return tree;
}

void destroy_irb_tree(irb_tree_s * tree, const destroy_fn destroy) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size));
    }

    free(tree->elements);
    free(tree->color);
    free(tree->parent);
    free(tree->node[IRB_TREE_LEFT]);
    free(tree->node[IRB_TREE_RIGHT]);

    memset(tree, 0, sizeof(irb_tree_s));
}

void clear_irb_tree(irb_tree_s * tree, const destroy_fn destroy) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size));
    }

    tree->elements = realloc(tree->elements, tree->size);
    tree->color = realloc(tree->elements, sizeof(bool));
    tree->parent = realloc(tree->parent, sizeof(size_t));
    tree->node[IRB_TREE_LEFT] = realloc(tree->node[IRB_TREE_LEFT], sizeof(size_t));
    tree->node[IRB_TREE_RIGHT] = realloc(tree->node[IRB_TREE_RIGHT], sizeof(size_t));

    assert(tree->elements && "[ERROR] Memory allocation failed.");
    assert(tree->color && "[ERROR] Memory allocation failed.");
    assert(tree->parent && "[ERROR] Memory allocation failed.");
    assert(tree->node[IRB_TREE_LEFT] && "[ERROR] Memory allocation failed.");
    assert(tree->node[IRB_TREE_RIGHT] && "[ERROR] Memory allocation failed.");

    tree->root = NIL;
    tree->capacity = 0;
}

irb_tree_s copy_irb_tree(const irb_tree_s tree, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const irb_tree_s replica = {
        .elements = malloc((tree.capacity + 1) * tree.size),
        .color = malloc((tree.capacity + 1) * sizeof(bool)),
        .parent = malloc((tree.capacity + 1) * sizeof(size_t)),
        .node[IRB_TREE_LEFT] = malloc((tree.capacity + 1) * sizeof(size_t)),
        .node[IRB_TREE_RIGHT] = malloc((tree.capacity + 1) * sizeof(size_t)),

        .capacity = tree.capacity, .root = tree.root, .length = tree.length, .compare = tree.compare, .size = tree.size,
    };

    // since teh structure always has one additional NIL node malloc must be checked even if capacity is zero
    assert(tree.elements && "[ERROR] Memory allocation failed.");
    assert(tree.color && "[ERROR] Memory allocation failed.");
    assert(tree.parent && "[ERROR] Memory allocation failed.");
    assert(tree.node[IRB_TREE_LEFT] && "[ERROR] Memory allocation failed.");
    assert(tree.node[IRB_TREE_RIGHT] && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < tree.length; ++i) {
        copy(replica.elements + (i * tree.size), tree.elements + (i * tree.size));
    }

    memcpy(replica.color, tree.color, tree.length * sizeof(bool));
    memcpy(replica.parent, tree.parent, tree.length * sizeof(size_t));
    memcpy(replica.node[IRB_TREE_LEFT], tree.node[IRB_TREE_LEFT], tree.length * sizeof(size_t));
    memcpy(replica.node[IRB_TREE_RIGHT], tree.node[IRB_TREE_RIGHT], tree.length * sizeof(size_t));

    return replica;
}

bool is_empty_irb_tree(const irb_tree_s tree) {
    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !tree.length;
}

void insert_irb_tree(irb_tree_s * tree, const void * element) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");

    if (tree->length == tree->capacity) {
        tree->capacity += IRB_TREE_CHUNK;
        _irb_tree_resize(tree);
    }

    size_t previous = NIL; // initially invalid for the head case when tree is empty
    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + ((*node) * tree->size));
        const size_t node_index = comparison <= 0 ? IRB_TREE_LEFT : IRB_TREE_RIGHT;

        previous = (*node); // change parent to child
        node = tree->node[node_index] + (*node); // change child to proper gradnchild
    }

    (*node) = tree->length + 1; // change child index from invalid value to next empty index in array
    tree->parent[(*node)] = previous; // make child's parent into parent
    // make child's left and right indexes invalid
    tree->node[IRB_TREE_LEFT][(*node)] = tree->node[IRB_TREE_RIGHT][(*node)] = NIL;

    memcpy(tree->elements + ((*node) * tree->size), element, tree->size);
    tree->length++;

    _irb_tree_insert_fixup(tree, (*node));
}

void remove_irb_tree(irb_tree_s * tree, const void * element, void * buffer) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t node = tree->root; // pointer to later change actual index of the empty child
    while (NIL != node) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        const size_t node_index = comparison <= 0 ? IRB_TREE_LEFT : IRB_TREE_RIGHT;

        // change parent to child and go to next child node
        node = tree->node[node_index][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        assert(false && "[ERROR] Element not found in tree.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (node * tree->size), tree->size);
    tree->length--;

    size_t current = node, child = NIL;
    bool original_color = tree->color[current];
    if (NIL == tree->node[IRB_TREE_LEFT][node]) {
        child = tree->node[IRB_TREE_RIGHT][node];
        _irb_tree_transplant(tree, node, tree->node[IRB_TREE_RIGHT][node]);
    } else if (NIL == tree->node[IRB_TREE_RIGHT][node]) {
        child = tree->node[IRB_TREE_LEFT][node];
        _irb_tree_transplant(tree, node, tree->node[IRB_TREE_LEFT][node]);
    } else {
        current = _irb_tree_minimum((*tree), tree->node[IRB_TREE_RIGHT][node]);
        original_color = tree->color[current];
        child = tree->node[IRB_TREE_RIGHT][current];

        if (tree->parent[current] == node) {
            tree->parent[child] = current;
        } else {
            _irb_tree_transplant(tree, current, tree->node[IRB_TREE_RIGHT][current]);
            tree->node[IRB_TREE_RIGHT][current] = tree->node[IRB_TREE_RIGHT][node];
            tree->parent[tree->node[IRB_TREE_RIGHT][current]] = current;
            child == NIL ? tree->parent[current] : tree->parent[node];
        }
        _irb_tree_transplant(tree, node, current);
        tree->node[IRB_TREE_LEFT][current] = tree->node[IRB_TREE_LEFT][node];
        tree->parent[tree->node[IRB_TREE_LEFT][current]] = current;
        tree->color[current] = tree->color[node];
    }

    if (IBLACK_TREE_COLOR == original_color) {
        _irb_tree_remove_fixup(tree, child);
    }

    _irb_tree_fill_hole(tree, node);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        tree->capacity -= IRB_TREE_CHUNK;
        _irb_tree_resize(tree);
    }
}

bool contains_irb_tree(const irb_tree_s tree, const void * element) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    for (size_t node = tree.root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree.compare(element, tree.elements + (node * tree.size));
        if (!comparison) {
            return true;
        }

        const size_t node_index = comparison <= 0 ? IRB_TREE_LEFT : IRB_TREE_RIGHT;
        node = tree.node[node_index][node]; // go to next child node
    }

    return false;
}

void get_max_irb_tree(const irb_tree_s tree, void * buffer) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    size_t maximum_node = tree.root;
    for (size_t i = tree.node[IRB_TREE_RIGHT][maximum_node]; NIL != i; i = tree.node[IRB_TREE_RIGHT][i]) {
        maximum_node = i;
    }

    memcpy(buffer, tree.elements + (maximum_node * tree.size), tree.size);
}

void get_min_irb_tree(const irb_tree_s tree, void * buffer) {
    assert(tree.length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree.elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree.parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree.node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree.root && "[INVALID] Paremeter can't be NIL.");

    size_t minimum_node = tree.root;
    for (size_t i = tree.node[IRB_TREE_LEFT][minimum_node]; NIL != i; i = tree.node[IRB_TREE_LEFT][i]) {
        minimum_node = i;
    }

    memcpy(buffer, tree.elements + (minimum_node * tree.size), tree.size);
}

void remove_max_irb_tree(irb_tree_s * tree, void * buffer) {
    assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t maximum_node = tree->root;
    for (size_t i = tree->node[IRB_TREE_RIGHT][maximum_node]; NIL != i; i = tree->node[IRB_TREE_RIGHT][i]) {
        maximum_node = i;
    }

    memcpy(buffer, tree->elements + (maximum_node * tree->size), tree->size);
    tree->length--;

    size_t current = maximum_node, child = NIL;
    bool original_color = tree->color[current];
    if (NIL == tree->node[IRB_TREE_LEFT][maximum_node]) {
        child = tree->node[IRB_TREE_RIGHT][maximum_node];
        _irb_tree_transplant(tree, maximum_node, tree->node[IRB_TREE_RIGHT][maximum_node]);
    } else if (NIL == tree->node[IRB_TREE_RIGHT][maximum_node]) {
        child = tree->node[IRB_TREE_LEFT][maximum_node];
        _irb_tree_transplant(tree, maximum_node, tree->node[IRB_TREE_LEFT][maximum_node]);
    } else {
        current = _irb_tree_minimum((*tree), tree->node[IRB_TREE_RIGHT][maximum_node]);
        original_color = tree->color[current];
        child = tree->node[IRB_TREE_RIGHT][current];

        if (tree->parent[current] == maximum_node) {
            tree->parent[child] = current;
        } else {
            _irb_tree_transplant(tree, current, tree->node[IRB_TREE_RIGHT][current]);
            tree->node[IRB_TREE_RIGHT][current] = tree->node[IRB_TREE_RIGHT][maximum_node];
            tree->parent[tree->node[IRB_TREE_RIGHT][current]] = current;
            child == NIL ? tree->parent[current] : tree->parent[maximum_node];
        }
        _irb_tree_transplant(tree, maximum_node, current);
        tree->node[IRB_TREE_LEFT][current] = tree->node[IRB_TREE_LEFT][maximum_node];
        tree->parent[tree->node[IRB_TREE_LEFT][current]] = current;
        tree->color[current] = tree->color[maximum_node];
    }

    if (IBLACK_TREE_COLOR == original_color) {
        _irb_tree_remove_fixup(tree, child);
    }

    _irb_tree_fill_hole(tree, maximum_node);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        tree->capacity -= IRB_TREE_CHUNK;
        _irb_tree_resize(tree);
    }
}

void remove_min_irb_tree(irb_tree_s * tree, void * buffer) {
        assert(tree && "[ERROR] Parameter can't be NULL.");
    assert(tree->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(tree->compare && "[INVALID] Parameter can't be NULL.");
    assert(tree->size && "[INVALID] Parameter can't be zero.");
    assert(tree->length <= tree->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(tree->elements && "[INVALID] Paremeter can't be NULL.");
    assert(tree->parent && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(tree->node[IRB_TREE_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != tree->root && "[INVALID] Paremeter can't be NIL.");

    size_t minimum_node = tree->root;
    for (size_t i = tree->node[IRB_TREE_LEFT][minimum_node]; NIL != i; i = tree->node[IRB_TREE_LEFT][i]) {
        minimum_node = i;
    }

    memcpy(buffer, tree->elements + (minimum_node * tree->size), tree->size);
    tree->length--;

    size_t current = minimum_node, child = NIL;
    bool original_color = tree->color[current];
    if (NIL == tree->node[IRB_TREE_LEFT][minimum_node]) {
        child = tree->node[IRB_TREE_RIGHT][minimum_node];
        _irb_tree_transplant(tree, minimum_node, tree->node[IRB_TREE_RIGHT][minimum_node]);
    } else if (NIL == tree->node[IRB_TREE_RIGHT][minimum_node]) {
        child = tree->node[IRB_TREE_LEFT][minimum_node];
        _irb_tree_transplant(tree, minimum_node, tree->node[IRB_TREE_LEFT][minimum_node]);
    } else {
        current = _irb_tree_minimum((*tree), tree->node[IRB_TREE_RIGHT][minimum_node]);
        original_color = tree->color[current];
        child = tree->node[IRB_TREE_RIGHT][current];

        if (tree->parent[current] == minimum_node) {
            tree->parent[child] = current;
        } else {
            _irb_tree_transplant(tree, current, tree->node[IRB_TREE_RIGHT][current]);
            tree->node[IRB_TREE_RIGHT][current] = tree->node[IRB_TREE_RIGHT][minimum_node];
            tree->parent[tree->node[IRB_TREE_RIGHT][current]] = current;
            child == NIL ? tree->parent[current] : tree->parent[minimum_node];
        }
        _irb_tree_transplant(tree, minimum_node, current);
        tree->node[IRB_TREE_LEFT][current] = tree->node[IRB_TREE_LEFT][minimum_node];
        tree->parent[tree->node[IRB_TREE_LEFT][current]] = current;
        tree->color[current] = tree->color[minimum_node];
    }

    if (IBLACK_TREE_COLOR == original_color) {
        _irb_tree_remove_fixup(tree, child);
    }

    _irb_tree_fill_hole(tree, minimum_node);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        tree->capacity -= IRB_TREE_CHUNK;
        _irb_tree_resize(tree);
    }
}

void inorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(tree.compare && "[INVALID] Parameter can't be NULL.");
    assert(tree.size && "[INVALID] Parameter can't be zero.");
    assert(tree.length <= tree.capacity && "[INVALID] Lenght can't be larger than capacity.");

    bool left_done = false;
    size_t node = tree.root;
    while (NIL != node) {
        while (!left_done && NIL != tree.node[IRB_TREE_LEFT][node]) {
            node = tree.node[IRB_TREE_LEFT][node];
        }

        if (!operate(tree.elements + (node * tree.size), arguments)) {
            break;
        }

        left_done = true;
        if (NIL != tree.node[IRB_TREE_RIGHT][node]) {
            left_done = false;
            node = tree.node[IRB_TREE_RIGHT][node];
        } else if (NIL != tree.parent[node]) {
            while (NIL != tree.parent[node] && node == tree.node[IRB_TREE_RIGHT][tree.parent[node]]) {
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

void preorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments) {
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

    while (stack.length && operate(tree.elements + (stack.elements[stack.length - 1] * tree.size), arguments)) {
        const size_t node = stack.elements[--stack.length];

        const size_t right_child = tree.node[IRB_TREE_RIGHT][node];
        if (NIL != right_child) {
            stack.elements[stack.length++] = right_child;
        }

        const size_t left_child = tree.node[IRB_TREE_LEFT][node];
        if (NIL != left_child) {
            stack.elements[stack.length++] = left_child;
        }
    }

    free(stack.elements);
}

void postorder_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments) {
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
            node = tree.node[IRB_TREE_LEFT][node];
        } else { // else node is invalid, thus pop a new node from the stack, operate on element, and go to node's right child
            const size_t peek = stack.elements[stack.length - 1];

            const size_t peek_right = tree.node[IRB_TREE_RIGHT][peek];
            if (NIL != peek_right && peek_right != last) {
                node = peek_right;
            } else {
                if (!operate(tree.elements + (node * tree.size), arguments)) {
                    break;
                }

                last = stack.elements[--stack.length];
            }
        }
    }

    free(stack.elements);
}

void level_irb_tree(const irb_tree_s tree, const operate_fn operate, void * arguments) {
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
    while (queue.length && operate(tree.elements + (queue.elements[queue.current] * tree.size), arguments)) {
        // pop index
        const size_t node = queue.elements[queue.current++];
        queue.length--;

        // push left child of popped parent to the top of the queue
        const size_t left_child = tree.node[IRB_TREE_LEFT][node];
        if (NIL != left_child) {
            queue.elements[queue.current + queue.length++] = left_child;
        }

        // push right child of popped parent to the top of the queue
        const size_t right_child = tree.node[IRB_TREE_RIGHT][node];
        if (NIL != right_child) {
            queue.elements[queue.current + queue.length++] = right_child;
        }
    }

    free(queue.elements);
}

void _irb_tree_left_rotate(irb_tree_s * tree, const size_t node) {
    const size_t x = node, y = tree->node[IRB_TREE_RIGHT][x], z = tree->node[IRB_TREE_LEFT][y];

    tree->node[IRB_TREE_RIGHT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[IRB_TREE_LEFT][tree->parent[x]]) {
        tree->node[IRB_TREE_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[IRB_TREE_RIGHT][tree->parent[x]] = y;
    }

    tree->node[IRB_TREE_LEFT][y] = x;
    tree->parent[x] = y;
}

void _irb_tree_right_rotate(irb_tree_s * tree, const size_t node) {
    const size_t x = node, y = tree->node[IRB_TREE_LEFT][x], z = tree->node[IRB_TREE_RIGHT][y];

    tree->node[IRB_TREE_LEFT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[IRB_TREE_LEFT][tree->parent[x]]) {
        tree->node[IRB_TREE_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[IRB_TREE_RIGHT][tree->parent[x]] = y;
    }

    tree->node[IRB_TREE_RIGHT][y] = x;
    tree->parent[x] = y;
}

void _irb_tree_transplant(irb_tree_s * tree, const size_t u, const size_t v) {
    if (NIL == tree->parent[u]) {
        tree->root = v;
    } else if (u == tree->node[IRB_TREE_LEFT][tree->parent[u]]) {
        tree->node[IRB_TREE_LEFT][tree->parent[u]] = v;
    } else {
        tree->node[IRB_TREE_RIGHT][tree->parent[u]] = v;
    }

    tree->parent[v] = tree->parent[u];
}

size_t _irb_tree_minimum(const irb_tree_s tree, const size_t node) {
    size_t n = node;
    while (NIL != tree.node[IRB_TREE_LEFT][n]) { // TREE MINIMUM
        n = tree.node[IRB_TREE_LEFT][n];
    }

    return n;
}

void _irb_tree_insert_fixup(irb_tree_s * tree, const size_t node) {
    for (size_t child = node; child != tree->root && IRED_TREE_COLOR == tree->color[tree->parent[child]];) {
        if (tree->parent[child] == tree->node[IRB_TREE_LEFT][tree->parent[tree->parent[child]]]) {
            const size_t uncle = tree->node[IRB_TREE_RIGHT][tree->parent[tree->parent[child]]];

            if (NIL != uncle && IRED_TREE_COLOR == tree->color[uncle]) {
                tree->color[tree->parent[child]] = tree->color[uncle] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_TREE_COLOR;
                child = tree->parent[tree->parent[child]];
            } else {
                if (child == tree->node[IRB_TREE_RIGHT][tree->parent[child]]) {
                    child = tree->parent[child];
                    _irb_tree_left_rotate(tree, child);
                }

                tree->color[tree->parent[child]] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_TREE_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[tree->parent[child]]);
            }
        } else {
            const size_t uncle = tree->node[IRB_TREE_LEFT][tree->parent[tree->parent[child]]];

            if (NIL != uncle && IRED_TREE_COLOR == tree->color[uncle]) {
                tree->color[tree->parent[child]] = tree->color[uncle] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_TREE_COLOR;
                child = tree->parent[tree->parent[child]];
            } else {
                if (child == tree->node[IRB_TREE_LEFT][tree->parent[child]]) {
                    child = tree->parent[child];
                    _irb_tree_right_rotate(tree, child);
                }

                tree->color[tree->parent[child]] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_TREE_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[tree->parent[child]]);
            }
        }
    }

    // fix NIL node
    tree->color[NIL] = IBLACK_TREE_COLOR;
    tree->parent[NIL] = tree->node[IRB_TREE_LEFT][NIL] = tree->node[IRB_TREE_RIGHT][NIL] = NIL;

    tree->color[tree->root] = IBLACK_TREE_COLOR;
}

void _irb_tree_remove_fixup(irb_tree_s * tree, const size_t node) {
    size_t child = node;

    while (child != tree->root && IBLACK_TREE_COLOR == tree->color[child]) {
        if (child == tree->parent[child]) {
            size_t sibling = tree->node[IRB_TREE_RIGHT][tree->parent[child]];
            if (IRED_TREE_COLOR == tree->color[sibling]) {
                tree->color[sibling] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[child]] = IRED_TREE_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[child]);
                sibling = tree->node[IRB_TREE_RIGHT][tree->parent[child]];
            }

            const size_t left_nibling = tree->node[IRB_TREE_LEFT][sibling];
            const size_t right_nibling = tree->node[IRB_TREE_RIGHT][sibling];

            if (IBLACK_TREE_COLOR == tree->color[left_nibling] && IBLACK_TREE_COLOR == tree->color[right_nibling]) {
                tree->color[sibling] = IRED_TREE_COLOR;
                child = tree->parent[child];
            } else {
                if (IBLACK_TREE_COLOR == tree->color[tree->node[IRB_TREE_RIGHT][sibling]]) {
                    tree->color[tree->node[IRB_TREE_LEFT][sibling]] = IBLACK_TREE_COLOR;
                    tree->color[sibling] = IRED_TREE_COLOR;
                    _irb_tree_right_rotate(tree, sibling);
                    sibling = tree->node[IRB_TREE_RIGHT][tree->parent[child]];
                }

                tree->color[sibling] = tree->color[tree->parent[child]];
                tree->color[tree->parent[child]] = IBLACK_TREE_COLOR;
                tree->color[tree->node[IRB_TREE_RIGHT][sibling]] = IBLACK_TREE_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[child]);
                child = tree->root;
            }
        } else {
            size_t sibling = tree->node[IRB_TREE_LEFT][tree->parent[child]];
            if (IRED_TREE_COLOR == tree->color[sibling]) {
                tree->color[sibling] = IBLACK_TREE_COLOR;
                tree->color[tree->parent[child]] = IRED_TREE_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[child]);
                sibling = tree->node[IRB_TREE_LEFT][tree->parent[child]];
            }

            const size_t left_nibling = tree->node[IRB_TREE_LEFT][sibling];
            const size_t right_nibling = tree->node[IRB_TREE_RIGHT][sibling];

            if (IBLACK_TREE_COLOR == tree->color[left_nibling] && IBLACK_TREE_COLOR == tree->color[right_nibling]) {
                tree->color[sibling] = IRED_TREE_COLOR;
                child = tree->parent[child];
            } else {
                if (IBLACK_TREE_COLOR == tree->color[tree->node[IRB_TREE_LEFT][sibling]]) {
                    tree->color[tree->node[IRB_TREE_LEFT][sibling]] = IBLACK_TREE_COLOR;
                    tree->color[sibling] = IRED_TREE_COLOR;
                    _irb_tree_left_rotate(tree, sibling);
                    sibling = tree->node[IRB_TREE_LEFT][tree->parent[child]];
                }

                tree->color[sibling] = tree->color[tree->parent[child]];
                tree->color[tree->parent[child]] = IBLACK_TREE_COLOR;
                tree->color[tree->node[IRB_TREE_LEFT][sibling]] = IBLACK_TREE_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[child]);
                child = tree->root;
            }
        }
    }

    // fix NIL node
    tree->color[NIL] = IBLACK_TREE_COLOR;
    tree->parent[NIL] = tree->node[IRB_TREE_LEFT][NIL] = tree->node[IRB_TREE_RIGHT][NIL] = NIL;

    tree->color[child] = IBLACK_TREE_COLOR;
}

void _irb_tree_fill_hole(irb_tree_s * tree, const size_t hole) {
    if (tree->length && tree->root == tree->length) { // if head node is last array element then change index to removed one
        tree->root = hole;
    }

    // cut hole node from the rest of the tree
    tree->node[IRB_TREE_LEFT][hole] = tree->node[IRB_TREE_RIGHT][hole] = tree->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memmove(tree->elements + (hole * tree->size), tree->elements + (tree->length * tree->size), tree->size);
    tree->elements[hole] = tree->elements[tree->length];
    tree->node[IRB_TREE_LEFT][hole] = tree->node[IRB_TREE_LEFT][tree->length];
    tree->node[IRB_TREE_RIGHT][hole] = tree->node[IRB_TREE_RIGHT][tree->length];
    tree->parent[hole] = tree->parent[tree->length];

    // redirect left child of rightmost array node if they don't overlap with removed index
    const size_t left_last = tree->node[IRB_TREE_LEFT][tree->length];
    if (NIL != left_last) {
        tree->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    const size_t right_last = tree->node[IRB_TREE_RIGHT][tree->length];
    if (NIL != right_last) {
        tree->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    const size_t parent_last = tree->parent[tree->length];
    if (NIL != parent_last) {
        const int comparison = tree->compare(tree->elements + (tree->length * tree->size), tree->elements + (parent_last * tree->size));
        const size_t node_index = comparison <= 0 ? IRB_TREE_LEFT : IRB_TREE_RIGHT;
        tree->node[node_index][parent_last] = hole;
    }
}

void _irb_tree_resize(irb_tree_s * tree) {
    const size_t resize = tree->capacity + 1;

    tree->elements = realloc(tree->elements, resize * tree->size);
    tree->color = realloc(tree->color, resize * sizeof(bool));
    tree->parent = realloc(tree->parent, resize * sizeof(size_t));
    tree->node[IRB_TREE_LEFT] = realloc(tree->node[IRB_TREE_LEFT], resize * sizeof(size_t));
    tree->node[IRB_TREE_RIGHT] = realloc(tree->node[IRB_TREE_RIGHT], resize * sizeof(size_t));

    assert(tree->elements && "[ERROR] Memory allocation failed.");
    assert(tree->color && "[ERROR] Memory allocation failed.");
    assert(tree->parent && "[ERROR] Memory allocation failed.");
    assert(tree->node[IRB_TREE_LEFT] && "[ERROR] Memory allocation failed.");
    assert(tree->node[IRB_TREE_RIGHT] && "[ERROR] Memory allocation failed.");
}
