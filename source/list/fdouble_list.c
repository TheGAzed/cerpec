#include <list/fdouble_list.h>

#include <assert.h>
#include <stdlib.h> // import exit()
#include <string.h>

/// @brief Fills hole/empty node index with last array-based node and fixes/redirects siblings.
/// @param list Strcuture to fill.
/// @param hole Index of hole node.
void _fdouble_list_fill_hole(fdouble_list_s * const list, size_t const hole);

fdouble_list_s create_fdouble_list(size_t const size, size_t const max) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");

    fdouble_list_s const list = {
        .max = max, .size = size, .allocator = &standard,
        .elements = standard.alloc(max * size, standard.arguments),
        .node[FDL_PREV] = standard.alloc(max * sizeof(size_t), standard.arguments),
        .node[FDL_NEXT] = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    assert(list.elements && "[ERROR] Memory allocation failed.");
    assert(list.node[FDL_PREV] && "[ERROR] Memory allocation failed.");
    assert(list.node[FDL_NEXT] && "[ERROR] Memory allocation failed.");

    return list;
}

fdouble_list_s make_fdouble_list(size_t const size, size_t const max, memory_s const * const allocator) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");
    assert(allocator && "[ERROR] Paremeter can't be NULL.");

    fdouble_list_s const list = {
        .max = max, .size = size, .allocator = allocator,
        .elements = allocator->alloc(max * size, allocator->arguments),
        .node[FDL_PREV] = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .node[FDL_NEXT] = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    assert(list.elements && "[ERROR] Memory allocation failed.");
    assert(list.node[FDL_PREV] && "[ERROR] Memory allocation failed.");
    assert(list.node[FDL_NEXT] && "[ERROR] Memory allocation failed.");

    return list;
}

void destroy_fdouble_list(fdouble_list_s * const list, set_fn const destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size));
        current = list->node[FDL_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->node[FDL_NEXT], list->allocator->arguments);
    list->allocator->free(list->node[FDL_PREV], list->allocator->arguments);

    list->max = list->head = list->length = list->size = 0;
    list->node[FDL_PREV] = list->node[FDL_NEXT] = NULL;
    list->elements = NULL;

    list->allocator = NULL;
}

void clear_fdouble_list(fdouble_list_s * const list, set_fn const destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size));
        current = list->node[FDL_NEXT][current];
    }

    // make list clear, but still usable
    list->head = list->length = 0;
}

fdouble_list_s copy_fdouble_list(fdouble_list_s const * const list, copy_fn const copy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // allocate and set replica of list
    fdouble_list_s const replica = {
        .max = list->max, .head = list->head, .length = list->length, .size = list->size,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .node[FDL_NEXT] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .node[FDL_PREV] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");
    assert(replica.node[FDL_NEXT] && "[ERROR] Memory allocation failed.");
    assert(replica.node[FDL_PREV] && "[ERROR] Memory allocation failed.");

    // copy nodes (elements and indexes) into list
    for (size_t i = 0; i < list->length; ++i) {
        copy(replica.elements + (i * replica.size), list->elements + (i * list->size));
    }
    memcpy(replica.node[FDL_NEXT], list->node[FDL_NEXT], list->length * sizeof(size_t));
    memcpy(replica.node[FDL_PREV], list->node[FDL_PREV], list->length * sizeof(size_t));

    return replica;
}

bool is_empty_fdouble_list(fdouble_list_s const * const list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    return !(list->length);
}

bool is_full_fdouble_list(fdouble_list_s const * const list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    return (list->length == list->max);
}

void insert_at_fdouble_list(fdouble_list_s * const list, void const * const element, size_t const index) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(index <= list->length && "[ERROR] Paremeter can't be greater than length.");
    assert(element != list && "[ERROR] Paremeters can't be the same.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
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
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // copy retrieved element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_fdouble_list(fdouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(element != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list->elements && "[ERROR] Array can't be NULL.");
    assert(list->node[FDL_NEXT] && "[ERROR] Array can't be NULL.");
    assert(list->node[FDL_PREV] && "[ERROR] Array can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // for each element in list travel forward
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        char const * const found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
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
    assert(false && "[ERROR] Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_last_fdouble_list(fdouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(element != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list->elements && "[ERROR] Array can't be NULL.");
    assert(list->node[FDL_NEXT] && "[ERROR] Array can't be NULL.");
    assert(list->node[FDL_PREV] && "[ERROR] Array can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // for each element in list travel backwards
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[FDL_PREV][current];

        char const * found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
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
    assert(false && "[ERROR] Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_at_fdouble_list(fdouble_list_s * const list, size_t const index, void * const buffer) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
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
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

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
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't shift empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[FDL_NEXT][list->head];
    }
}

void shift_prev_fdouble_list(fdouble_list_s * const list, size_t const shift) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't shift empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[FDL_PREV][list->head];
    }
}

void splice_fdouble_list(fdouble_list_s * const destination, fdouble_list_s * const source, size_t const index) {
    assert(destination && "[ERROR] Paremeter can't be NULL.");
    assert(source && "[ERROR] Paremeter can't be NULL.");
    assert(index <= destination->length && "[ERROR] Paremeter can't be greater than length.");
    assert(destination != source && "[ERROR] Paremeters can't be the same.");
    assert(destination->length + source->length <= destination->max && "[ERROR] Spliced length exceeds maximum.");

    assert(destination->size && "[INVALID] Size can't be zero.");
    assert(destination->length <= destination->max && "[INVALID] Length exceeds maximum.");
    assert(destination->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(destination->max && "[INVALID] Parameter can't be zero.");

    assert(source->size && "[INVALID] Size can't be zero.");
    assert(source->length <= source->max && "[INVALID] Length exceeds maximum.");
    assert(source->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(source->max && "[INVALID] Parameter can't be zero.");

    assert(source->size == destination->size && "[INVALID] Element sizes must be equal.");

    // determine closest direction to index and go there
    size_t current = destination->head;
    size_t const real_index = index <= (destination->length >> 1) ? index : destination->length - index;
    bool const node_index = real_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = destination->node[node_index][current];
    }

    // just copy source's elements and indexes relative to destination's node array (just increment them by length)
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    for (size_t i = 0; i < source->length; ++i) {
        destination->node[FDL_NEXT][destination->length + i] = source->node[FDL_NEXT][i] + destination->length;
        destination->node[FDL_PREV][destination->length + i] = source->node[FDL_PREV][i] + destination->length;
    }

    // redirect destination nodes if it and source had any
    if (destination->length && source->length) {
        const size_t first_destination = current;
        const size_t last_destination  = destination->node[FDL_PREV][current];

        const size_t first_source = source->head + destination->length;
        const size_t last_source  = source->node[FDL_PREV][source->head] + destination->length;

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

fdouble_list_s split_fdouble_list(fdouble_list_s * const list, size_t const index, size_t const length) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");
    assert(length <= list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? FDL_NEXT : FDL_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // create split list
    fdouble_list_s split = {
        .head = 0, .size = list->size, .length = 0, .max = list->max, .allocator = list->allocator,

        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .node[FDL_NEXT] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .node[FDL_PREV] = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
    };
    assert(split.elements && "[ERROR] Memory allocation failed.");
    assert(split.node[FDL_NEXT] && "[ERROR] Memory allocation failed.");
    assert(split.node[FDL_PREV] && "[ERROR] Memory allocation failed.");

    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[FDL_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[FDL_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        const size_t next = list->node[FDL_NEXT][current];
        if (list->head == current) {
            list->head = next;
        }

        _fdouble_list_fill_hole(list, current);

        split_current = split.node[FDL_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index || (index >= list->length)) {
        list->head = current;
    }

    return split;
}

fdouble_list_s extract_fdouble_list(fdouble_list_s * const list, filter_fn const filter, void * const arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(filter && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // only create positive to save true filtered values
    fdouble_list_s positive = {
        .size = list->size, .allocator = list->allocator, .max = list->max,
        .elements = list->allocator->alloc(list->size * list->max, list->allocator->arguments),
        .node[FDL_PREV] = list->allocator->alloc(sizeof(size_t) * list->max, list->allocator->arguments),
        .node[FDL_NEXT] = list->allocator->alloc(sizeof(size_t) * list->max, list->allocator->arguments),
    };
    assert(positive.elements && "[ERROR] Memory allocation failed.");
    assert(positive.node[FDL_NEXT] && "[ERROR] Memory allocation failed.");
    assert(positive.node[FDL_PREV] && "[ERROR] Memory allocation failed.");

    size_t * pos = &(positive.head);
    size_t const length = list->length; // list length may change
    for (size_t i = 0, current = list->head; i < length; ++i) {
        char const * element = list->elements + (current * list->size);

        if (!filter(element, arguments)) { // if no extraction go to next list node and continue
            current = list->node[FDL_NEXT][current];
            continue;
        } // else extract and append list node into positive list

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

    return positive;
}

void map_next_fdouble_list(fdouble_list_s const * const list, handle_fn const operate, void * const arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // for each forward element in list call operate function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        if (!operate(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void map_prev_fdouble_list(fdouble_list_s const * const list, handle_fn const handle, void * const arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(handle && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    // for each backward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[FDL_PREV][current];
        if (!handle(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void apply_fdouble_list(fdouble_list_s const * const list, process_fn const process, void * const arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(process && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->max && "[INVALID] Length exceeds maximum.");
    assert(list->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(list->max && "[INVALID] Parameter can't be zero.");

    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arguments);
    assert((!list->length || elements_array) && "[ERROR] Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        memcpy(elements_array + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(elements_array, list->length, arguments);

    // copy elements back into list
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[FDL_NEXT][current]) {
        memcpy(list->elements + (current * list->size), elements_array + (i * list->size), list->size);
    }

    list->allocator->free(elements_array, list->allocator->arguments);
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
