#include <list/fdouble_list.h>

#include <stdlib.h> // import exit()
#include <string.h>

/// @brief Fills hole/empty node index with last array-based node and fixes/redirects siblings.
/// @param list Strcuture to fill.
/// @param hole Index of hole node.
void _fdouble_list_fill_hole(fdouble_list_s * const list, size_t const hole);

fdouble_list_s create_fdouble_list(size_t const size, size_t const max) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");

    fdouble_list_s const list = {
        .max = max, .size = size, .allocator = &standard,
        .elements = standard.alloc(max * size, standard.arg),
        .node[FDL_PREV] = standard.alloc(max * sizeof(size_t), standard.arg),
        .node[FDL_NEXT] = standard.alloc(max * sizeof(size_t), standard.arg),
    };
    error(list.elements && "Memory allocation failed.");
    error(list.node[FDL_PREV] && "Memory allocation failed.");
    error(list.node[FDL_NEXT] && "Memory allocation failed.");

    return list;
}

fdouble_list_s make_fdouble_list(size_t const size, size_t const max, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    fdouble_list_s const list = {
        .max = max, .size = size, .allocator = allocator,
        .elements = allocator->alloc(max * size, allocator->arg),
        .node[FDL_PREV] = allocator->alloc(max * sizeof(size_t), allocator->arg),
        .node[FDL_NEXT] = allocator->alloc(max * sizeof(size_t), allocator->arg),
    };
    error(list.elements && "Memory allocation failed.");
    error(list.node[FDL_PREV] && "Memory allocation failed.");
    error(list.node[FDL_NEXT] && "Memory allocation failed.");

    return list;
}

void destroy_fdouble_list(fdouble_list_s * const list, set_fn const destroy, void * const ad) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size), ad);
        current = list->node[FDL_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arg);
    list->allocator->free(list->node[FDL_NEXT], list->allocator->arg);
    list->allocator->free(list->node[FDL_PREV], list->allocator->arg);

    memset(list, 0, sizeof(fdouble_list_s));
}

void clear_fdouble_list(fdouble_list_s * const list, set_fn const destroy, void * const ad) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // call destroy function for each element in list
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        destroy(list->elements + (current * list->size), ad);
        current = list->node[FDL_NEXT][current];
    }

    // make list clear, but still usable
    list->head = list->length = 0;
}

fdouble_list_s copy_fdouble_list(fdouble_list_s const * const list, copy_fn const copy, void * const ac) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // allocate and set replica of list
    fdouble_list_s const replica = {
        .max = list->max, .head = list->head, .length = list->length, .size = list->size,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arg),
        .node[FDL_NEXT] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arg),
        .node[FDL_PREV] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arg),
        .allocator = list->allocator,
    };
    error(replica.elements && "Memory allocation failed.");
    error(replica.node[FDL_NEXT] && "Memory allocation failed.");
    error(replica.node[FDL_PREV] && "Memory allocation failed.");

    // copy nodes (elements and indexes) into list
    for (size_t i = 0; i < list->length; ++i) {
        copy(replica.elements + (i * replica.size), list->elements + (i * list->size), ac);
    }
    memcpy(replica.node[FDL_NEXT], list->node[FDL_NEXT], list->length * sizeof(size_t));
    memcpy(replica.node[FDL_PREV], list->node[FDL_PREV], list->length * sizeof(size_t));

    return replica;
}

bool is_empty_fdouble_list(fdouble_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    return !(list->length);
}

bool is_full_fdouble_list(fdouble_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    return (list->length == list->max);
}

void insert_at_fdouble_list(fdouble_list_s * const list, void const * const element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(element != list && "Paremeters can't be the same.");
    error(list != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    list->node[FDL_NEXT][list->length] = current;
    if (list->length) { // if list is not empty perform complex insertion
        list->node[FDL_PREV][list->length] = list->node[FDL_PREV][current];

        list->node[FDL_NEXT][list->node[FDL_PREV][current]] = list->length;
        list->node[FDL_PREV][current] = list->length;
    } else { // else list is empty and thus needs only a simple redirection to itself
        list->node[FDL_PREV][list->length] = current;
    }

    if (!index) { // if index is zero then list's head must become last empty array node
        list->head = list->length;
    }

    // copy element into list by pushing it as last array node
    memcpy(list->elements + (list->length * list->size), element, list->size);
    list->length++;
}

void get_fdouble_list(fdouble_list_s const * const list, size_t const index, void * const buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // copy retrieved element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_fdouble_list(fdouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(element != buffer && "Paremeters can't be the same.");
    error(list->elements && "Array can't be NULL.");
    error(list->node[FDL_NEXT] && "Array can't be NULL.");
    error(list->node[FDL_PREV] && "Array can't be NULL.");
    error(list != element && "Parameters can't be equal.");
    error(list != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // for each element in list travel forward
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        char const * const found = list->elements + (current * list->size);
        // if element isn't found continue
        if (0 != compare(element, found)) {
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[FDL_NEXT][current];
        }

        _fdouble_list_fill_hole(list, current);

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_last_fdouble_list(fdouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(element != buffer && "Paremeters can't be the same.");
    error(list->elements && "Array can't be NULL.");
    error(list->node[FDL_NEXT] && "Array can't be NULL.");
    error(list->node[FDL_PREV] && "Array can't be NULL.");
    error(list != element && "Parameters can't be equal.");
    error(list != buffer && "Parameters can't be equal.");
    error(buffer != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // for each element in list travel backwards
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[FDL_PREV][current];

        char const * found = list->elements + (current * list->size);
        // if element isn't found continue
        if (0 != compare(element, found)) {
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[FDL_NEXT][current];
        }

        _fdouble_list_fill_hole(list, current);

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_at_fdouble_list(fdouble_list_s * const list, size_t const index, void * const buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // copy removed node element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
    list->length--;

    // if current is head then change head to next node
    if (current == list->head) {
        list->head = list->node[FDL_NEXT][current];
    }

    _fdouble_list_fill_hole(list, current);
}

void reverse_fdouble_list(fdouble_list_s * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    size_t current = list->head;
    for (size_t i = 0; i < list->length; ++i) {
        list->head = current;

        // swap next and previous node indexes
        size_t const next = list->node[FDL_NEXT][current];
        list->node[FDL_NEXT][current] = list->node[FDL_PREV][current];
        list->node[FDL_PREV][current] = next;

        current = list->node[FDL_PREV][current];
    }
}

void shift_next_fdouble_list(fdouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift && list->length; ++i) {
        list->head = list->node[FDL_NEXT][list->head];
    }
}

void shift_prev_fdouble_list(fdouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift && list->length; ++i) {
        list->head = list->node[FDL_PREV][list->head];
    }
}

void splice_fdouble_list(fdouble_list_s * const destination, fdouble_list_s * const source, size_t const index) {
    error(destination && "Paremeter can't be NULL.");
    error(source && "Paremeter can't be NULL.");
    error(index <= destination->length && "Paremeter can't be greater than length.");
    error(destination != source && "Paremeters can't be equal.");
    error(destination->length + source->length <= destination->max && "Spliced length exceeds maximum.");
    error(source->size == destination->size && "Element sizes must be equal.");

    valid(destination->size && "Size can't be zero.");
    valid(destination->length <= destination->max && "Length exceeds maximum.");
    valid(destination->allocator && "Allocator can't be NULL.");
    valid(destination->max && "Maximum size can't be zero.");
    valid(destination->elements && "Elements array can't be NULL.");
    valid(destination->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(destination->node[FDL_PREV] && "Previous array can't be NULL.");

    valid(source->size && "Size can't be zero.");
    valid(source->length <= source->max && "Length exceeds maximum.");
    valid(source->allocator && "Allocator can't be NULL.");
    valid(source->max && "Maximum size can't be zero.");
    valid(source->elements && "Elements array can't be NULL.");
    valid(source->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(source->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = destination->head;
    size_t const closest_index = index <= (destination->length / 2) ? index : destination->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = destination->node[closest_node][current];
    }

    // just copy source's elements and indexes relative to destination's node array (just increment them by length)
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    for (size_t i = 0; i < source->length; ++i) {
        destination->node[FDL_NEXT][destination->length + i] = source->node[FDL_NEXT][i] + destination->length;
        destination->node[FDL_PREV][destination->length + i] = source->node[FDL_PREV][i] + destination->length;
    }

    // redirect destination nodes if it and source had any
    if (destination->length && source->length) {
        size_t const first_destination = current;
        size_t const last_destination  = destination->node[FDL_PREV][current];

        size_t const first_source = source->head + destination->length;
        size_t const last_source  = source->node[FDL_PREV][source->head] + destination->length;

        destination->node[FDL_NEXT][last_destination] = first_source;
        destination->node[FDL_PREV][first_source] = last_destination;

        destination->node[FDL_NEXT][last_source] = first_destination;
        destination->node[FDL_PREV][first_destination] = last_source;
    }

    // if source isn't empty and index points to head node destination's make head to source's
    if (!index && source->length) {
        destination->head = source->head + destination->length;
    }

    destination->length += source->length;

    // clear (NOT DESTROY) source list
    source->length = source->head = 0;
}

fdouble_list_s slice_fdouble_list(fdouble_list_s * const list, size_t const index, size_t const length, size_t const list_max, size_t const slice_max) {
    error(list && "Paremeter can't be NULL.");
    error(list_max && "Paremeter can't be zero.");
    error(slice_max && "Paremeter can't be zero.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(length <= list->length && "Paremeter can't be greater than length.");
    error(list->length - length <= list_max && "Maximum size can't be more than length.");
    error(length <= slice_max && "Maximum size can't be more than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // create split list
    fdouble_list_s split = {
        .size = list->size, .max = slice_max, .allocator = list->allocator,

        .elements = list->allocator->alloc(slice_max * list->size, list->allocator->arg),
        .node[FDL_NEXT] = list->allocator->alloc(slice_max * sizeof(size_t), list->allocator->arg),
        .node[FDL_PREV] = list->allocator->alloc(slice_max * sizeof(size_t), list->allocator->arg),
    };
    error(split.elements && "Memory allocation failed.");
    error(split.node[FDL_NEXT] && "Memory allocation failed.");
    error(split.node[FDL_PREV] && "Memory allocation failed.");

    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[FDL_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[FDL_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        size_t const next = list->node[FDL_NEXT][current];
        _fdouble_list_fill_hole(list, current);

        split_current = split.node[FDL_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index || (index > list->length)) {
        list->head = current;
    }

    list->max = list_max;
    list->elements = list->allocator->realloc(list->elements, list_max * list->size, list->allocator->arg);
    list->node[FDL_NEXT] = list->allocator->realloc(list->node[FDL_NEXT], list_max * sizeof(size_t), list->allocator->arg);
    list->node[FDL_PREV] = list->allocator->realloc(list->node[FDL_PREV], list_max * sizeof(size_t), list->allocator->arg);

    return split;
}

fdouble_list_s split_fdouble_list(fdouble_list_s * const list, size_t const index, size_t const list_max, size_t const split_max) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(index <= list_max && "Maximum size can't be more than length.");
    error(list->length - index <= split_max && "Maximum size can't be more than length.");
    error(list_max && "Paremeter can't be zero.");
    error(split_max && "Paremeter can't be zero.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // create split list
    fdouble_list_s split = {
        .size = list->size, .max = split_max, .allocator = list->allocator,

        .elements = list->allocator->alloc(split_max * list->size, list->allocator->arg),
        .node[FDL_NEXT] = list->allocator->alloc(split_max * sizeof(size_t), list->allocator->arg),
        .node[FDL_PREV] = list->allocator->alloc(split_max * sizeof(size_t), list->allocator->arg),
    };
    error(split.elements && "Memory allocation failed.");
    error(split.node[FDL_NEXT] && "Memory allocation failed.");
    error(split.node[FDL_PREV] && "Memory allocation failed.");

    size_t const length = list->length - index;
    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[FDL_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[FDL_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        size_t const next = list->node[FDL_NEXT][current];
        _fdouble_list_fill_hole(list, current);

        split_current = split.node[FDL_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index) {
        list->head = current;
    }

    list->max = list_max;
    list->elements = list->allocator->realloc(list->elements, list_max * list->size, list->allocator->arg);
    list->node[FDL_NEXT] = list->allocator->realloc(list->node[FDL_NEXT], list_max * sizeof(size_t), list->allocator->arg);
    list->node[FDL_PREV] = list->allocator->realloc(list->node[FDL_PREV], list_max * sizeof(size_t), list->allocator->arg);

    return split;
}

fdouble_list_s extract_fdouble_list(fdouble_list_s * const list, filter_fn const filter, size_t const list_max, size_t const extract_max) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");
    error(list_max && "Paremeter can't be zero.");
    error(extract_max && "Paremeter can't be zero.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // only create positive to save true filtered values
    fdouble_list_s positive = {
        .size = list->size, .allocator = list->allocator, .max = extract_max,
        .elements = list->allocator->alloc(list->size * extract_max, list->allocator->arg),
        .node[FDL_PREV] = list->allocator->alloc(sizeof(size_t) * extract_max, list->allocator->arg),
        .node[FDL_NEXT] = list->allocator->alloc(sizeof(size_t) * extract_max, list->allocator->arg),
    };
    error(positive.elements && "Memory allocation failed.");
    error(positive.node[FDL_NEXT] && "Memory allocation failed.");
    error(positive.node[FDL_PREV] && "Memory allocation failed.");

    size_t * pos = &(positive.head);
    size_t const length = list->length; // list length may change
    for (size_t i = 0, current = list->head; i < length; ++i) {
        char const * element = list->elements + (current * list->size);

        if (!filter(element)) { // if no extraction go to next list node and continue
            current = list->node[FDL_NEXT][current];
            continue;
        } // else extract and append list node into positive list

        error(positive.length + 1 <= extract_max && "Maximum size can't be more than length.");

        (*pos) = positive.length; // set head and next nodes to next index
        positive.node[FDL_PREV][positive.length] = positive.length - 1; // set previous node indexes to one minus current
        positive.node[FDL_PREV][0] = positive.length; // set first node's prev to positive length

        // remove element from main list and add it to positive one
        memcpy(positive.elements + (positive.length * positive.size), element, list->size);
        positive.length++;
        list->length--;

        size_t const next = list->node[FDL_NEXT][current];
        if (list->head == current) {
            list->head = next;
        }

        _fdouble_list_fill_hole(list, current);

        pos = positive.node[FDL_NEXT] + (positive.length - 1);
        current = (next == list->length) ? current : next; // nex may point to last node in array, which gets swapped
    }
    (*pos) = 0;

    error(list->length <= list_max && "Maximum size can't be more than length.");
    list->max = list_max;
    list->elements = list->allocator->realloc(list->elements, list_max * list->size, list->allocator->arg);
    list->node[FDL_NEXT] = list->allocator->realloc(list->node[FDL_NEXT], list_max * sizeof(size_t), list->allocator->arg);
    list->node[FDL_PREV] = list->allocator->realloc(list->node[FDL_PREV], list_max * sizeof(size_t), list->allocator->arg);

    return positive;
}

void each_next_fdouble_list(fdouble_list_s const * const list, handle_fn const handle, void * const ah) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(list != ah && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // for each forward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        if (!handle(list->elements + (current * list->size), ah)) {
            break;
        }
    }
}

void each_prev_fdouble_list(fdouble_list_s const * const list, handle_fn const handle, void * const ah) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(list != ah && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // for each backward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[FDL_PREV][current];
        if (!handle(list->elements + (current * list->size), ah)) {
            break;
        }
    }
}

void apply_fdouble_list(fdouble_list_s const * const list, process_fn const process, void * const ap) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(list != ap && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->max && "Length exceeds maximum.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->max && "Maximum size can't be zero.");
    valid(list->elements && "Elements array can't be NULL.");
    valid(list->node[FDL_NEXT] && "Next array can't be NULL.");
    valid(list->node[FDL_PREV] && "Previous array can't be NULL.");

    // create temporary continuous array for list element
    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arg);
    error((!list->length || elements_array) && "Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        memcpy(elements_array + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(elements_array, list->length, ap);

    // copy elements back into list
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        memcpy(list->elements + (current * list->size), elements_array + (i * list->size), list->size);
    }

    list->allocator->free(elements_array, list->allocator->arg);
}

void _fdouble_list_fill_hole(fdouble_list_s * list, const size_t hole) {
    if (list->head == list->length) { list->head = hole; }

    // cut current removed node's siblings from itself
    list->node[FDL_NEXT][list->node[FDL_PREV][hole]] = list->node[FDL_NEXT][hole];
    list->node[FDL_PREV][list->node[FDL_NEXT][hole]] = list->node[FDL_PREV][hole];

    // cut current removed node from its siblings
    list->node[FDL_PREV][hole] = list->node[FDL_NEXT][hole] = hole;

    // replace element at current index with popped last element like in a stack
    memmove(list->elements + (hole * list->size), list->elements + (list->length * list->size), list->size);
    list->node[FDL_NEXT][hole] = list->node[FDL_NEXT][list->length];
    list->node[FDL_PREV][hole] = list->node[FDL_PREV][list->length];

    // redirect array's last swapped node's siblings to hole
    list->node[FDL_PREV][list->node[FDL_NEXT][list->length]] = hole;
    list->node[FDL_NEXT][list->node[FDL_PREV][list->length]] = hole;
}
