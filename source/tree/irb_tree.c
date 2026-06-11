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
size_t _irb_tree_floor(irb_tree_s const * const tree, void const * const element);

/// Returns the ceil node of element parameter.
/// @param tree Structure to search.
/// @param element Element to ceil.
/// @return Ceil node index of element or NIL, if no such ceil exists.
size_t _irb_tree_ceil(irb_tree_s const * const tree, void const * const element);

/// Returns the in-order successor node of element parameter.
/// @param tree Structure to search.
/// @param element Element to find successor.
/// @return Successor node index of element or NIL, if no successor exists.
size_t _irb_tree_successor(irb_tree_s const * const tree, void const * const element);

/// Returns the in-order predecessor node of element parameter.
/// @param tree Structure to search.
/// @param element Element to find predecessor.
/// @return Predecessor node index of element or NIL, if no predecessor exists.
size_t _irb_tree_predecessor(irb_tree_s const * const tree, void const * const element);

/// Resizes (reallocates) tree parameter arrays based on changed capacity.
/// @param tree Structure to resize.
/// @param size New size.
void _irb_tree_resize(irb_tree_s * const tree, size_t const size);

irb_tree_s create_irb_tree(size_t const size, compare_fn const compare) {
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");

    irb_tree_s const tree = {
        .root = NIL, .compare = compare, .size = size,
        .elements = standard.alloc(size, standard.arg),
        .color = standard.alloc(sizeof(bool), standard.arg),
        .parent = standard.alloc(sizeof(size_t), standard.arg),
        .node[IRBT_LEFT] = standard.alloc(sizeof(size_t), standard.arg),
        .node[IRBT_RIGHT] = standard.alloc(sizeof(size_t), standard.arg),
        .allocator = &standard,
    };
    error(tree.elements && "Memory allocation failed.");
    error(tree.color && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[IRBT_LEFT] && "Memory allocation failed.");
    error(tree.node[IRBT_RIGHT] && "Memory allocation failed.");

    // set NIL node since the tree uses special NIL nodes
    tree.color[NIL] = IBLACK_COLOR;
    tree.parent[NIL] = tree.node[IRBT_LEFT][NIL] = tree.node[IRBT_RIGHT][NIL] = NIL;

    return tree;
}

irb_tree_s make_irb_tree(size_t const size, compare_fn const compare, memory_s const * const allocator) {
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");

    irb_tree_s const tree = {
        .root = NIL, .compare = compare, .size = size,
        .elements = allocator->alloc(size, allocator->arg),
        .color = allocator->alloc(sizeof(bool), allocator->arg),
        .parent = allocator->alloc(sizeof(size_t), allocator->arg),
        .node[IRBT_LEFT] = allocator->alloc(sizeof(size_t), allocator->arg),
        .node[IRBT_RIGHT] = allocator->alloc(sizeof(size_t), allocator->arg),
        .allocator = allocator,
    };
    error(tree.elements && "Memory allocation failed.");
    error(tree.color && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[IRBT_LEFT] && "Memory allocation failed.");
    error(tree.node[IRBT_RIGHT] && "Memory allocation failed.");

    // set NIL node since the tree uses special NIL nodes
    tree.color[NIL] = IBLACK_COLOR;
    tree.parent[NIL] = tree.node[IRBT_LEFT][NIL] = tree.node[IRBT_RIGHT][NIL] = NIL;

    return tree;
}

void destroy_irb_tree(irb_tree_s * const tree, set_fn const destroy, void * const ad) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < tree->length; ++i) {
        destroy(tree->elements + (i * tree->size), ad);
    }
    tree->allocator->free(tree->elements, tree->allocator->arg);
    tree->allocator->free(tree->color, tree->allocator->arg);
    tree->allocator->free(tree->parent, tree->allocator->arg);
    tree->allocator->free(tree->node[IRBT_LEFT], tree->allocator->arg);
    tree->allocator->free(tree->node[IRBT_RIGHT], tree->allocator->arg);

    memset(tree, 0, sizeof(irb_tree_s));
}

void clear_irb_tree(irb_tree_s * const tree, set_fn const destroy, void * const ad) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < tree->length; ++i) {
        destroy(tree->elements + (i * tree->size), ad);
    }

    tree->elements = tree->allocator->realloc(tree->elements, tree->size, tree->allocator->arg);
    tree->color = tree->allocator->realloc(tree->color, sizeof(bool), tree->allocator->arg);
    tree->parent = tree->allocator->realloc(tree->parent, sizeof(size_t), tree->allocator->arg);
    tree->node[IRBT_LEFT] = tree->allocator->realloc(tree->node[IRBT_LEFT], sizeof(size_t), tree->allocator->arg);
    tree->node[IRBT_RIGHT] = tree->allocator->realloc(tree->node[IRBT_RIGHT], sizeof(size_t), tree->allocator->arg);

    error(tree->elements && "Memory allocation failed.");
    error(tree->color && "Memory allocation failed.");
    error(tree->parent && "Memory allocation failed.");
    error(tree->node[IRBT_LEFT] && "Memory allocation failed.");
    error(tree->node[IRBT_RIGHT] && "Memory allocation failed.");

    tree->root = NIL;
    tree->length = tree->capacity = 0;
}

irb_tree_s copy_irb_tree(irb_tree_s const * const tree, copy_fn const copy, void * const ac) {
    error(tree && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    irb_tree_s const replica = {
        .elements = tree->allocator->alloc((tree->capacity + 1) * tree->size, tree->allocator->arg),
        .color = tree->allocator->alloc((tree->capacity + 1) * sizeof(bool), tree->allocator->arg),
        .parent = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arg),
        .node[IRBT_LEFT] = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arg),
        .node[IRBT_RIGHT] = tree->allocator->alloc((tree->capacity + 1) * sizeof(size_t), tree->allocator->arg),
        .allocator = tree->allocator,

        .capacity = tree->capacity, .root = tree->root, .length = tree->length, .compare = tree->compare, .size = tree->size,
    };

    // since the structure always has one additional NIL node malloc must be checked even if capacity is zero
    error(replica.elements && "Memory allocation failed.");
    error(replica.color && "Memory allocation failed.");
    error(replica.parent && "Memory allocation failed.");
    error(replica.node[IRBT_LEFT] && "Memory allocation failed.");
    error(replica.node[IRBT_RIGHT] && "Memory allocation failed.");

    for (size_t i = 1; i < tree->length + 1; ++i) {
        copy(replica.elements + (i * tree->size), tree->elements + (i * tree->size), ac);
    }
    memcpy(replica.color, tree->color, (tree->length + 1) * sizeof(bool));
    memcpy(replica.parent, tree->parent, (tree->length + 1) * sizeof(size_t));
    memcpy(replica.node[IRBT_LEFT], tree->node[IRBT_LEFT], (tree->length + 1) * sizeof(size_t));
    memcpy(replica.node[IRBT_RIGHT], tree->node[IRBT_RIGHT], (tree->length + 1) * sizeof(size_t));

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

void insert_irb_tree(irb_tree_s * const tree, void const * const element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    if (tree->length == tree->capacity) {
        size_t const capacity = tree->length ? tree->length * CERPEC_FACTOR : IRB_TREE_CHUNK;
        _irb_tree_resize(tree, capacity);
    }

    size_t previous = NIL; // initially invalid for the head case when tree is empty
    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*node) * tree->size));

        previous = (*node); // change parent to child

        // go to next child node
        node = (comparison <= 0) ? tree->node[IRBT_LEFT] + (*node) : tree->node[IRBT_RIGHT] + (*node);
    }

    (*node) = tree->length + 1; // change child index from invalid value to next empty index in array
    tree->parent[(*node)] = previous; // make child's parent into parent
    // make child's left and right indexes invalid
    tree->node[IRBT_LEFT][(*node)] = tree->node[IRBT_RIGHT][(*node)] = NIL;
    tree->color[(*node)] = IRED_COLOR;

    memcpy(tree->elements + ((*node) * tree->size), element, tree->size);
    tree->length++;

    _irb_tree_insert_fixup(tree, (*node));
}

void remove_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
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
        int const comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRBT_LEFT][node] : tree->node[IRBT_RIGHT][node];
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

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

bool contains_irb_tree(irb_tree_s const * const tree, void const * const element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    for (size_t node = tree->root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            return true;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRBT_LEFT][node] : tree->node[IRBT_RIGHT][node];
    }

    return false;
}

void get_max_irb_tree(irb_tree_s const * const tree, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t maximum = tree->root;
    for (size_t i = tree->node[IRBT_RIGHT][maximum]; NIL != i; i = tree->node[IRBT_RIGHT][i]) {
        maximum = i;
    }

    memcpy(buffer, tree->elements + (maximum * tree->size), tree->size);
}

void get_min_irb_tree(irb_tree_s const * const tree, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t minimum = tree->root;
    for (size_t i = tree->node[IRBT_LEFT][minimum]; NIL != i; i = tree->node[IRBT_LEFT][i]) {
        minimum = i;
    }

    memcpy(buffer, tree->elements + (minimum * tree->size), tree->size);
}

void remove_max_irb_tree(irb_tree_s * const tree, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t maximum = tree->root;
    for (size_t i = tree->node[IRBT_RIGHT][maximum]; NIL != i; i = tree->node[IRBT_RIGHT][i]) {
        maximum = i;
    }

    _irb_tree_remove(tree, maximum);

    memcpy(buffer, tree->elements + (maximum * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, maximum);

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_min_irb_tree(irb_tree_s * const tree, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != buffer && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t minimum = tree->root;
    for (size_t i = tree->node[IRBT_LEFT][minimum]; NIL != i; i = tree->node[IRBT_LEFT][i]) {
        minimum = i;
    }

    _irb_tree_remove(tree, minimum);

    memcpy(buffer, tree->elements + (minimum * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, minimum);

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void get_floor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
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

    memcpy(buffer, tree->elements + (floor * tree->size), tree->size);
}

void get_ceil_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const ceil = _irb_tree_ceil(tree, element);
    if (NIL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (ceil * tree->size), tree->size);
}

void remove_floor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
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

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_ceil_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const ceil = _irb_tree_ceil(tree, element);
    if (NIL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, ceil);

    memcpy(buffer, tree->elements + (ceil * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, ceil);

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void get_successor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const successor = _irb_tree_successor(tree, element);
    if (NIL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (successor * tree->size), tree->size);
}

void get_predecessor_irb_tree(irb_tree_s const * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const predecessor = _irb_tree_predecessor(tree, element);
    if (NIL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (predecessor * tree->size), tree->size);
}

void remove_successor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const successor = _irb_tree_successor(tree, element);
    if (NIL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, successor);

    memcpy(buffer, tree->elements + (successor * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, successor);

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void remove_predecessor_irb_tree(irb_tree_s * const tree, void const * const element, void * const buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    size_t const predecessor = _irb_tree_predecessor(tree, element);
    if (NIL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    _irb_tree_remove(tree, predecessor);

    memcpy(buffer, tree->elements + (predecessor * tree->size), tree->size);
    tree->length--;

    _irb_tree_fill_hole(tree, predecessor);

    if (tree->length <= tree->capacity / CERPEC_FACTOR && (tree->length > IRB_TREE_CHUNK || !tree->length)) {
        _irb_tree_resize(tree, tree->length);
    }
}

void update_irb_tree(irb_tree_s const * const tree, void const * const latter, void * const former) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(latter && "Parameter can't be NULL.");
    error(former && "Parameter can't be NULL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[IRBT_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[IRBT_RIGHT] && "Paremeter can't be NULL.");
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
        int const comparison = tree->compare(latter, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[IRBT_LEFT][node] : tree->node[IRBT_RIGHT][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(former, tree->elements + (node * tree->size), tree->size);
    memcpy(tree->elements + (node * tree->size), latter, tree->size);
}

void in_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != ah && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    bool left_done = false;
    size_t node = tree->root;
    while (NIL != node) {
        size_t const parent = tree->parent[node];
        size_t const left = tree->node[IRBT_LEFT][node];
        size_t const right = tree->node[IRBT_RIGHT][node];

        while (!left_done && NIL != left) {
            node = left;
        }

        if (!handle(tree->elements + (node * tree->size), ah)) { break; }

        left_done = true;
        if (NIL != right) {
            left_done = false;
            node = right;
        } else if (NIL != parent) {
            while (NIL != parent && node == tree->node[IRBT_RIGHT][parent]) { node = parent; }
            if (NIL == parent) { break; }

            node = parent;
        } else {
            break;
        }
    }
}

void pre_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != ah && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct irb_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arg),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    if (tree->length) {
        stack.elements[stack.length++] = tree->root;
    }

    while (stack.length && handle(tree->elements + (stack.elements[stack.length - 1] * tree->size), ah)) {
        size_t const node = stack.elements[--stack.length];

        size_t const right_child = tree->node[IRBT_RIGHT][node];
        if (NIL != right_child) {
            stack.elements[stack.length++] = right_child;
        }

        size_t const left_child = tree->node[IRBT_LEFT][node];
        if (NIL != left_child) {
            stack.elements[stack.length++] = left_child;
        }
    }

    tree->allocator->free(stack.elements, tree->allocator->arg);
}

void post_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != ah && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct irb_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arg),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    // push root node onto stack and initially save it into variable
    size_t node = tree->root;
    size_t last = NIL;
    while (stack.length || NIL != node) { // while stack is not empty OR node is valid
        if (NIL != node) { // if node is valid push it onto the stack and go to node's left child
            stack.elements[stack.length++] = node;
            node = tree->node[IRBT_LEFT][node];
        } else { // else node is invalid, thus pop a new node from the stack, handle on element, and go to node's right child
            size_t const peek = stack.elements[stack.length - 1];

            size_t const peek_right = tree->node[IRBT_RIGHT][peek];
            if (NIL != peek_right && peek_right != last) {
                node = peek_right;
            } else {
                if (!handle(tree->elements + (node * tree->size), ah)) {
                    break;
                }

                last = stack.elements[--stack.length];
            }
        }
    }

    tree->allocator->free(stack.elements, tree->allocator->arg);
}

void level_order_irb_tree(irb_tree_s const * const tree, handle_fn const handle, void * const ah) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != ah && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->length <= tree->capacity && "Lenght can't be larger than capacity.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->allocator && "Allocator can't be NULL.");

    // create simple queue to manage breath first level order traversal of node indexes
    struct irb_tree_queue queue = {
        .length = 0, .current = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arg),
    };
    error(!tree->length || queue.elements && "Memory allocation failed.");

    if (tree->length) { // if tree isn't empty push root node
        queue.elements[queue.current + queue.length++] = tree->root;
    }

    // while queue isn't empty handle on element, pop parent and push valid children
    while (queue.length && handle(tree->elements + (queue.elements[queue.current] * tree->size), ah)) {
        // pop index
        size_t const node = queue.elements[queue.current++];
        queue.length--;

        // push left child of popped parent to the top of the queue
        size_t const left_child = tree->node[IRBT_LEFT][node];
        if (NIL != left_child) {
            queue.elements[queue.current + queue.length++] = left_child;
        }

        // push right child of popped parent to the top of the queue
        size_t const right_child = tree->node[IRBT_RIGHT][node];
        if (NIL != right_child) {
            queue.elements[queue.current + queue.length++] = right_child;
        }
    }

    tree->allocator->free(queue.elements, tree->allocator->arg);
}

void _irb_tree_left_rotate(irb_tree_s * const tree, size_t const node) {
    size_t const x = node, y = tree->node[IRBT_RIGHT][x], z = tree->node[IRBT_LEFT][y];

    tree->node[IRBT_RIGHT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[IRBT_LEFT][tree->parent[x]]) {
        tree->node[IRBT_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[IRBT_RIGHT][tree->parent[x]] = y;
    }

    tree->node[IRBT_LEFT][y] = x;
    tree->parent[x] = y;
}

void _irb_tree_right_rotate(irb_tree_s * const tree, size_t const node) {
    size_t const x = node, y = tree->node[IRBT_LEFT][x], z = tree->node[IRBT_RIGHT][y];

    tree->node[IRBT_LEFT][x] = z;
    if (NIL != z) {
        tree->parent[z] = x;
    }
    tree->parent[y] = tree->parent[x];

    if (NIL == tree->parent[x]) {
        tree->root = y;
    } else if (x == tree->node[IRBT_LEFT][tree->parent[x]]) {
        tree->node[IRBT_LEFT][tree->parent[x]] = y;
    } else {
        tree->node[IRBT_RIGHT][tree->parent[x]] = y;
    }

    tree->node[IRBT_RIGHT][y] = x;
    tree->parent[x] = y;
}

void _irb_tree_transplant(irb_tree_s * const tree, size_t const u, size_t const v) {
    if (NIL == tree->parent[u]) {
        tree->root = v;
    } else if (u == tree->node[IRBT_LEFT][tree->parent[u]]) {
        tree->node[IRBT_LEFT][tree->parent[u]] = v;
    } else {
        tree->node[IRBT_RIGHT][tree->parent[u]] = v;
    }

    tree->parent[v] = tree->parent[u];
}

size_t _irb_tree_minimum(irb_tree_s const * const tree, size_t const node) {
    size_t n = node;
    while (NIL != tree->node[IRBT_LEFT][n]) { // TREE MINIMUM
        n = tree->node[IRBT_LEFT][n];
    }

    return n;
}

void _irb_tree_insert_fixup(irb_tree_s * const tree, size_t const node) {
    for (size_t child = node; child != tree->root && IRED_COLOR == tree->color[tree->parent[child]];) {
        if (tree->parent[child] == tree->node[IRBT_LEFT][tree->parent[tree->parent[child]]]) {
            size_t const uncle = tree->node[IRBT_RIGHT][tree->parent[tree->parent[child]]];

            if (NIL != uncle && IRED_COLOR == tree->color[uncle]) {
                tree->color[tree->parent[child]] = tree->color[uncle] = IBLACK_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_COLOR;
                child = tree->parent[tree->parent[child]];
            } else {
                if (child == tree->node[IRBT_RIGHT][tree->parent[child]]) {
                    child = tree->parent[child];
                    _irb_tree_left_rotate(tree, child);
                }

                tree->color[tree->parent[child]] = IBLACK_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[tree->parent[child]]);
            }
        } else {
            size_t const uncle = tree->node[IRBT_LEFT][tree->parent[tree->parent[child]]];

            if (NIL != uncle && IRED_COLOR == tree->color[uncle]) {
                tree->color[tree->parent[child]] = tree->color[uncle] = IBLACK_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_COLOR;
                child = tree->parent[tree->parent[child]];
            } else {
                if (child == tree->node[IRBT_LEFT][tree->parent[child]]) {
                    child = tree->parent[child];
                    _irb_tree_right_rotate(tree, child);
                }

                tree->color[tree->parent[child]] = IBLACK_COLOR;
                tree->color[tree->parent[tree->parent[child]]] = IRED_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[tree->parent[child]]);
            }
        }
    }

    // fix NIL node
    tree->color[NIL] = IBLACK_COLOR;
    tree->parent[NIL] = tree->node[IRBT_LEFT][NIL] = tree->node[IRBT_RIGHT][NIL] = NIL;

    tree->color[tree->root] = IBLACK_COLOR;
}

void _irb_tree_remove(irb_tree_s * const tree, size_t const node) {
    size_t current = node, child = NIL;
    bool original_color = tree->color[current];
    if (NIL == tree->node[IRBT_LEFT][node]) {
        child = tree->node[IRBT_RIGHT][node];
        _irb_tree_transplant(tree, node, tree->node[IRBT_RIGHT][node]);
    } else if (NIL == tree->node[IRBT_RIGHT][node]) {
        child = tree->node[IRBT_LEFT][node];
        _irb_tree_transplant(tree, node, tree->node[IRBT_LEFT][node]);
    } else {
        current = _irb_tree_minimum(tree, tree->node[IRBT_RIGHT][node]);
        original_color = tree->color[current];
        child = tree->node[IRBT_RIGHT][current];

        if (tree->parent[current] == node) {
            tree->parent[child] = current;
        } else {
            _irb_tree_transplant(tree, current, tree->node[IRBT_RIGHT][current]);
            tree->node[IRBT_RIGHT][current] = tree->node[IRBT_RIGHT][node];
            tree->parent[tree->node[IRBT_RIGHT][current]] = current;
        }
        _irb_tree_transplant(tree, node, current);
        tree->node[IRBT_LEFT][current] = tree->node[IRBT_LEFT][node];
        tree->parent[tree->node[IRBT_LEFT][current]] = current;
        tree->color[current] = tree->color[node];
    }

    if (IBLACK_COLOR == original_color) {
        _irb_tree_remove_fixup(tree, child);
    }

    // fix NIL node
    tree->color[NIL] = IBLACK_COLOR;
    tree->parent[NIL] = tree->node[IRBT_LEFT][NIL] = tree->node[IRBT_RIGHT][NIL] = NIL;
}

void _irb_tree_remove_fixup(irb_tree_s * const tree, size_t const node) {
    size_t child = node;
    while (child != tree->root && IBLACK_COLOR == tree->color[child]) {
        if (child == tree->node[IRBT_LEFT][tree->parent[child]]) {
            size_t sibling = tree->node[IRBT_RIGHT][tree->parent[child]];
            if (IRED_COLOR == tree->color[sibling]) {
                tree->color[sibling] = IBLACK_COLOR;
                tree->color[tree->parent[child]] = IRED_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[child]);
                sibling = tree->node[IRBT_RIGHT][tree->parent[child]];
            }

            size_t const left_nibling = tree->node[IRBT_LEFT][sibling];
            size_t const right_nibling = tree->node[IRBT_RIGHT][sibling];

            if (IBLACK_COLOR == tree->color[left_nibling] && IBLACK_COLOR == tree->color[right_nibling]) {
                tree->color[sibling] = IRED_COLOR;
                child = tree->parent[child];
            } else {
                if (IBLACK_COLOR == tree->color[tree->node[IRBT_RIGHT][sibling]]) {
                    tree->color[tree->node[IRBT_LEFT][sibling]] = IBLACK_COLOR;
                    tree->color[sibling] = IRED_COLOR;
                    _irb_tree_right_rotate(tree, sibling);
                    sibling = tree->node[IRBT_RIGHT][tree->parent[child]];
                }

                tree->color[sibling] = tree->color[tree->parent[child]];
                tree->color[tree->parent[child]] = IBLACK_COLOR;
                tree->color[tree->node[IRBT_RIGHT][sibling]] = IBLACK_COLOR;
                _irb_tree_left_rotate(tree, tree->parent[child]);
                child = tree->root;
            }
        } else {
            size_t sibling = tree->node[IRBT_LEFT][tree->parent[child]];
            if (IRED_COLOR == tree->color[sibling]) {
                tree->color[sibling] = IBLACK_COLOR;
                tree->color[tree->parent[child]] = IRED_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[child]);
                sibling = tree->node[IRBT_LEFT][tree->parent[child]];
            }

            size_t const left_nibling = tree->node[IRBT_LEFT][sibling];
            size_t const right_nibling = tree->node[IRBT_RIGHT][sibling];

            if (IBLACK_COLOR == tree->color[left_nibling] && IBLACK_COLOR == tree->color[right_nibling]) {
                tree->color[sibling] = IRED_COLOR;
                child = tree->parent[child];
            } else {
                if (IBLACK_COLOR == tree->color[tree->node[IRBT_LEFT][sibling]]) {
                    tree->color[tree->node[IRBT_RIGHT][sibling]] = IBLACK_COLOR;
                    tree->color[sibling] = IRED_COLOR;
                    _irb_tree_left_rotate(tree, sibling);
                    sibling = tree->node[IRBT_LEFT][tree->parent[child]];
                }

                tree->color[sibling] = tree->color[tree->parent[child]];
                tree->color[tree->parent[child]] = IBLACK_COLOR;
                tree->color[tree->node[IRBT_LEFT][sibling]] = IBLACK_COLOR;
                _irb_tree_right_rotate(tree, tree->parent[child]);
                child = tree->root;
            }
        }
    }

    tree->color[child] = IBLACK_COLOR;
}

void _irb_tree_fill_hole(irb_tree_s * const tree, size_t const hole) {
    size_t const last = tree->length + 1;
    if (tree->length && tree->root == last) { // if head node is last array element then change index to removed one
        tree->root = hole;
    }

    // cut hole node from the rest of the tree
    tree->node[IRBT_LEFT][hole] = tree->node[IRBT_RIGHT][hole] = tree->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memmove(tree->elements + (hole * tree->size), tree->elements + (last * tree->size), tree->size);
    tree->node[IRBT_LEFT][hole] = tree->node[IRBT_LEFT][last];
    tree->node[IRBT_RIGHT][hole] = tree->node[IRBT_RIGHT][last];
    tree->parent[hole] = tree->parent[last];
    tree->color[hole] = tree->color[last];

    // redirect left child of rightmost array node if they don't overlap with removed index
    size_t const left_last = tree->node[IRBT_LEFT][last];
    if (NIL != left_last) {
        tree->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    size_t const right_last = tree->node[IRBT_RIGHT][last];
    if (NIL != right_last) {
        tree->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    size_t const parent_last = tree->parent[last];
    if (NIL != parent_last) {
        int const comparison = tree->compare(tree->elements + (last * tree->size), tree->elements + (parent_last * tree->size));
        size_t const node_index = comparison <= 0 ? IRBT_LEFT : IRBT_RIGHT;
        tree->node[node_index][parent_last] = hole;
    }
}

size_t _irb_tree_floor(irb_tree_s const * const tree, void const * const element) {
    size_t floor = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (!comparison) {
            floor = n;
            break;
        }
        if (comparison > 0) {
            floor = n;
        }

        n = comparison < 0 ? tree->node[IRBT_LEFT][n] : tree->node[IRBT_RIGHT][n];
    }

    return floor;
}

size_t _irb_tree_ceil(irb_tree_s const * const tree, void const * const element) {
    size_t ceil = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (!comparison) {
            ceil = n;
            break;
        }
        if (comparison < 0) {
            ceil = n;
        }

        n = comparison < 0 ? tree->node[IRBT_LEFT][n] : tree->node[IRBT_RIGHT][n];
    }

    return ceil;
}

size_t _irb_tree_successor(irb_tree_s const * const tree, void const * const element) {
    size_t successor = NIL;

    if (!tree->compare(element, tree->elements + (tree->root * tree->size)) && NIL != tree->node[IRBT_RIGHT][tree->root]) {
        for (successor = tree->node[IRBT_RIGHT][tree->root]; NIL != tree->node[IRBT_LEFT][successor];) {
            successor = tree->node[IRBT_LEFT][successor];
        }
    } else {
        for (size_t n = tree->root; NIL != n;) {
            // calculate and determine next child node, i.e. if left or right child
            int const comparison = tree->compare(element, tree->elements + (n * tree->size));
            if (comparison < 0) {
                successor = n;
            }

            n = comparison < 0 ? tree->node[IRBT_LEFT][n] : tree->node[IRBT_RIGHT][n];
        }
    }

    return successor;
}

size_t _irb_tree_predecessor(irb_tree_s const * const tree, void const * const element) {
    size_t predecessor = NIL;
    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (comparison > 0) {
            predecessor = n;
        } else if (!comparison) {
            if (NIL != tree->node[IRBT_LEFT][n]) {
                for (predecessor = tree->node[IRBT_LEFT][n]; NIL != tree->node[IRBT_RIGHT][predecessor];) {
                    predecessor = tree->node[IRBT_RIGHT][predecessor];
                }
            }
            break;
        }

        n = comparison < 0 ? tree->node[IRBT_LEFT][n] : tree->node[IRBT_RIGHT][n];
    }

    return predecessor;
}

void _irb_tree_resize(irb_tree_s * const tree, size_t const size) {
    tree->capacity = size;
    size_t const resize = tree->capacity + 1;

    tree->elements = tree->allocator->realloc(tree->elements, resize * tree->size, tree->allocator->arg);
    error(tree->elements && "Memory allocation failed.");

    tree->color = tree->allocator->realloc(tree->color, resize * sizeof(bool), tree->allocator->arg);
    error(tree->color && "Memory allocation failed.");

    tree->parent = tree->allocator->realloc(tree->parent, resize * sizeof(size_t), tree->allocator->arg);
    error(tree->parent && "Memory allocation failed.");

    tree->node[IRBT_LEFT] = tree->allocator->realloc(tree->node[IRBT_LEFT], resize * sizeof(size_t), tree->allocator->arg);
    error(tree->node[IRBT_LEFT] && "Memory allocation failed.");

    tree->node[IRBT_RIGHT] = tree->allocator->realloc(tree->node[IRBT_RIGHT], resize * sizeof(size_t), tree->allocator->arg);
    error(tree->node[IRBT_RIGHT] && "Memory allocation failed.");
}
