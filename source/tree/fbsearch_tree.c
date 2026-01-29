#include <tree/fbsearch_tree.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL ((size_t)(-1))

struct fbsearch_tree_stack {
    size_t length;
    size_t * elements;
};

struct fbsearch_tree_queue {
    size_t length, current;
    size_t * elements;
};

/// @brief Helper function to get pointer index to floor of element.
/// @param tree Structure to get pointer index from.
/// @param element Element to search floor of.
/// @return Pointer to index of floor element or NULL.
size_t * _fbsearch_tree_floor(fbsearch_tree_s * const restrict tree, void const * const restrict element);

/// @brief Helper function to get pointer index to ceil of element.
/// @param tree Structure to get pointer index from.
/// @param element Element to search ceil of.
/// @return Pointer to index of ceil element or NULL.
size_t * _fbsearch_tree_ceil(fbsearch_tree_s * const restrict tree, void const * const restrict element);

/// @brief Helper function to get pointer index to successor of element.
/// @param tree Structure to get pointer index from.
/// @param element Element to search successor of.
/// @return Pointer to index of successor element or NULL.
size_t * _fbsearch_tree_successor(fbsearch_tree_s * const restrict tree, void const * const restrict element);

/// @brief Helper function to get pointer index to predecessor of element.
/// @param tree Structure to get pointer index from.
/// @param element Element to search predecessor of.
/// @return Pointer to index of predecessor element or NULL.
size_t * _fbsearch_tree_predecessor(fbsearch_tree_s * const restrict tree, void const * const restrict element);

/// Binary search tree node removal fixup.
/// @param tree Structure to fix.
/// @param node Index reference to removed node.
/// @return Index of hole left behind by fixup.
size_t _fbsearch_tree_remove_fixup(fbsearch_tree_s const * const restrict tree, size_t * const restrict node);

/// Fills the hole left after removing an element in the tree's arrays, puts rightmost element into hole.
/// @param tree Structure to fill.
/// @param hole Index of hole in structure's arrays.
void _fbsearch_tree_fill_hole(fbsearch_tree_s * const tree, size_t const hole);

fbsearch_tree_s create_fbsearch_tree(size_t const size, size_t const max, compare_fn const compare) {
    error(size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");

    fbsearch_tree_s const tree = {
        .compare = compare, .allocator = &standard, .max = max, .root = NIL, .size = size,
        .elements = standard.alloc(max * size, standard.arguments),
        .parent = standard.alloc(max * sizeof(size_t), standard.arguments),
        .node[FBST_RIGHT] = standard.alloc(max * sizeof(size_t), standard.arguments),
        .node[FBST_LEFT] = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    error(tree.elements && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[FBST_RIGHT] && "Memory allocation failed.");
    error(tree.node[FBST_LEFT] && "Memory allocation failed.");

    return tree;
}

fbsearch_tree_s make_fbsearch_tree(size_t const size, size_t const max, compare_fn const compare, memory_s const * const allocator) {
    error(size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");
    error(allocator && "Parameter can't be NULL.");

    fbsearch_tree_s const tree = {
        .compare = compare, .allocator = allocator, .max = max, .root = NIL, .size = size,
        .elements = allocator->alloc(max * size, allocator->arguments),
        .parent = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .node[FBST_RIGHT] = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .node[FBST_LEFT] = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    error(tree.elements && "Memory allocation failed.");
    error(tree.parent && "Memory allocation failed.");
    error(tree.node[FBST_RIGHT] && "Memory allocation failed.");
    error(tree.node[FBST_LEFT] && "Memory allocation failed.");

    return tree;
}

void destroy_fbsearch_tree(fbsearch_tree_s * const tree, set_fn const destroy) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    for (size_t i = 0; i < tree->length; ++i) {
        destroy(tree->elements + (i * tree->size));
    }
    tree->allocator->free(tree->elements, tree->allocator->arguments);
    tree->allocator->free(tree->parent, tree->allocator->arguments);
    tree->allocator->free(tree->node[FBST_LEFT], tree->allocator->arguments);
    tree->allocator->free(tree->node[FBST_RIGHT], tree->allocator->arguments);

    memset(tree, 0, sizeof(fbsearch_tree_s));
}

void clear_fbsearch_tree(fbsearch_tree_s * const tree, set_fn const destroy) {
    error(tree && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    for (size_t i = 0; i < tree->length; ++i) {
        destroy(tree->elements + (i * tree->size));
    }

    tree->length = 0;
    tree->root = NIL;
}

fbsearch_tree_s copy_fbsearch_tree(fbsearch_tree_s const * const tree, copy_fn const copy) {
    error(tree && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    fbsearch_tree_s const replica = {
        .elements = tree->allocator->alloc(tree->max * tree->size, tree->allocator->arguments),
        .parent = tree->allocator->alloc(tree->max * sizeof(size_t), tree->allocator->arguments),
        .node[FBST_LEFT] = tree->allocator->alloc(tree->max * sizeof(size_t), tree->allocator->arguments),
        .node[FBST_RIGHT] = tree->allocator->alloc(tree->max * sizeof(size_t), tree->allocator->arguments),

        .max = tree->max, .root = tree->root, .length = tree->length, .compare = tree->compare,
        .size = tree->size, .allocator = tree->allocator,
    };
    error(replica.elements && "Memory allocation failed.");
    error(replica.parent && "Memory allocation failed.");
    error(replica.node[FBST_LEFT] && "Memory allocation failed.");
    error(replica.node[FBST_RIGHT] && "Memory allocation failed.");

    for (size_t i = 0; i < tree->length; ++i) {
        copy(replica.elements + (i * tree->size), tree->elements + (i * tree->size));
    }
    memcpy(replica.parent, tree->parent, tree->length * sizeof(size_t));
    memcpy(replica.node[FBST_LEFT], tree->node[FBST_LEFT], tree->length * sizeof(size_t));
    memcpy(replica.node[FBST_RIGHT], tree->node[FBST_RIGHT], tree->length * sizeof(size_t));

    return replica;
}

bool is_empty_fbsearch_tree(fbsearch_tree_s const * const tree) {
    error(tree && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    return !tree->length;
}

bool is_full_fbsearch_tree(fbsearch_tree_s const * const tree) {
    error(tree && "Parameter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    return (tree->length == tree->max);
}

void insert_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t previous = NIL; // initially invalid for the head case when tree is empty
    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*node) * tree->size));

        previous = (*node); // change parent to child
        node = comparison <= 0 ? tree->node[FBST_LEFT] + (*node) : tree->node[FBST_RIGHT] + (*node);
    }

    (*node) = tree->length; // change child index from invalid value to next empty index in array
    tree->parent[(*node)] = previous; // make child's parent into parent
    tree->node[FBST_LEFT][(*node)] = tree->node[FBST_RIGHT][(*node)] = NIL; // make child's left and right indexes invalid

    memcpy(tree->elements + ((*node) * tree->size), element, tree->size);
    tree->length++;
}

void remove_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * node = &(tree->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*node) * tree->size));
        if (!comparison) {
            break;
        }

        // change parent to child and go to next child node
        node = comparison < 0 ? tree->node[FBST_LEFT] + (*node) : tree->node[FBST_RIGHT] + (*node);
    }

    if (NIL == (*node)) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*node) * tree->size), tree->size);
    tree->length--;

    size_t const hole = _fbsearch_tree_remove_fixup(tree, node);
    _fbsearch_tree_fill_hole(tree, hole);
}

bool contains_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict element) {
    error(tree && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    for (size_t node = tree->root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (node * tree->size));
        if (!comparison) {
            return true;
        }

        node = comparison < 0 ? tree->node[FBST_LEFT][node] : tree->node[FBST_RIGHT][node];
    }

    return false;
}

void get_max_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != buffer && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t maximum = tree->root;
    for (size_t i = tree->node[FBST_RIGHT][maximum]; NIL != i; i = tree->node[FBST_RIGHT][i]) {
        maximum = i;
    }

    memcpy(buffer, tree->elements + (maximum * tree->size), tree->size);
}

void get_min_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != buffer && "Parameters can't be equal.");
    error(NIL != tree->root && "Paremeter can't be NIL.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t minimum = tree->root;
    for (size_t i = tree->node[FBST_LEFT][minimum]; NIL != i; i = tree->node[FBST_LEFT][i]) {
        minimum = i;
    }

    memcpy(buffer, tree->elements + (minimum * tree->size), tree->size);
}

void remove_max_fbsearch_tree(fbsearch_tree_s * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != buffer && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * maximum = &(tree->root);
    for (size_t * i = tree->node[FBST_RIGHT] + (*maximum); NIL != (*i); i = tree->node[FBST_RIGHT] + (*i)) {
        maximum = i;
    }

    memcpy(buffer, tree->elements + ((*maximum) * tree->size), tree->size);
    tree->length--;

    size_t const hole = (*maximum);

    size_t const left_node = tree->node[FBST_LEFT][(*maximum)];
    if (NIL != left_node) {
        tree->parent[left_node] = tree->parent[(*maximum)];
    }
    (*maximum) = left_node;

    _fbsearch_tree_fill_hole(tree, hole);
}

void remove_min_fbsearch_tree(fbsearch_tree_s * const restrict tree, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != buffer && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * minimum = &(tree->root);
    for (size_t * i = tree->node[FBST_LEFT] + (*minimum); NIL != (*i); i = tree->node[FBST_LEFT] + (*i)) {
        minimum = i;
    }

    memcpy(buffer, tree->elements + ((*minimum) * tree->size), tree->size);
    tree->length--;

    size_t const hole = (*minimum);

    size_t const right_node = tree->node[FBST_RIGHT][(*minimum)];
    if (NIL != right_node) {
        tree->parent[right_node] = tree->parent[(*minimum)];
    }
    (*minimum) = right_node;

    _fbsearch_tree_fill_hole(tree, hole);
}

void get_floor_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

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

        n = comparison < 0 ? tree->node[FBST_LEFT][n] : tree->node[FBST_RIGHT][n];
    }

    if (NIL == floor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in tree->");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (floor * tree->size), tree->size);
}

void get_ceil_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

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

        n = comparison < 0 ? tree->node[FBST_LEFT][n] : tree->node[FBST_RIGHT][n];
    }

    if (NIL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in tree->");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (ceil * tree->size), tree->size);
}

void remove_floor_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * floor = _fbsearch_tree_floor(tree, element);
    if (NULL == floor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*floor) * tree->size), tree->size);
    tree->length--;

    size_t const hole = _fbsearch_tree_remove_fixup(tree, floor);
    _fbsearch_tree_fill_hole(tree, hole);
}

void remove_ceil_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * ceil = _fbsearch_tree_ceil(tree, element);
    if (NULL == ceil) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*ceil) * tree->size), tree->size);
    tree->length--;

    size_t const hole = _fbsearch_tree_remove_fixup(tree, ceil);
    _fbsearch_tree_fill_hole(tree, hole);
}

void get_successor_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    // case: root is same as target and right child is not empty
    size_t successor = NIL;
    size_t const right = tree->node[FBST_RIGHT][tree->root];
    if (!tree->compare(element, tree->elements + (tree->root * tree->size)) && NIL != right) {
        for (successor = right; NIL != tree->node[FBST_LEFT][successor];) {
            successor = tree->node[FBST_LEFT][successor];
        }

        goto SUCCESSOR_CHECK; // skip to avoid binary tree search
    }

    for (size_t n = tree->root; NIL != n;) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (comparison < 0) {
            successor = n;
        }

        n = comparison < 0 ? tree->node[FBST_LEFT][n] : tree->node[FBST_RIGHT][n];
    }

SUCCESSOR_CHECK:
    if (NIL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in tree.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (successor * tree->size), tree->size);
}

void get_predecessor_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t predecessor = NIL;
    for (size_t n = tree->root; NIL != n;) {
        size_t const left = tree->node[FBST_LEFT][n];
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + (n * tree->size));
        if (comparison > 0) {
            predecessor = n;
        } else if (!comparison) {
            if (NIL != left) {
                for (predecessor = left; NIL != tree->node[FBST_RIGHT][predecessor];) {
                    predecessor = tree->node[FBST_RIGHT][predecessor];
                }
            }
            break;
        }

        n = comparison < 0 ? left : tree->node[FBST_RIGHT][n];
    }

    if (NIL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in tree.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + (predecessor * tree->size), tree->size);
}

void remove_successor_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * successor = _fbsearch_tree_successor(tree, element);
    if (NULL == successor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*successor) * tree->size), tree->size);
    tree->length--;

    size_t const hole = _fbsearch_tree_remove_fixup(tree, successor);
    _fbsearch_tree_fill_hole(tree, hole);
}

void remove_predecessor_fbsearch_tree(fbsearch_tree_s * const restrict tree, void const * const restrict element, void * const restrict buffer) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(buffer && "Parameter can't be NULL.");
    error(tree != element && "Parameters can't be equal.");
    error(tree != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t * predecessor = _fbsearch_tree_predecessor(tree, element);
    if (NULL == predecessor) {
        // element was NOT found, thus return an error
        error(false && "Element not found in structure.");
        exit(EXIT_FAILURE);
    }

    memcpy(buffer, tree->elements + ((*predecessor) * tree->size), tree->size);
    tree->length--;

    size_t const hole = _fbsearch_tree_remove_fixup(tree, predecessor);
    _fbsearch_tree_fill_hole(tree, hole);
}

void update_fbsearch_tree(fbsearch_tree_s const * const restrict tree, void const * const restrict latter, void * const restrict former) {
    error(tree && "Parameter can't be NULL.");
    error(tree->length && "Can't get element from empty structure.");
    error(latter && "Parameter can't be NULL.");
    error(former && "Parameter can't be NULL.");
    error(tree != latter && "Parameters can't be equal.");
    error(tree != former && "Parameters can't be equal.");
    error(former != latter && "Parameters can't be equal.");
    error(tree->elements && "Paremeter can't be NULL.");
    error(tree->parent && "Paremeter can't be NULL.");
    error(tree->node[FBST_LEFT] && "Paremeter can't be NULL.");
    error(tree->node[FBST_RIGHT] && "Paremeter can't be NULL.");
    error(NIL != tree->root && "Paremeter can't be NIL.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    size_t node = tree->root; // pointer to later change actual index of the empty child
    while (NIL != node) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(latter, tree->elements + (node * tree->size));
        if (!comparison) {
            break;
        }

        // go to next child node
        node = (comparison < 0) ? tree->node[FBST_LEFT][node] : tree->node[FBST_RIGHT][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        error(false && "Element not found in tree.");
        exit(EXIT_FAILURE);
    }

    memcpy(former, tree->elements + (node * tree->size), tree->size);
    memcpy(tree->elements + (node * tree->size), latter, tree->size);
}

void in_order_fbsearch_tree(fbsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    bool left_done = false;
    size_t node = tree->root;
    while (NIL != node) {
        while (!left_done && NIL != tree->node[FBST_LEFT][node]) {
            node = tree->node[FBST_LEFT][node];
        }

        if (!handle(tree->elements + (node * tree->size), arguments)) {
            break;
        }

        left_done = true;
        if (NIL != tree->node[FBST_RIGHT][node]) {
            left_done = false;
            node = tree->node[FBST_RIGHT][node];
        } else if (NIL != tree->parent[node]) {
            while (NIL != tree->parent[node] && node == tree->node[FBST_RIGHT][tree->parent[node]]) {
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

void pre_order_fbsearch_tree(fbsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct fbsearch_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    if (tree->length) {
        stack.elements[stack.length++] = tree->root;
    }

    while (stack.length && handle(tree->elements + (stack.elements[stack.length - 1] * tree->size), arguments)) {
        size_t const node = stack.elements[--stack.length];

        size_t const right_child = tree->node[FBST_RIGHT][node];
        if (NIL != right_child) {
            stack.elements[stack.length++] = right_child;
        }

        size_t const left_child = tree->node[FBST_LEFT][node];
        if (NIL != left_child) {
            stack.elements[stack.length++] = left_child;
        }
    }

    tree->allocator->free(stack.elements, tree->allocator->arguments);
}

void post_order_fbsearch_tree(fbsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    // create simple stack to manage depth first in-order traversal of node indexes
    struct fbsearch_tree_stack stack = {
        .length = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || stack.elements && "Memory allocation failed.");

    // push root node onto stack and initially save it into variable
    size_t node = tree->root;
    size_t last = NIL;
    while (stack.length || NIL != node) { // while stack is not empty OR node is valid
        if (NIL != node) { // if node is valid push it onto the stack and go to node's left child
            stack.elements[stack.length++] = node;
            node = tree->node[FBST_LEFT][node];
        } else { // else node is invalid, thus pop a new node from the stack, handle on element, and go to node's right child
            size_t const peek = stack.elements[stack.length - 1];

            size_t const peek_right = tree->node[FBST_RIGHT][peek];
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

void level_order_fbsearch_tree(fbsearch_tree_s const * const restrict tree, handle_fn const handle, void * const restrict arguments) {
    error(tree && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(tree != arguments && "Parameters can't be equal.");

    valid(tree->size && "Size can't be zero.");
    valid(tree->max && "Maximum size can't be zero.");
    valid(tree->compare && "Compare function can't be NULL.");
    valid(tree->length <= tree->max && "Lenght can't be larger than maximum.");
    valid(tree->allocator && "Allocator can't be NULL.");
    valid(tree->elements && "Elements array can't be NULL");
    valid(tree->parent && "Parents array can't be NULL");
    valid(tree->node[FBST_LEFT] && "Lefts array can't be NULL");
    valid(tree->node[FBST_RIGHT] && "Rights array can't be NULL");

    // create simple queue to manage breath first level order traversal of node indexes
    struct fbsearch_tree_queue queue = {
        .length = 0, .current = 0, .elements = tree->allocator->alloc(tree->length * sizeof(size_t), tree->allocator->arguments),
    };
    error(!tree->length || queue.elements && "Memory allocation failed.");

    if (tree->length) { // if tree isn't empty push root node
        queue.elements[queue.current + queue.length++] = tree->root;
    }

    // while queue isn't empty operate on element, pop parent and push valid children
    while (queue.length && handle(tree->elements + (queue.elements[queue.current] * tree->size), arguments)) {
        // pop index
        size_t const node = queue.elements[queue.current++];
        queue.length--;

        // push left child of popped parent to the top of the queue
        size_t const left_child = tree->node[FBST_LEFT][node];
        if (NIL != left_child) {
            queue.elements[queue.current + queue.length++] = left_child;
        }

        // push right child of popped parent to the top of the queue
        size_t const right_child = tree->node[FBST_RIGHT][node];
        if (NIL != right_child) {
            queue.elements[queue.current + queue.length++] = right_child;
        }
    }

    tree->allocator->free(queue.elements, tree->allocator->arguments);
}

size_t * _fbsearch_tree_floor(fbsearch_tree_s * const restrict tree, void const * const restrict element) {
    size_t * floor = NULL;
    for (size_t * n = &(tree->root); NIL != (*n);) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*n) * tree->size));
        if (!comparison) {
            floor = n;
            break;
        }
        if (comparison > 0) {
            floor = n;
        }

        n = comparison < 0 ? (tree->node[FBST_LEFT] + (*n)) : (tree->node[FBST_RIGHT] + (*n));
    }

    return floor;
}

size_t * _fbsearch_tree_ceil(fbsearch_tree_s * const restrict tree, void const * const restrict element) {
    size_t * ceil = NULL;
    for (size_t * n = &(tree->root); NIL != (*n);) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*n) * tree->size));
        if (!comparison) {
            ceil = n;
            break;
        }
        if (comparison < 0) {
            ceil = n;
        }

        n = comparison < 0 ? tree->node[FBST_LEFT] + (*n) : tree->node[FBST_RIGHT] + (*n);
    }

    return ceil;
}

size_t * _fbsearch_tree_successor(fbsearch_tree_s * const restrict tree, void const * const restrict element) {
    size_t * successor = NULL;

    size_t * const right = tree->node[FBST_RIGHT] + tree->root;
    if (!tree->compare(element, tree->elements + (tree->root * tree->size)) && NIL != (*right)) {
        for (successor = right; NIL != *(tree->node[FBST_LEFT] + (*successor));) {
            successor = tree->node[FBST_LEFT] + (*successor);
        }

        return successor;
    }

    for (size_t * n = &(tree->root); NIL != (*n);) {
        // calculate and determine next child node, i.e. if left or right child
        int const comparison = tree->compare(element, tree->elements + ((*n) * tree->size));
        if (comparison < 0) {
            successor = n;
        }

        n = comparison < 0 ? tree->node[FBST_LEFT] + (*n) : tree->node[FBST_RIGHT] + (*n);
    }

    return successor;
}

size_t * _fbsearch_tree_predecessor(fbsearch_tree_s * const restrict tree, void const * const restrict element) {
    size_t * predecessor = NULL;
    for (size_t * n = &(tree->root); NIL != (*n);) {
        size_t * const left = tree->node[FBST_LEFT] + (*n);
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = tree->compare(element, tree->elements + ((*n) * tree->size));
        if (comparison > 0) {
            predecessor = n;
        } else if (!comparison) {
            if (NIL != (*left)) {
                for (predecessor = left; NIL != *(tree->node[FBST_RIGHT] + (*predecessor));) {
                    predecessor = tree->node[FBST_RIGHT] + (*predecessor);
                }
            }
            break;
        }

        n = comparison < 0 ? left : tree->node[FBST_RIGHT] + (*n);
    }

    return predecessor;
}

void _fbsearch_tree_fill_hole(fbsearch_tree_s * const tree, size_t const hole) {
    if (tree->length && tree->root == tree->length) { // if head node is last array element then change index to removed one
        tree->root = hole;
    }

    // cut hole node from the rest of the tree
    tree->node[FBST_LEFT][hole] = tree->node[FBST_RIGHT][hole] = tree->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memmove(tree->elements + (hole * tree->size), tree->elements + (tree->length * tree->size), tree->size);
    tree->node[FBST_LEFT][hole] = tree->node[FBST_LEFT][tree->length];
    tree->node[FBST_RIGHT][hole] = tree->node[FBST_RIGHT][tree->length];
    tree->parent[hole] = tree->parent[tree->length];

    // redirect left child of rightmost array node if they don't overlap with removed index
    size_t const left_last = tree->node[FBST_LEFT][tree->length];
    if (NIL != left_last) {
        tree->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    size_t const right_last = tree->node[FBST_RIGHT][tree->length];
    if (NIL != right_last) {
        tree->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    size_t const parent_last = tree->parent[tree->length];
    if (NIL != parent_last) {
        const int comparison = tree->compare(tree->elements + (tree->length * tree->size), tree->elements + (parent_last * tree->size));
        const size_t node_index = comparison <= 0 ? FBST_LEFT : FBST_RIGHT;
        tree->node[node_index][parent_last] = hole;
    }
}

size_t _fbsearch_tree_remove_fixup(fbsearch_tree_s const * const restrict tree, size_t * const restrict node) {
    // calculate the rightmost depth of the left child
    size_t left_depth = 0, * left_node = node;
    for (size_t * l = tree->node[FBST_LEFT] + (*left_node); NIL != (*l); l = tree->node[FBST_RIGHT] + (*l)) {
        left_depth++;
        left_node = l;
    }

    // calculate the leftmost depth of the right child
    size_t right_depth = 0, * right_node = node;
    for (size_t * r = tree->node[FBST_RIGHT] + (*right_node); NIL != (*r); r = tree->node[FBST_LEFT] + (*r)) {
        right_depth++;
        right_node = r;
    }

    size_t const hole = left_depth > right_depth ? (*left_node) : (*right_node);
    memmove(tree->elements + ((*node) * tree->size), tree->elements + (hole * tree->size), tree->size);
    if (left_depth > right_depth) {
        if (NIL != tree->node[FBST_LEFT][(*left_node)]) { // if right child exists cut off parent
            tree->parent[tree->node[FBST_LEFT][(*left_node)]] = tree->parent[(*left_node)];
        }
        (*left_node) = tree->node[FBST_LEFT][(*left_node)]; // cut off new hole index
    } else {
        if (NIL != tree->node[FBST_RIGHT][(*right_node)]) { // if left child exists cut off parent
            tree->parent[tree->node[FBST_RIGHT][(*right_node)]] = tree->parent[(*right_node)];
        }
        (*right_node) = tree->node[FBST_RIGHT][(*right_node)]; // cut off new hole index
    }

    return hole;
}
