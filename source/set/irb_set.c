#include <set/irb_set.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL (0)

/// Left set rotation that moves one node up in the set and one node down.
/// @param set Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_set_left_rotate(irb_set_s * set, const size_t node);

/// Right set rotation that moves one node up in the set and one node down.
/// @param set Structure to rotate.
/// @param node Index of node to start rotations from.
void _irb_set_right_rotate(irb_set_s * set, const size_t node);

/// Replace one subset with another.
/// @param set Structure to replace subsets in.
/// @param u First subset.
/// @param v Second subset.
void _irb_set_transplant(irb_set_s * set, const size_t u, const size_t v);

/// Finds the minimum node in subset.
/// @param set Structure to search.
/// @param node Root of subset.
/// @return Minimum node.
size_t _irb_set_minimum(const irb_set_s set, const size_t node);

/// Red black set fixup function for set insert.
/// @param set Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_set_insert_fixup(irb_set_s * set, const size_t node);

/// Red black set fixup function for set removal.
/// @param set Structure to fixup.
/// @param node Node to start fixup upwards.
void _irb_set_remove_fixup(irb_set_s * set, const size_t node);

/// Fills the hole left after removing an element in the set's arrays, puts rightmost element into hole.
/// @param set Structure to fill.
/// @param hole Index of hole in structure's arrays.
void _irb_set_fill_hole(irb_set_s * set, const size_t hole);

/// Resizes (reallocates) set parameter arrays based on changed capacity.
/// @param set Structure to resize.
/// @param size New size.
void _irb_set_resize(irb_set_s * set, const size_t size);

irb_set_s create_irb_set(const size_t size, const compare_fn compare) {
    assert(compare && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    // initialize structure
    const irb_set_s set = {
        .root = NIL, .compare = compare, .size = size,
        .elements = malloc(size),
        .color = malloc(sizeof(bool)),
        .parent = malloc(sizeof(size_t)),
        .node[IRB_SET_LEFT] = malloc(sizeof(size_t)),
        .node[IRB_SET_RIGHT] = malloc(sizeof(size_t)),
    };
    assert(set.elements && "[ERROR] Memory allocation failed.");
    assert(set.color && "[ERROR] Memory allocation failed.");
    assert(set.parent && "[ERROR] Memory allocation failed.");
    assert(set.node[IRB_SET_LEFT] && "[ERROR] Memory allocation failed.");
    assert(set.node[IRB_SET_RIGHT] && "[ERROR] Memory allocation failed.");

    // set NIL node since the set uses special NIL nodes
    set.color[NIL] = IBLACK_SET_COLOR;
    set.parent[NIL] = set.node[IRB_SET_LEFT][NIL] = set.node[IRB_SET_RIGHT][NIL] = NIL;

    return set;
}

void destroy_irb_set(irb_set_s * set, const destroy_fn destroy) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(set->compare && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // destroy elements in array order
    while (set->length) {
        set->length--;
        destroy(set->elements + (set->length * set->size));
    }

    // free arrays
    free(set->elements);
    free(set->color);
    free(set->parent);
    free(set->node[IRB_SET_LEFT]);
    free(set->node[IRB_SET_RIGHT]);

    // make strucutre invalid/set to zero
    memset(set, 0, sizeof(irb_set_s));
}

void clear_irb_set(irb_set_s * set, const destroy_fn destroy) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(set->compare && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // destroy elements in array order
    while (set->length) {
        set->length--;
        destroy(set->elements + (set->length * set->size));
    }

    // shrink arrays to hold only one node
    set->elements = realloc(set->elements, set->size);
    set->color = realloc(set->color, sizeof(bool));
    set->parent = realloc(set->parent, sizeof(size_t));
    set->node[IRB_SET_LEFT] = realloc(set->node[IRB_SET_LEFT], sizeof(size_t));
    set->node[IRB_SET_RIGHT] = realloc(set->node[IRB_SET_RIGHT], sizeof(size_t));

    assert(set->elements && "[ERROR] Memory allocation failed.");
    assert(set->color && "[ERROR] Memory allocation failed.");
    assert(set->parent && "[ERROR] Memory allocation failed.");
    assert(set->node[IRB_SET_LEFT] && "[ERROR] Memory allocation failed.");
    assert(set->node[IRB_SET_RIGHT] && "[ERROR] Memory allocation failed.");

    // clear (NOT destroy) structure
    set->root = NIL;
    set->capacity = 0;
}

irb_set_s copy_irb_set(const irb_set_s set, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(set.compare && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // initialize replica
    const irb_set_s replica = {
        .elements = malloc((set.capacity + 1) * set.size),
        .color = malloc((set.capacity + 1) * sizeof(bool)),
        .parent = malloc((set.capacity + 1) * sizeof(size_t)),
        .node[IRB_SET_LEFT] = malloc((set.capacity + 1) * sizeof(size_t)),
        .node[IRB_SET_RIGHT] = malloc((set.capacity + 1) * sizeof(size_t)),

        .capacity = set.capacity, .root = set.root, .length = set.length, .compare = set.compare, .size = set.size,
    };

    // since the structure always has one additional NIL node malloc must be checked even if capacity is zero
    assert(replica.elements && "[ERROR] Memory allocation failed.");
    assert(replica.color && "[ERROR] Memory allocation failed.");
    assert(replica.parent && "[ERROR] Memory allocation failed.");
    assert(replica.node[IRB_SET_LEFT] && "[ERROR] Memory allocation failed.");
    assert(replica.node[IRB_SET_RIGHT] && "[ERROR] Memory allocation failed.");

    // set NIL node since the set uses special NIL nodes
    replica.color[NIL] = IBLACK_SET_COLOR;
    replica.parent[NIL] = replica.node[IRB_SET_LEFT][NIL] = replica.node[IRB_SET_RIGHT][NIL] = NIL;

    // copy elements and indexes straight to replica
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < set.length + 1; ++i) {
        copy(replica.elements + (i * set.size), set.elements + (i * set.size));
    }
    memcpy(replica.color + 1, set.color + 1, set.length * sizeof(bool));
    memcpy(replica.parent + 1, set.parent + 1, set.length * sizeof(size_t));
    memcpy(replica.node[IRB_SET_LEFT] + 1, set.node[IRB_SET_LEFT] + 1, set.length * sizeof(size_t));
    memcpy(replica.node[IRB_SET_RIGHT] + 1, set.node[IRB_SET_RIGHT] + 1, set.length * sizeof(size_t));

    return replica;
}

bool is_empty_irb_set(const irb_set_s set) {
    assert(set.compare && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !(set.length);
}

void insert_irb_set(irb_set_s * set, const void * element) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set->compare && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    if (set->length == set->capacity) {
        _irb_set_resize(set, set->capacity + IRB_SET_CHUNK);
    }

    size_t previous = NIL; // initially invalid for the head case when set is empty
    size_t * node = &(set->root); // pointer to later change actual index of the empty child
    while (NIL != (*node)) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = set->compare(element, set->elements + ((*node) * set->size));
        const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

        previous = (*node); // change parent to child
        node = set->node[node_index] + (*node); // change child to proper gradnchild
    }

    (*node) = set->length + 1; // change child index from invalid value to next empty index in array
    set->parent[(*node)] = previous; // make child's parent into parent
    // make child's left and right indexes invalid
    set->node[IRB_SET_LEFT][(*node)] = set->node[IRB_SET_RIGHT][(*node)] = NIL;
    set->color[(*node)] = IRED_SET_COLOR;

    // copy element into structure
    memcpy(set->elements + ((*node) * set->size), element, set->size);
    set->length++;

    _irb_set_insert_fixup(set, (*node));
}

void remove_irb_set(irb_set_s * set, const void * element, void * buffer) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(set->length && "[ERROR] Can't get element from empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(set->compare && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set->elements && "[INVALID] Paremeter can't be NULL.");
    assert(set->parent && "[INVALID] Paremeter can't be NULL.");
    assert(set->node[IRB_SET_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(set->node[IRB_SET_RIGHT] && "[INVALID] Paremeter can't be NULL.");
    assert(NIL != set->root && "[INVALID] Paremeter can't be NIL.");

    size_t node = set->root; // pointer to later change actual index of the empty child
    while (NIL != node) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = set->compare(element, set->elements + (node * set->size));
        if (!comparison) {
            break;
        }

        const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

        // change parent to child and go to next child node
        node = set->node[node_index][node];
    }

    if (NIL == node) {
        // element was NOT found, thus return an error
        assert(false && "[ERROR] Element not found in set.");
        exit(EXIT_FAILURE);
    }

    size_t current = node, child = NIL;
    bool original_color = set->color[current];
    if (NIL == set->node[IRB_SET_LEFT][node]) {
        child = set->node[IRB_SET_RIGHT][node];
        _irb_set_transplant(set, node, set->node[IRB_SET_RIGHT][node]);
    } else if (NIL == set->node[IRB_SET_RIGHT][node]) {
        child = set->node[IRB_SET_LEFT][node];
        _irb_set_transplant(set, node, set->node[IRB_SET_LEFT][node]);
    } else {
        current = _irb_set_minimum((*set), set->node[IRB_SET_RIGHT][node]);
        original_color = set->color[current];
        child = set->node[IRB_SET_RIGHT][current];

        if (set->parent[current] == node) {
            set->parent[child] = current;
        } else {
            _irb_set_transplant(set, current, set->node[IRB_SET_RIGHT][current]);
            set->node[IRB_SET_RIGHT][current] = set->node[IRB_SET_RIGHT][node];
            set->parent[set->node[IRB_SET_RIGHT][current]] = current;
        }
        _irb_set_transplant(set, node, current);
        set->node[IRB_SET_LEFT][current] = set->node[IRB_SET_LEFT][node];
        set->parent[set->node[IRB_SET_LEFT][current]] = current;
        set->color[current] = set->color[node];
    }

    if (IBLACK_SET_COLOR == original_color) {
        _irb_set_remove_fixup(set, child);
    }

    // fix NIL node
    set->color[NIL] = IBLACK_SET_COLOR;
    set->parent[NIL] = set->node[IRB_SET_LEFT][NIL] = set->node[IRB_SET_RIGHT][NIL] = NIL;

    // copy found element into buffer for removal
    memcpy(buffer, set->elements + (node * set->size), set->size);
    set->length--;

    _irb_set_fill_hole(set, node);

    if (set->length == set->capacity - IRB_SET_CHUNK) {
        _irb_set_resize(set, set->length);
    }
}

bool contains_irb_set(const irb_set_s set, const void * element) {
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set.compare && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set.elements && "[INVALID] Paremeter can't be NULL.");
    assert(set.parent && "[INVALID] Paremeter can't be NULL.");
    assert(set.node[IRB_SET_LEFT] && "[INVALID] Paremeter can't be NULL.");
    assert(set.node[IRB_SET_RIGHT] && "[INVALID] Paremeter can't be NULL.");

    for (size_t node = set.root; NIL != node;) {
        // calculate and determine next child node, i.e. if left or right child
        const int comparison = set.compare(element, set.elements + (node * set.size));
        if (!comparison) {
            return true;
        }

        const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
        node = set.node[node_index][node]; // go to next child node
    }

    return false;
}

irb_set_s union_irb_set(const irb_set_s set_one, const irb_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.compare == set_two.compare && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements to maximum's replica
    const irb_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const irb_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;

    // copy maximum set into set union
    irb_set_s set_union = copy_irb_set(maximum, copy);

    // for each element in minimum set
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < minimum.length + 1; ++i) {
        const char * element = minimum.elements + (i * minimum.size);
        bool contains = false;
        for (size_t node = maximum.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = maximum.compare(element, maximum.elements + (node * maximum.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = maximum.node[node_index][node]; // go to next child node
        }

        // if maximum set contains minimum's element then continue
        if (contains) {
            continue;
        } // else add minimum's element to set union

        if (set_union.length == set_union.capacity) {
            _irb_set_resize(&set_union, set_union.capacity + IRB_SET_CHUNK);
        }

        size_t previous = NIL; // initially invalid for the head case when set is empty
        size_t * node = &(set_union.root); // pointer to later change actual index of the empty child
        while (NIL != (*node)) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_union.compare(element, set_union.elements + ((*node) * set_union.size));
            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

            previous = (*node); // change parent to child
            node = set_union.node[node_index] + (*node); // change child to proper gradnchild
        }

        (*node) = set_union.length + 1; // change child index from invalid value to next empty index in array
        set_union.parent[(*node)] = previous; // make child's parent into parent
        // make child's left and right indexes invalid
        set_union.node[IRB_SET_LEFT][(*node)] = set_union.node[IRB_SET_RIGHT][(*node)] = NIL;
        set_union.color[(*node)] = IRED_SET_COLOR;

        copy(set_union.elements + ((*node) * set_union.size), element);
        set_union.length++;

        _irb_set_insert_fixup(&set_union, (*node));
    }

    return set_union;
}

irb_set_s intersect_irb_set(const irb_set_s set_one, const irb_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.compare == set_two.compare && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements
    const irb_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const irb_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;

    irb_set_s set_intersect = create_irb_set(set_one.size, set_one.compare);

    // for each element in minimum set
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < minimum.length + 1; ++i) {
        const char * element = minimum.elements + (i * minimum.size);
        bool contains = false;
        for (size_t node = maximum.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = maximum.compare(element, maximum.elements + (node * maximum.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = maximum.node[node_index][node]; // go to next child node
        }

        // if maximum set contains minimum's element then continue
        if (!contains) {
            continue;
        } // else add minimum's element to set union

        if (set_intersect.length == set_intersect.capacity) {
            _irb_set_resize(&set_intersect, set_intersect.capacity + IRB_SET_CHUNK);
        }

        size_t previous = NIL; // initially invalid for the head case when set is empty
        size_t * node = &(set_intersect.root); // pointer to later change actual index of the empty child
        while (NIL != (*node)) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_intersect.compare(element, set_intersect.elements + ((*node) * set_intersect.size));
            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

            previous = (*node); // change parent to child
            node = set_intersect.node[node_index] + (*node); // change child to proper gradnchild
        }

        (*node) = set_intersect.length + 1; // change child index from invalid value to next empty index in array
        set_intersect.parent[(*node)] = previous; // make child's parent into parent
        // make child's left and right indexes invalid
        set_intersect.node[IRB_SET_LEFT][(*node)] = set_intersect.node[IRB_SET_RIGHT][(*node)] = NIL;
        set_intersect.color[(*node)] = IRED_SET_COLOR;

        copy(set_intersect.elements + ((*node) * set_intersect.size), element);
        set_intersect.length++;

        _irb_set_insert_fixup(&set_intersect, (*node));
    }

    return set_intersect;
}

irb_set_s subtract_irb_set(const irb_set_s minuend, const irb_set_s subtrahend, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(minuend.compare == subtrahend.compare && "[ERROR] Function pointers must be the same.");
    assert(minuend.size == subtrahend.size && "[ERROR] Sizes must be the same.");

    assert(minuend.compare && "[INVALID] Parameter can't be NULL.");
    assert(minuend.size && "[INVALID] Parameter can't be zero.");
    assert(minuend.length <= minuend.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(subtrahend.compare && "[INVALID] Parameter can't be NULL.");
    assert(subtrahend.size && "[INVALID] Parameter can't be zero.");
    assert(subtrahend.length <= subtrahend.capacity && "[INVALID] Lenght can't be larger than capacity.");

    irb_set_s set_subtract = create_irb_set(minuend.size, minuend.compare);

    // for each element in minimum set
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < minuend.length + 1; ++i) {
        const char * element = minuend.elements + (i * minuend.size);
        bool contains = false;
        for (size_t node = subtrahend.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = subtrahend.compare(element, subtrahend.elements + (node * subtrahend.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = subtrahend.node[node_index][node]; // go to next child node
        }

        // if maximum set contains minuend's element then continue
        if (contains) {
            continue;
        } // else add minuend's element to set union

        if (set_subtract.length == set_subtract.capacity) {
            _irb_set_resize(&set_subtract, set_subtract.capacity + IRB_SET_CHUNK);
        }

        size_t previous = NIL; // initially invalid for the head case when set is empty
        size_t * node = &(set_subtract.root); // pointer to later change actual index of the empty child
        while (NIL != (*node)) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_subtract.compare(element, set_subtract.elements + ((*node) * set_subtract.size));
            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

            previous = (*node); // change parent to child
            node = set_subtract.node[node_index] + (*node); // change child to proper gradnchild
        }

        (*node) = set_subtract.length + 1; // change child index from invalid value to next empty index in array
        set_subtract.parent[(*node)] = previous; // make child's parent into parent
        // make child's left and right indexes invalid
        set_subtract.node[IRB_SET_LEFT][(*node)] = set_subtract.node[IRB_SET_RIGHT][(*node)] = NIL;
        set_subtract.color[(*node)] = IRED_SET_COLOR;

        copy(set_subtract.elements + ((*node) * set_subtract.size), element);
        set_subtract.length++;

        _irb_set_insert_fixup(&set_subtract, (*node));
    }

    return set_subtract;
}

irb_set_s exclude_irb_set(const irb_set_s set_one, const irb_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.compare == set_two.compare && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    irb_set_s set_exclude = create_irb_set(set_one.size, set_one.compare);

    // for each element in set one
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < set_one.length + 1; ++i) {
        const char * element = set_one.elements + (i * set_one.size);
        bool contains = false;
        for (size_t node = set_two.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_two.compare(element, set_two.elements + (node * set_two.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = set_two.node[node_index][node]; // go to next child node
        }

        // if maximum set contains minimum's element then continue
        if (contains) {
            continue;
        } // else add minimum's element to set union

        if (set_exclude.length == set_exclude.capacity) {
            _irb_set_resize(&set_exclude, set_exclude.capacity + IRB_SET_CHUNK);
        }

        size_t previous = NIL; // initially invalid for the head case when set is empty
        size_t * node = &(set_exclude.root); // pointer to later change actual index of the empty child
        while (NIL != (*node)) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_exclude.compare(element, set_exclude.elements + ((*node) * set_exclude.size));
            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

            previous = (*node); // change parent to child
            node = set_exclude.node[node_index] + (*node); // change child to proper gradnchild
        }

        (*node) = set_exclude.length + 1; // change child index from invalid value to next empty index in array
        set_exclude.parent[(*node)] = previous; // make child's parent into parent
        // make child's left and right indexes invalid
        set_exclude.node[IRB_SET_LEFT][(*node)] = set_exclude.node[IRB_SET_RIGHT][(*node)] = NIL;
        set_exclude.color[(*node)] = IRED_SET_COLOR;

        copy(set_exclude.elements + ((*node) * set_exclude.size), element);
        set_exclude.length++;

        _irb_set_insert_fixup(&set_exclude, (*node));
    }

    // for each element in set two
    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < set_two.length + 1; ++i) {
        const char * element = set_two.elements + (i * set_two.size);
        bool contains = false;
        for (size_t node = set_one.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_one.compare(element, set_one.elements + (node * set_one.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = set_one.node[node_index][node]; // go to next child node
        }

        // if maximum set contains minimum's element then continue
        if (contains) {
            continue;
        } // else add minimum's element to set union

        if (set_exclude.length == set_exclude.capacity) {
            _irb_set_resize(&set_exclude, set_exclude.capacity + IRB_SET_CHUNK);
        }

        size_t previous = NIL; // initially invalid for the head case when set is empty
        size_t * node = &(set_exclude.root); // pointer to later change actual index of the empty child
        while (NIL != (*node)) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = set_exclude.compare(element, set_exclude.elements + ((*node) * set_exclude.size));
            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;

            previous = (*node); // change parent to child
            node = set_exclude.node[node_index] + (*node); // change child to proper gradnchild
        }

        (*node) = set_exclude.length + 1; // change child index from invalid value to next empty index in array
        set_exclude.parent[(*node)] = previous; // make child's parent into parent
        // make child's left and right indexes invalid
        set_exclude.node[IRB_SET_LEFT][(*node)] = set_exclude.node[IRB_SET_RIGHT][(*node)] = NIL;
        set_exclude.color[(*node)] = IRED_SET_COLOR;

        copy(set_exclude.elements + ((*node) * set_exclude.size), element);
        set_exclude.length++;

        _irb_set_insert_fixup(&set_exclude, (*node));
    }

    return set_exclude;
}

bool is_subset_irb_set(const irb_set_s super, const irb_set_s sub) {
    assert(super.compare == sub.compare && "[ERROR] Function pointers must be the same.");
    assert(super.size == sub.size && "[ERROR] Sizes must be the same.");

    assert(super.compare && "[INVALID] Parameter can't be NULL.");
    assert(super.size && "[INVALID] Parameter can't be zero.");
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(sub.compare && "[INVALID] Parameter can't be NULL.");
    assert(sub.size && "[INVALID] Parameter can't be zero.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < sub.length + 1; ++i) {
        const char * element = sub.elements + (i * sub.size);
        bool contains = false;
        for (size_t node = super.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = super.compare(element, super.elements + (node * super.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = super.node[node_index][node]; // go to next child node
        }

        if (!contains) {
            return false;
        }
    }

    return true;
}

bool is_proper_subset_irb_set(const irb_set_s super, const irb_set_s sub) {
    assert(super.compare == sub.compare && "[ERROR] Function pointers must be the same.");
    assert(super.size == sub.size && "[ERROR] Sizes must be the same.");

    assert(super.compare && "[INVALID] Parameter can't be NULL.");
    assert(super.size && "[INVALID] Parameter can't be zero.");
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(sub.compare && "[INVALID] Parameter can't be NULL.");
    assert(sub.size && "[INVALID] Parameter can't be zero.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < sub.length + 1; ++i) {
        const char * element = sub.elements + (i * sub.size);
        bool contains = false;
        for (size_t node = super.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = super.compare(element, super.elements + (node * super.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = super.node[node_index][node]; // go to next child node
        }

        if (!contains) {
            return false;
        }
    }

    return (sub.length != super.length);
}

bool is_disjoint_irb_set(const irb_set_s set_one, const irb_set_s set_two) {
    assert(set_one.compare == set_two.compare && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.compare && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const irb_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const irb_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;

    // start at 1 since NIL is at zero and elements start beyond NIL
    for (size_t i = 1; i < minimum.length + 1; ++i) {
        const char * element = minimum.elements + (i * minimum.size);
        bool contains = false;
        for (size_t node = maximum.root; NIL != node;) {
            // calculate and determine next child node, i.e. if left or right child
            const int comparison = maximum.compare(element, maximum.elements + (node * maximum.size));
            if (!comparison) {
                contains = true;
                break;
            }

            const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
            node = maximum.node[node_index][node]; // go to next child node
        }

        if (contains) {
            return false;
        }
    }

    return true;
}

void foreach_irb_set(const irb_set_s set, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(set.compare && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < set.length && operate(set.elements + (i * set.size), arguments); ++i) {}
}

void _irb_set_left_rotate(irb_set_s * set, const size_t node) {
    const size_t x = node, y = set->node[IRB_SET_RIGHT][x], z = set->node[IRB_SET_LEFT][y];

    set->node[IRB_SET_RIGHT][x] = z;
    if (NIL != z) {
        set->parent[z] = x;
    }
    set->parent[y] = set->parent[x];

    if (NIL == set->parent[x]) {
        set->root = y;
    } else if (x == set->node[IRB_SET_LEFT][set->parent[x]]) {
        set->node[IRB_SET_LEFT][set->parent[x]] = y;
    } else {
        set->node[IRB_SET_RIGHT][set->parent[x]] = y;
    }

    set->node[IRB_SET_LEFT][y] = x;
    set->parent[x] = y;
}

void _irb_set_right_rotate(irb_set_s * set, const size_t node) {
    const size_t x = node, y = set->node[IRB_SET_LEFT][x], z = set->node[IRB_SET_RIGHT][y];

    set->node[IRB_SET_LEFT][x] = z;
    if (NIL != z) {
        set->parent[z] = x;
    }
    set->parent[y] = set->parent[x];

    if (NIL == set->parent[x]) {
        set->root = y;
    } else if (x == set->node[IRB_SET_LEFT][set->parent[x]]) {
        set->node[IRB_SET_LEFT][set->parent[x]] = y;
    } else {
        set->node[IRB_SET_RIGHT][set->parent[x]] = y;
    }

    set->node[IRB_SET_RIGHT][y] = x;
    set->parent[x] = y;
}

void _irb_set_transplant(irb_set_s * set, const size_t u, const size_t v) {
    if (NIL == set->parent[u]) {
        set->root = v;
    } else if (u == set->node[IRB_SET_LEFT][set->parent[u]]) {
        set->node[IRB_SET_LEFT][set->parent[u]] = v;
    } else {
        set->node[IRB_SET_RIGHT][set->parent[u]] = v;
    }

    set->parent[v] = set->parent[u];
}

size_t _irb_set_minimum(const irb_set_s set, const size_t node) {
    size_t n = node;
    while (NIL != set.node[IRB_SET_LEFT][n]) { // SET MINIMUM
        n = set.node[IRB_SET_LEFT][n];
    }

    return n;
}

void _irb_set_insert_fixup(irb_set_s * set, const size_t node) {
    for (size_t child = node; child != set->root && IRED_SET_COLOR == set->color[set->parent[child]];) {
        if (set->parent[child] == set->node[IRB_SET_LEFT][set->parent[set->parent[child]]]) {
            const size_t uncle = set->node[IRB_SET_RIGHT][set->parent[set->parent[child]]];

            if (NIL != uncle && IRED_SET_COLOR == set->color[uncle]) {
                set->color[set->parent[child]] = set->color[uncle] = IBLACK_SET_COLOR;
                set->color[set->parent[set->parent[child]]] = IRED_SET_COLOR;
                child = set->parent[set->parent[child]];
            } else {
                if (child == set->node[IRB_SET_RIGHT][set->parent[child]]) {
                    child = set->parent[child];
                    _irb_set_left_rotate(set, child);
                }

                set->color[set->parent[child]] = IBLACK_SET_COLOR;
                set->color[set->parent[set->parent[child]]] = IRED_SET_COLOR;
                _irb_set_right_rotate(set, set->parent[set->parent[child]]);
            }
        } else {
            const size_t uncle = set->node[IRB_SET_LEFT][set->parent[set->parent[child]]];

            if (NIL != uncle && IRED_SET_COLOR == set->color[uncle]) {
                set->color[set->parent[child]] = set->color[uncle] = IBLACK_SET_COLOR;
                set->color[set->parent[set->parent[child]]] = IRED_SET_COLOR;
                child = set->parent[set->parent[child]];
            } else {
                if (child == set->node[IRB_SET_LEFT][set->parent[child]]) {
                    child = set->parent[child];
                    _irb_set_right_rotate(set, child);
                }

                set->color[set->parent[child]] = IBLACK_SET_COLOR;
                set->color[set->parent[set->parent[child]]] = IRED_SET_COLOR;
                _irb_set_left_rotate(set, set->parent[set->parent[child]]);
            }
        }
    }

    // fix NIL node
    set->color[NIL] = IBLACK_SET_COLOR;
    set->parent[NIL] = set->node[IRB_SET_LEFT][NIL] = set->node[IRB_SET_RIGHT][NIL] = NIL;

    set->color[set->root] = IBLACK_SET_COLOR;
}

void _irb_set_remove_fixup(irb_set_s * set, const size_t node) {
    size_t child = node;
    while (child != set->root && IBLACK_SET_COLOR == set->color[child]) {
        if (child == set->parent[child]) {
            size_t sibling = set->node[IRB_SET_RIGHT][set->parent[child]];
            if (IRED_SET_COLOR == set->color[sibling]) {
                set->color[sibling] = IBLACK_SET_COLOR;
                set->color[set->parent[child]] = IRED_SET_COLOR;
                _irb_set_left_rotate(set, set->parent[child]);
                sibling = set->node[IRB_SET_RIGHT][set->parent[child]];
            }

            const size_t left_nibling = set->node[IRB_SET_LEFT][sibling];
            const size_t right_nibling = set->node[IRB_SET_RIGHT][sibling];

            if (IBLACK_SET_COLOR == set->color[left_nibling] && IBLACK_SET_COLOR == set->color[right_nibling]) {
                set->color[sibling] = IRED_SET_COLOR;
                child = set->parent[child];
            } else {
                if (IBLACK_SET_COLOR == set->color[set->node[IRB_SET_RIGHT][sibling]]) {
                    set->color[set->node[IRB_SET_LEFT][sibling]] = IBLACK_SET_COLOR;
                    set->color[sibling] = IRED_SET_COLOR;
                    _irb_set_right_rotate(set, sibling);
                    sibling = set->node[IRB_SET_RIGHT][set->parent[child]];
                }

                set->color[sibling] = set->color[set->parent[child]];
                set->color[set->parent[child]] = IBLACK_SET_COLOR;
                set->color[set->node[IRB_SET_RIGHT][sibling]] = IBLACK_SET_COLOR;
                _irb_set_left_rotate(set, set->parent[child]);
                child = set->root;
            }
        } else {
            size_t sibling = set->node[IRB_SET_LEFT][set->parent[child]];
            if (IRED_SET_COLOR == set->color[sibling]) {
                set->color[sibling] = IBLACK_SET_COLOR;
                set->color[set->parent[child]] = IRED_SET_COLOR;
                _irb_set_right_rotate(set, set->parent[child]);
                sibling = set->node[IRB_SET_LEFT][set->parent[child]];
            }

            const size_t left_nibling = set->node[IRB_SET_LEFT][sibling];
            const size_t right_nibling = set->node[IRB_SET_RIGHT][sibling];

            if (IBLACK_SET_COLOR == set->color[left_nibling] && IBLACK_SET_COLOR == set->color[right_nibling]) {
                set->color[sibling] = IRED_SET_COLOR;
                child = set->parent[child];
            } else {
                if (IBLACK_SET_COLOR == set->color[set->node[IRB_SET_LEFT][sibling]]) {
                    set->color[set->node[IRB_SET_LEFT][sibling]] = IBLACK_SET_COLOR;
                    set->color[sibling] = IRED_SET_COLOR;
                    _irb_set_left_rotate(set, sibling);
                    sibling = set->node[IRB_SET_LEFT][set->parent[child]];
                }

                set->color[sibling] = set->color[set->parent[child]];
                set->color[set->parent[child]] = IBLACK_SET_COLOR;
                set->color[set->node[IRB_SET_LEFT][sibling]] = IBLACK_SET_COLOR;
                _irb_set_right_rotate(set, set->parent[child]);
                child = set->root;
            }
        }
    }

    set->color[child] = IBLACK_SET_COLOR;
}

void _irb_set_fill_hole(irb_set_s * set, const size_t hole) {
    const size_t last = set->length + 1;
    if (set->length && set->root == last) { // if head node is last array element then change index to removed one
        set->root = hole;
    }

    // cut hole node from the rest of the set
    set->node[IRB_SET_LEFT][hole] = set->node[IRB_SET_RIGHT][hole] = set->parent[hole] = hole;

    // replace removed element with rightmost array one (or fill hole with valid element)
    memmove(set->elements + (hole * set->size), set->elements + (last * set->size), set->size);
    set->node[IRB_SET_LEFT][hole] = set->node[IRB_SET_LEFT][last];
    set->node[IRB_SET_RIGHT][hole] = set->node[IRB_SET_RIGHT][last];
    set->parent[hole] = set->parent[last];

    // redirect left child of rightmost array node if they don't overlap with removed index
    const size_t left_last = set->node[IRB_SET_LEFT][last];
    if (NIL != left_last) {
        set->parent[left_last] = hole;
    }

    // redirect right child of rightmost array node if they don't overlap with removed index
    const size_t right_last = set->node[IRB_SET_RIGHT][last];
    if (NIL != right_last) {
        set->parent[right_last] = hole;
    }

    // redirect parent of rightmost array node if they don't overlap with removed index
    const size_t parent_last = set->parent[last];
    if (NIL != parent_last) {
        const int comparison = set->compare(set->elements + (last * set->size), set->elements + (parent_last * set->size));
        const size_t node_index = comparison <= 0 ? IRB_SET_LEFT : IRB_SET_RIGHT;
        set->node[node_index][parent_last] = hole;
    }
}

void _irb_set_resize(irb_set_s * set, const size_t size) {
    set->capacity = size;
    const size_t resize = set->capacity + 1;

    set->elements = realloc(set->elements, resize * set->size);
    set->color = realloc(set->color, resize * sizeof(bool));
    set->parent = realloc(set->parent, resize * sizeof(size_t));
    set->node[IRB_SET_LEFT] = realloc(set->node[IRB_SET_LEFT], resize * sizeof(size_t));
    set->node[IRB_SET_RIGHT] = realloc(set->node[IRB_SET_RIGHT], resize * sizeof(size_t));

    assert(set->elements && "[ERROR] Memory allocation failed.");
    assert(set->color && "[ERROR] Memory allocation failed.");
    assert(set->parent && "[ERROR] Memory allocation failed.");
    assert(set->node[IRB_SET_LEFT] && "[ERROR] Memory allocation failed.");
    assert(set->node[IRB_SET_RIGHT] && "[ERROR] Memory allocation failed.");
}
