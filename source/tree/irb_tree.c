#include <tree/irb_tree.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (0)

struct irb_tree_stack {
    size_t length;
    size_t * elements;
};

struct irb_tree_queue {
    size_t length, current;
    size_t * elements;
};

/// Left tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_tree_left_rotate(irb_tree_s * const tree, size_t const node);

/// Right tree rotation that moves one node up in the tree and one node down.
/// @param tree Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_tree_right_rotate(irb_tree_s * const tree, size_t const node);

/// Replace one subtree with another.
/// @param tree Structure to replace subtrees in.
/// @param u First subtree.
/// @param v Second subtree.
void _irb_tree_transplant(irb_tree_s * const tree, size_t const u, size_t const v);

/// Finds the minimum node in subtree.
/// @param tree Structure to search.
/// @param node Root of subtree.
/// @return Minimum node.
size_t _irb_tree_minimum(irb_tree_s const * const tree, size_t const node);

/// Red black tree fixup function for tree insert.
/// @param tree Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_tree_insert_fixup(irb_tree_s * const tree, size_t const node);

/// Red black tree function to remove node index.
/// @param tree Structure to remove node.
/// @param node Node to remove.
void _irb_tree_remove(irb_tree_s * const tree, size_t const node);

/// Red black tree fixup function for tree removal.
/// @param tree Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_tree_remove_fixup(irb_tree_s * const tree, size_t const node);

/// Fills the hole left after removing an element in the tree's arrays, puts rightmost element into hole.
/// @param tree Structure to fill.
/// @param hole Index of hole in structure's arrays.
void _irb_tree_fill_hole(irb_tree_s * const tree, size_t const hole);

/// Returns the floor node of element parameter.
/// @param tree Structure to search.
/// @param element Element to floor.
/// @return Floor node index of element or NIL, if no such floor exists.
size_t _irb_tree_floor(irb_tree_s const * const restrict tree, void const * const restrict element);

/// Returns the ceil node of element parameter.
/// @param tree Structure to search.
/// @param element Element to ceil.
/// @return Ceil node index of element or NIL, if no such ceil exists.
size_t _irb_tree_ceil(irb_tree_s const * const restrict tree, void const * const restrict element);

/// Returns the in-order successor node of element parameter.
/// @param tree Structure to search.
/// @param element Element to find successor.
/// @return Successor node index of element or NIL, if no successor exists.
size_t _irb_tree_successor(irb_tree_s const * const restrict tree, void const * const restrict element);

/// Returns the in-order predecessor node of element parameter.
/// @param tree Structure to search.
/// @param element Element to find predecessor.
/// @return Predecessor node index of element or NIL, if no predecessor exists.
size_t _irb_tree_predecessor(irb_tree_s const * const restrict tree, void const * const restrict element);

/// Resizes (reallocates) tree parameter arrays based on changed capacity.
/// @param tree Structure to resize.
/// @param size New size.
void _irb_tree_resize(irb_tree_s * const tree, size_t const size);

irb_tree_s create_irb_tree(size_t const size, compare_fn const compare) {
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");

    const irb_tree_s tree = {
        .root = NIL, .compare = compare, .size = size,
        .elements = standard.alloc(size, standard.arguments),
        .color = standard.alloc(sizeof(bool), standard.arguments),
        .parent = standard.alloc(sizeof(size_t), standard.arguments),
        .node[IRB_TREE_LEFT] = standard.alloc(sizeof(size_t), standard.arguments),
        .node[IRB_TREE_RIGHT] = standard.alloc(sizeof(size_t), standard.arguments),
        .allocator = &standard,
    };

    error(tree.elements && "Memory allocation failed.");
    error(tree.color && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[IRB_TREE_LEFT] && "Memory allocation failed.");
    error(tree.node[IRB_TREE_RIGHT] && "Memory allocation failed.");

    // set NIL node since the tree uses special NIL nodes
    tree.color[NIL] = IBLACK_TREE_COLOR;
    tree.parent[NIL] = tree.node[IRB_TREE_LEFT][NIL] = tree.node[IRB_TREE_RIGHT][NIL] = NIL;

    return tree;
}

irb_tree_s make_irb_tree(size_t const size, compare_fn const compare, memory_s const * const allocator) {
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");

    const irb_tree_s tree = {
        .root = NIL, .compare = compare, .size = size,
        .elements = allocator->alloc(size, allocator->arguments),
        .color = allocator->alloc(sizeof(bool), allocator->arguments),
        .parent = allocator->alloc(sizeof(size_t), allocator->arguments),
        .node[IRB_TREE_LEFT] = allocator->alloc(sizeof(size_t), allocator->arguments),
        .node[IRB_TREE_RIGHT] = allocator->alloc(sizeof(size_t), allocator->arguments),
        .allocator = allocator,
    };

    error(tree.elements && "Memory allocation failed.");
    error(tree.color && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[IRB_TREE_LEFT] && "Memory allocation failed.");
    error(tree.node[IRB_TREE_RIGHT] && "Memory allocation failed.");

    // set NIL node since the tree uses special NIL nodes
    tree.color[NIL] = IBLACK_TREE_COLOR;
    tree.parent[NIL] = tree.node[IRB_TREE_LEFT][NIL] = tree.node[IRB_TREE_RIGHT][NIL] = NIL;

    return tree;
}

void destroy_irb_tree(irb_tree_s * const tree, set_fn const destroy) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size));
    }

    tree->allocator->free(tree->elements, tree->allocator->arguments);
    tree->allocator->free(tree->color, tree->allocator->arguments);
    tree->allocator->free(tree->parent, tree->allocator->arguments);
    tree->allocator->free(tree->node[IRB_TREE_LEFT], tree->allocator->arguments);
    tree->allocator->free(tree->node[IRB_TREE_RIGHT], tree->allocator->arguments);

    memset(tree, 0, sizeof(irb_tree_s));
}

void clear_irb_tree(irb_tree_s * const tree, set_fn const destroy) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    while (tree->length) {
        tree->length--;
        destroy(tree->elements + (tree->length * tree->size));
    }

    tree->elements = tree->allocator->realloc(tree->elements, tree->size, tree->allocator->arguments);
    tree->color = tree->allocator->realloc(tree->color, sizeof(bool), tree->allocator->arguments);
    tree->parent = tree->allocator->realloc(tree->parent, sizeof(size_t), tree->allocator->arguments);
    tree->node[IRB_TREE_LEFT] = tree->allocator->realloc(tree->node[IRB_TREE_LEFT], sizeof(size_t), tree->allocator->arguments);
    tree->node[IRB_TREE_RIGHT] = tree->allocator->realloc(tree->node[IRB_TREE_RIGHT], sizeof(size_t), tree->allocator->arguments);

    error(tree->elements && "Memory allocation failed.");
    error(tree->color && "Memory allocation failed.");
    error(tree->parent && "Memory allocation failed.");
    error(tree->node[IRB_TREE_LEFT] && "Memory allocation failed.");
    error(tree->node[IRB_TREE_RIGHT] && "Memory allocation failed.");

    tree->root = NIL;
    tree->capacity = 0;
}

irb_tree_s copy_irb_tree(irb_tree_s const * const tree, copy_fn const copy) {
    error(tree && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    irb_tree_s const replica = {
        .elements = tree->allocator->alloc((tree->capacity + 1) * tree->size, tree->allocator->arguments),
        .color = tree->allocator->alloc((tree->capacity + 1) * sizeof(bool), tree->allocator->arguments),
        .parent = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arguments),
        .node[IRB_TREE_LEFT] = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arguments),
        .node[IRB_TREE_RIGHT] = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arguments),
        .allocator = tree->allocator,

        .capacity = tree->capacity, .root = tree->root, .length = tree->length, .compare = tree->compare, .size = tree->size,
    };

    // since the structure always has one additional NIL node malloc must be checked even if capacity is zero
    error(replica.elements && "Memory allocation failed.");
    error(replica.color && "Memory allocation failed.");
    error(replica.parent && "Memory allocation failed.");
    error(replica.node[IRB_TREE_LEFT] && "Memory allocation failed.");
    error(replica.node[IRB_TREE_RIGHT] && "Memory allocation failed.");

    for (size_t i = 1; i < tree->length + 1; ++i) {
        copy(replica.elements + (i * tree->size), tree->elements + (i * tree->size));
    }
    memcpy(replica.color, tree->color, (tree->length + 1) * sizeof(bool));
    memcpy(replica.parent, tree->parent, (tree->length + 1) * sizeof(size_t));
    memcpy(replica.node[IRB_TREE_LEFT], tree->node[IRB_TREE_LEFT], (tree->length + 1) * sizeof(size_t));
    memcpy(replica.node[IRB_TREE_RIGHT], tree->node[IRB_TREE_RIGHT], (tree->length + 1) * sizeof(size_t));

    return replica;
}

bool is_empty_irb_tree(irb_tree_s const * const tree) {
    error(tree && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    return !(tree->length);
}

void insert_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    if (tree->length == tree->capacity) {
        _irb_tree_resize(tree, tree->capacity + IRB_TREE_CHUNK);
    }

    size_t previous = NIL; // initially invalid for the head case when tree is empty
    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + ((*node) * tree->size));

        previous = (*node); // change parent to child

        // go to next child node
        node = (comparison <= 0) ? tree->node[IRB_TREE_LEFT] + (*node) : tree->node[IRB_TREE_RIGHT] + (*node);
    }

    (*node) = tree->length + 1; // change child index from invalid value to next empty index in array
    tree->parent[(*node)] = previous; // make child's parent into parent
    // make child's left and right indexes invalid
    tree->node[IRB_TREE_LEFT][(*node)] = tree->node[IRB_TREE_RIGHT][(*node)] = NIL;
    tree->color[(*node)] = IRED_TREE_COLOR;

    memcpy(tree->elements + ((*node) * tree->size), element, tree->size);
    tree->length++;

    _irb_tree_insert_fixup(tree, (*node));
}

void remove_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t node = tree->root; // pointer to later change actual index of the empty child
    while (NIL != node) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRB_TREE_LEFT][node] : tree->node[IRB_TREE_RIGHT][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, node);

    memcpy(buffer, tree->elements + (node * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, node);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

bool contains_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    for (size_t node = tree->root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            return true;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRB_TREE_LEFT][node] : tree->node[IRB_TREE_RIGHT][node];
    }

    return false;
}

void get_max_irb_tree(irb_tree_s const * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t maximum = tree->root;
    for (size_t i = tree->node[IRB_TREE_RIGHT][maximum]; NIL != i; i = tree->node[IRB_TREE_RIGHT][i]) {
        maximum = i;
    }

    memcpy(buffer, tree->elements + (maximum * tree->size), tree->size);
}

void get_min_irb_tree(irb_tree_s const * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t minimum = tree->root;
    for (size_t i = tree->node[IRB_TREE_LEFT][minimum]; NIL != i; i = tree->node[IRB_TREE_LEFT][i]) {
        minimum = i;
    }

    memcpy(buffer, tree->elements + (minimum * tree->size), tree->size);
}

void remove_max_irb_tree(irb_tree_s * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t maximum = tree->root;
    for (size_t i = tree->node[IRB_TREE_RIGHT][maximum]; NIL != i; i = tree->node[IRB_TREE_RIGHT][i]) {
        maximum = i;
    }

    _irb_tree_remove(tree, maximum);

    memcpy(buffer, tree->elements + (maximum * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, maximum);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_min_irb_tree(irb_tree_s * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t minimum = tree->root;
    for (size_t i = tree->node[IRB_TREE_LEFT][minimum]; NIL != i; i = tree->node[IRB_TREE_LEFT][i]) {
        minimum = i;
    }

    _irb_tree_remove(tree, minimum);

    memcpy(buffer, tree->elements + (minimum * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, minimum);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void get_floor_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t floor = _irb_tree_floor(tree, element);

    if (NIL == floor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (floor * tree->size), tree->size);
}

void get_ceil_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t ceil = _irb_tree_ceil(tree, element);

    if (NIL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (ceil * tree->size), tree->size);
}

void remove_floor_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const floor = _irb_tree_floor(tree, element);

    if (NIL == floor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, floor);

    memcpy(buffer, tree->elements + (floor * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, floor);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_ceil_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t ceil = _irb_tree_ceil(tree, element);

    if (NIL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, ceil);

    memcpy(buffer, tree->elements + (ceil * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, ceil);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void get_successor_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t successor = _irb_tree_successor(tree, element);

    if (NIL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (successor * tree->size), tree->size);
}

void get_predecessor_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t predecessor = _irb_tree_predecessor(tree, element);

    if (NIL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (predecessor * tree->size), tree->size);
}

void remove_successor_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t successor = _irb_tree_successor(tree, element);

    if (NIL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, successor);

    memcpy(buffer, tree->elements + (successor * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, successor);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_predecessor_irb_tree(irb_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    const size_t predecessor = _irb_tree_predecessor(tree, element);

    if (NIL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, predecessor);

    memcpy(buffer, tree->elements + (predecessor * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, predecessor);

    if (tree->length == tree->capacity - IRB_TREE_CHUNK) {
        _irb_tree_resize(tree, tree->length);
    }
}

void update_irb_tree(irb_tree_s const * const restrict tree, void const * const restrict latter, void * const restrict former) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(latter && "Parameter can't be NULL.");
    error(former && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRB_TREE_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != latter && "Parameters can't be equal.");
    error(tree != former && "Parameters can't be equal.");
    error(former != latter && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t node = tree->root; // pointer to later change actual index of the empty child
    while (NIL != node) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(latter, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRB_TREE_LEFT][node] : tree->node[IRB_TREE_RIGHT][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(former, tree->elements + (node * tree->size), tree->size);
    memcpy(tree->elements + (node * tree->size), latter, tree->size);
}

void inorder_irb_tree(irb_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    bool left_done = false;
    size_t node = tree->root;
    while (NIL != node) {
        while (!left_done && NIL != tree->node[IRB_TREE_LEFT][node]) {
            node = tree->node[IRB_TREE_LEFT][node];
        }

        if (!handle(tree->elements + (node * tree->size), arguments)) {
            break;
        }

        left_done = true;
        if (NIL != tree->node[IRB_TREE_RIGHT][node]) {
            left_done = false;
            node = tree->node[IRB_TREE_RIGHT][node];
        } else if (NIL != tree->parent[node]) {
            while (NIL != tree->parent[node] && node == tree->node[IRB_TREE_RIGHT][tree->parent[node]]) {
                node = tree->parent[node];
            }

            if (NIL == tree->parent[node]) {
                break;
            }

            node = tree->parent[node];
        } else {
            break;
        }
    }
}

void preorder_irb_tree(irb_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct irb_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    if (tree->length) {
        stack.elements[stack.length++] = tree->root;
    }

    while (stack.length && handle(tree->elements + (stack.elements[stack.length - 1] * tree->size), arguments)) {
        const size_t node = stack.elements[--stack.length];

        const size_t right_child = tree->node[IRB_TREE_RIGHT][node];
        if (NIL != right_child) {
            stack.elements[stack.length++] = right_child;
        }

        const size_t left_child = tree->node[IRB_TREE_LEFT][node];
        if (NIL != left_child) {
            stack.elements[stack.length++] = left_child;
        }
    }

    tree->allocator->free(stack.elements, tree->allocator->arguments);
}

void postorder_irb_tree(irb_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct irb_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    // push root node onto stack and initially save it into variable
    size_t node = tree->root;
    size_t last = NIL;
    while (stack.length || NIL != node) { // while stack is not empty OR node is valid
        if (NIL != node) { // if node is valid push it onto the stack and go to node's left child
            stack.elements[stack.length++] = node;
            node = tree->node[IRB_TREE_LEFT][node];
        } else { // else node is invalid, thus pop a new node from the stack, handle on element, and go to node's right child
            const size_t peek = stack.elements[stack.length - 1];

            const size_t peek_right = tree->node[IRB_TREE_RIGHT][peek];
            if (NIL != peek_right && peek_right != last) {
                node = peek_right;
            } else {
                if (!handle(tree->elements + (node * tree->size), arguments)) {
                    break;
                }

                last = stack.elements[--stack.length];
            }
        }
    }

    tree->allocator->free(stack.elements, tree->allocator->arguments);
}

void levelorder_irb_tree(irb_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple queue to manage breath first level order traversal of node indexes
    struct irb_tree_queue queue = {
        .length = 0, .current = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || queue.elements && "Memory allocation failed.");

    if (tree->length) { // if tree isn't empty push root node
        queue.elements[queue.current + queue.length++] = tree->root;
    }

    // while queue isn't empty handle on element, pop parent and push valid children
    while (queue.length && handle(tree->elements + (queue.elements[queue.current] * tree->size), arguments)) {
        // pop index
        const size_t node = queue.elements[queue.current++];
        queue.length--;

        // push left child of popped parent to the top of the queue
        const size_t left_child = tree->node[IRB_TREE_LEFT][node];
        if (NIL != left_child) {
            queue.elements[queue.current + queue.length++] = left_child;
        }

        // push right child of popped parent to the top of the queue
        const size_t right_child = tree->node[IRB_TREE_RIGHT][node];
        if (NIL != right_child) {
            queue.elements[queue.current + queue.length++] = right_child;
        }
    }

    tree->allocator->free(queue.elements, tree->allocator->arguments);
}

void _irb_tree_left_rotate(irb_tree_s * const tree, size_t const node) {
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

void _irb_tree_right_rotate(irb_tree_s * const tree, size_t const node) {
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

void _irb_tree_transplant(irb_tree_s * const tree, size_t const u, size_t const v) {
    if (NIL == tree->parent[u]) {
        tree->root = v;
    } else if (u == tree->node[IRB_TREE_LEFT][tree->parent[u]]) {
        tree->node[IRB_TREE_LEFT][tree->parent[u]] = v;
    } else {
        tree->node[IRB_TREE_RIGHT][tree->parent[u]] = v;
    }

    tree->parent[v] = tree->parent[u];
}

size_t _irb_tree_minimum(irb_tree_s const * const tree, size_t const node) {
    size_t n = node;
    while (NIL != tree->node[IRB_TREE_LEFT][n]) { // TREE MINIMUM
        n = tree->node[IRB_TREE_LEFT][n];
    }

    return n;
}

void _irb_tree_insert_fixup(irb_tree_s * const tree, size_t const node) {
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

void _irb_tree_remove(irb_tree_s * const tree, size_t const node) {
    size_t current = node, child = NIL;
    bool original_color = tree->color[current];
    if (NIL == tree->node[IRB_TREE_LEFT][node]) {
        child = tree->node[IRB_TREE_RIGHT][node];
        _irb_tree_transplant(tree, node, tree->node[IRB_TREE_RIGHT][node]);
    } else if (NIL == tree->node[IRB_TREE_RIGHT][node]) {
        child = tree->node[IRB_TREE_LEFT][node];
        _irb_tree_transplant(tree, node, tree->node[IRB_TREE_LEFT][node]);
    } else {
        current = _irb_tree_minimum(tree, tree->node[IRB_TREE_RIGHT][node]);
        original_color = tree->color[current];
        child = tree->node[IRB_TREE_RIGHT][current];

        if (tree->parent[current] == node) {
            tree->parent[child] = current;
        } else {
            _irb_tree_transplant(tree, current, tree->node[IRB_TREE_RIGHT][current]);
            tree->node[IRB_TREE_RIGHT][current] = tree->node[IRB_TREE_RIGHT][node];
            tree->parent[tree->node[IRB_TREE_RIGHT][current]] = current;
        }
        _irb_tree_transplant(tree, node, current);
        tree->node[IRB_TREE_LEFT][current] = tree->node[IRB_TREE_LEFT][node];
        tree->parent[tree->node[IRB_TREE_LEFT][current]] = current;
        tree->color[current] = tree->color[node];
    }

    if (IBLACK_TREE_COLOR == original_color) {
        _irb_tree_remove_fixup(tree, child);
    }

    // fix NIL node
    tree->color[NIL] = IBLACK_TREE_COLOR;
    tree->parent[NIL] = tree->node[IRB_TREE_LEFT][NIL] = tree->node[IRB_TREE_RIGHT][NIL] = NIL;
}

void _irb_tree_remove_fixup(irb_tree_s * const tree, size_t const node) {
    size_t child = node;
    while (child != tree->root && IBLACK_TREE_COLOR == tree->color[child]) {
        if (child == tree->node[IRB_TREE_LEFT][tree->parent[child]]) {
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
                    tree->color[tree->node[IRB_TREE_RIGHT][sibling]] = IBLACK_TREE_COLOR;
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

    tree->color[child] = IBLACK_TREE_COLOR;
}

void _irb_tree_fill_hole(irb_tree_s * const tree, size_t const hole) {
    const size_t last = tree->length + 1;
    if (tree->length && tree->root == last) { // if head node is last array element then change index to removed one
        tree->root = hole;
    }

    // cut hole node from the rest of the tree
    tree->node[IRB_TREE_LEFT][hole] = tree->node[IRB_TREE_RIGHT][hole] = tree->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memmove(tree->elements + (hole * tree->size), tree->elements + (last * tree->size), tree->size);
    tree->node[IRB_TREE_LEFT][hole] = tree->node[IRB_TREE_LEFT][last];
    tree->node[IRB_TREE_RIGHT][hole] = tree->node[IRB_TREE_RIGHT][last];
    tree->parent[hole] = tree->parent[last];
    tree->color[hole] = tree->color[last];

    // redirect left child of rightmost array node if they don't overlap with removed index
    const size_t left_last = tree->node[IRB_TREE_LEFT][last];
    if (NIL != left_last) {
        tree->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    const size_t right_last = tree->node[IRB_TREE_RIGHT][last];
    if (NIL != right_last) {
        tree->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    const size_t parent_last = tree->parent[last];
    if (NIL != parent_last) {
        const int comparison = tree->compare(tree->elements + (last * tree->size), tree->elements + (parent_last * tree->size));
        const size_t node_index = comparison <= 0 ? IRB_TREE_LEFT : IRB_TREE_RIGHT;
        tree->node[node_index][parent_last] = hole;
    }
}

size_t _irb_tree_floor(irb_tree_s const * const restrict tree, void const * const restrict element) {
    size_t floor = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (!comparison) {
            floor = n;
            break;
        }
        if (comparison > 0) {
            floor = n;
        }

        n = comparison < 0 ? tree->node[IRB_TREE_LEFT][n] : tree->node[IRB_TREE_RIGHT][n];
    }

    return floor;
}

size_t _irb_tree_ceil(irb_tree_s const * const restrict tree, void const * const restrict element) {
    size_t ceil = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (!comparison) {
            ceil = n;
            break;
        }
        if (comparison < 0) {
            ceil = n;
        }

        n = comparison < 0 ? tree->node[IRB_TREE_LEFT][n] : tree->node[IRB_TREE_RIGHT][n];
    }

    return ceil;
}

size_t _irb_tree_successor(irb_tree_s const * const restrict tree, void const * const restrict element) {
    size_t successor = NIL;

    if (!tree->compare(element, tree->elements + (tree->root * tree->size)) && NIL != tree->node[IRB_TREE_RIGHT][tree->root]) {
        for (successor = tree->node[IRB_TREE_RIGHT][tree->root]; NIL != tree->node[IRB_TREE_LEFT][successor];) {
            successor = tree->node[IRB_TREE_LEFT][successor];
        }

        return successor;
    }

    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (comparison < 0) {
            successor = n;
        }

        n = comparison < 0 ? tree->node[IRB_TREE_LEFT][n] : tree->node[IRB_TREE_RIGHT][n];
    }

    return successor;
}

size_t _irb_tree_predecessor(irb_tree_s const * const restrict tree, void const * const restrict element) {
    size_t predecessor = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (comparison > 0) {
            predecessor = n;
        } else if (!comparison) {
            if (NIL != tree->node[IRB_TREE_LEFT][n]) {
                for (predecessor = tree->node[IRB_TREE_LEFT][n]; NIL != tree->node[IRB_TREE_RIGHT][predecessor];) {
                    predecessor = tree->node[IRB_TREE_RIGHT][predecessor];
                }
            }
            break;
        }

        n = comparison < 0 ? tree->node[IRB_TREE_LEFT][n] : tree->node[IRB_TREE_RIGHT][n];
    }

    return predecessor;
}

void _irb_tree_resize(irb_tree_s * const tree, size_t const size) {
    tree->capacity = size;
    const size_t resize = tree->capacity + 1;

    tree->elements = tree->allocator->realloc(tree->elements, resize * tree->size, tree->allocator->arguments);
    error(tree->elements && "Memory allocation failed.");

    tree->color = tree->allocator->realloc(tree->color, resize * sizeof(bool), tree->allocator->arguments);
    error(tree->color && "Memory allocation failed.");

    tree->parent = tree->allocator->realloc(tree->parent, resize * sizeof(size_t), tree->allocator->arguments);
    error(tree->parent && "Memory allocation failed.");

    tree->node[IRB_TREE_LEFT] = tree->allocator->realloc(tree->node[IRB_TREE_LEFT], resize * sizeof(size_t), tree->allocator->arguments);
    error(tree->node[IRB_TREE_LEFT] && "Memory allocation failed.");

    tree->node[IRB_TREE_RIGHT] = tree->allocator->realloc(tree->node[IRB_TREE_RIGHT], resize * sizeof(size_t), tree->allocator->arguments);
    error(tree->node[IRB_TREE_RIGHT] && "Memory allocation failed.");
}
