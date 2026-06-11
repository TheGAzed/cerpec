#include <list/idouble_list.h>

#include <stdlib.h> // import exit()
#include <string.h>
#include <limits.h>

/// @brief Resizes structure to new size.
/// @param list Strcuture to resize.
/// @param size New size to be used.
void _idouble_list_resize(idouble_list_s * const list, size_t const size);

/// @brief Ceils (rounds up) a size to the nearest power of two or chunk.
/// @param size Size to ceil.
/// @return Ceiled size.
size_t _idouble_list_ceil_size(size_t const size);

/// @brief Fills hole/empty node index with last array-based node and fixes/redirects siblings.
/// @param list Strcuture to fill.
/// @param hole Index of hole node.
void _idouble_list_fill_hole(idouble_list_s * const list, size_t const hole);

idouble_list_s create_idouble_list(size_t const size) {
    error(size && "Paremeter can't be zero.");

    return (idouble_list_s) { .size = size, .allocator = &standard, };
}

idouble_list_s make_idouble_list(size_t const size, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (idouble_list_s) { .size = size, .allocator = allocator, };
}

void destroy_idouble_list(idouble_list_s * const list, set_fn const destroy, void * const ad) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size), ad);
        current = list->node[IDL_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arg);
    list->allocator->free(list->node[IDL_NEXT], list->allocator->arg);
    list->allocator->free(list->node[IDL_PREV], list->allocator->arg);

    memset(list, 0, sizeof(idouble_list_s));
}

void clear_idouble_list(idouble_list_s * const list, set_fn const destroy, void * const ad) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size), ad);
        current = list->node[IDL_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arg);
    list->allocator->free(list->node[IDL_NEXT], list->allocator->arg);
    list->allocator->free(list->node[IDL_PREV], list->allocator->arg);

    // make list clear, but still usable
    list->node[IDL_NEXT] = list->node[IDL_PREV] = NULL;
    list->elements = NULL;
    list->capacity = list->head = list->length = 0;
}

idouble_list_s copy_idouble_list(idouble_list_s const * const list, copy_fn const copy, void * const ac) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // allocate and set replica of list
    idouble_list_s const replica = {
        .capacity = list->capacity, .head = list->head, .length = list->length, .size = list->size,
        .node[IDL_NEXT] = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arg),
        .node[IDL_PREV] = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arg),
        .elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arg),
        .allocator = list->allocator,
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");
    error((!replica.capacity || replica.node[IDL_NEXT]) && "Memory allocation failed.");
    error((!replica.capacity || replica.node[IDL_PREV]) && "Memory allocation failed.");

    // copy nodes (elements and indexes) into list
    for (size_t i = 0; i < list->length; ++i) {
        copy(replica.elements + (i * replica.size), list->elements + (i * list->size), ac);
    }
    memcpy(replica.node[IDL_NEXT], list->node[IDL_NEXT], list->length * sizeof(size_t));
    memcpy(replica.node[IDL_PREV], list->node[IDL_PREV], list->length * sizeof(size_t));

    return replica;
}

bool is_empty_idouble_list(idouble_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    return !(list->length);
}

void insert_at_idouble_list(idouble_list_s * const list, void const * const element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(element != list && "Paremeters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // if list is full resize (expand) it
    if (list->length == list->capacity) {
        size_t const capacity = list->length ? list->length * CERPEC_FACTOR : IDOUBLE_LIST_CHUNK;
        _idouble_list_resize(list, capacity);
    }

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    list->node[IDL_NEXT][list->length] = current;
    if (list->length) { // if list is not empty perform complex insertion
        list->node[IDL_PREV][list->length] = list->node[IDL_PREV][current];

        list->node[IDL_NEXT][list->node[IDL_PREV][current]] = list->length;
        list->node[IDL_PREV][current] = list->length;
    } else { // else list is empty and thus needs only a simple redirection to itself
        list->node[IDL_PREV][list->length] = current;
    }

    if (!index) { // if index is zero then list's head must become last empty array node
        list->head = list->length;
    }

    // copy element into list by pushing it as last array node
    memcpy(list->elements + (list->length * list->size), element, list->size);
    list->length++;
}

void get_idouble_list(idouble_list_s const * const list, size_t const index, void * const buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(buffer != list && "Paremeters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // copy retrieved element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_idouble_list(idouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(element != buffer && "Paremeters can't be equal.");
    error(element != list && "Paremeters can't be the same.");
    error(buffer != list && "Paremeters can't be the same.");
    error(list->length && "Structure can't be empty.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each element in list travel forward
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDL_NEXT][current]) {
        char const * const found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[IDL_NEXT][current];
        }

        _idouble_list_fill_hole(list, current);

        // if smaller capacity exists then resize (shrink) list
        if (list->length <= list->capacity / CERPEC_FACTOR && (list->length > IDOUBLE_LIST_CHUNK || !list->length)) {
            _idouble_list_resize(list, list->length);
        }

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_last_idouble_list(idouble_list_s * const list, void const * const element, void * const buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(element != buffer && "Paremeters can't be equal.");
    error(element != list && "Paremeters can't be the same.");
    error(buffer != list && "Paremeters can't be the same.");
    error(list->length && "Structure can't be empty.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each element in list travel backwards
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[IDL_PREV][current];

        char const * found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[IDL_NEXT][current];
        }

        _idouble_list_fill_hole(list, current);

        // if smaller capacity exists then resize (shrink) list
        if (list->length <= list->capacity / CERPEC_FACTOR && (list->length > IDOUBLE_LIST_CHUNK || !list->length)) {
            _idouble_list_resize(list, list->length);
        }

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_at_idouble_list(idouble_list_s * const list, size_t const index, void * const buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(buffer != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // copy removed node element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
    list->length--;

    // if current is head then change head to next node
    if (current == list->head) {
        list->head = list->node[IDL_NEXT][current];
    }

    _idouble_list_fill_hole(list, current);

    // if smaller capacity exists then resize (shrink) list
    if (list->length <= list->capacity / CERPEC_FACTOR && (list->length > IDOUBLE_LIST_CHUNK || !list->length)) {
        _idouble_list_resize(list, list->length);
    }
}

void reverse_idouble_list(idouble_list_s * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    size_t current = list->head;
    for (size_t i = 0; i < list->length; ++i) {
        list->head = current;

        // swap next and previous node indexes
        size_t const next = list->node[IDL_NEXT][current];
        list->node[IDL_NEXT][current] = list->node[IDL_PREV][current];
        list->node[IDL_PREV][current] = next;

        current = list->node[IDL_PREV][current];
    }
}

void shift_next_idouble_list(idouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift && list->length; ++i) {
        list->head = list->node[IDL_NEXT][list->head];
    }
}

void shift_prev_idouble_list(idouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift && list->length; ++i) {
        list->head = list->node[IDL_PREV][list->head];
    }
}

void splice_idouble_list(idouble_list_s * const destination, idouble_list_s * const source, size_t const index) {
    error(destination && "Paremeter can't be NULL.");
    error(source && "Paremeter can't be NULL.");
    error(index <= destination->length && "Paremeter can't be greater than length.");
    error(destination != source && "Paremeters can't be equal.");
    error(source->size == destination->size && "Element sizes must be equal.");

    valid(destination->size && "Size can't be zero.");
    valid(destination->length <= destination->capacity && "Length exceeds capacity.");
    valid(destination->allocator && "Allocator can't be NULL.");

    valid(source->size && "Size can't be zero.");
    valid(source->length <= source->capacity && "Length exceeds capacity.");
    valid(source->allocator && "Allocator can't be NULL.");

    // calculate new capacity of destination list and resize it
    size_t const sum = destination->length + source->length;
    size_t const max = destination->capacity > source->capacity ? destination->capacity : source->capacity;
    size_t const capacity = sum > max ? CERPEC_FACTOR * max : max;
    _idouble_list_resize(destination, capacity);

    // determine closest direction to index and go there
    size_t current = destination->head;
    size_t const closest_index = index <= (destination->length / 2) ? index : destination->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = destination->node[closest_node][current];
    }

    // just copy source's elements and indexes relative to destination's node array (just increment them by length)
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    for (size_t i = 0; i < source->length; ++i) {
        destination->node[IDL_NEXT][destination->length + i] = source->node[IDL_NEXT][i] + destination->length;
        destination->node[IDL_PREV][destination->length + i] = source->node[IDL_PREV][i] + destination->length;
    }

    // redirect destination nodes if it and source had any
    if (destination->length && source->length) {
        size_t const first_destination = current;
        size_t const last_destination  = destination->node[IDL_PREV][current];

        size_t const first_source = source->head + destination->length;
        size_t const last_source  = source->node[IDL_PREV][source->head] + destination->length;

        destination->node[IDL_NEXT][last_destination] = first_source;
        destination->node[IDL_PREV][first_source] = last_destination;

        destination->node[IDL_NEXT][last_source] = first_destination;
        destination->node[IDL_PREV][first_destination] = last_source;
    }

    // if source isn't empty and index points to head node destination's make head to source's
    if (!index && source->length) {
        destination->head = source->head + destination->length;
    }

    destination->length += source->length;

    // clear (NOT DESTROY) source list
    source->allocator->free(source->elements, source->allocator->arg);
    source->allocator->free(source->node[IDL_NEXT], source->allocator->arg);
    source->allocator->free(source->node[IDL_PREV], source->allocator->arg);

    source->node[IDL_NEXT] = source->node[IDL_PREV] = NULL;
    source->elements = NULL;
    source->length = source->capacity = source->head = 0;
}

idouble_list_s slice_idouble_list(idouble_list_s * const list, size_t const index, size_t const length) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(length <= list->length && "Paremeter can't be greater than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    // create split list
    size_t const split_capacity = _idouble_list_ceil_size(length);
    idouble_list_s split = {
        .elements = list->allocator->alloc(split_capacity * list->size, list->allocator->arg),
        .node[IDL_NEXT] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arg),
        .node[IDL_PREV] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arg),

        .size = list->size, .capacity = split_capacity, .allocator = list->allocator,
    };
    error((!split.capacity || split.elements) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDL_NEXT]) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDL_PREV]) && "Memory allocation failed.");

    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[IDL_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[IDL_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        size_t const next = list->node[IDL_NEXT][current];
        _idouble_list_fill_hole(list, current);

        split_current = split.node[IDL_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // shrink resize original list if necessary
    size_t const list_capacity = _idouble_list_ceil_size(list->length);
    if (list->capacity != list_capacity) {
        _idouble_list_resize(list, list_capacity);
    }

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index || (index > list->length)) {
        list->head = current;
    }

    return split;
}

idouble_list_s split_idouble_list(idouble_list_s * const list, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const closest_index = index <= (list->length / 2) ? index : list->length - index;
    bool const closest_node = closest_index == index ? IDL_NEXT : IDL_PREV;
    for (size_t i = 0; i < closest_index; ++i) {
        current = list->node[closest_node][current];
    }

    size_t const length = list->length - index;
    // create split list
    size_t const split_capacity = _idouble_list_ceil_size(length);
    idouble_list_s split = {
        .elements = list->allocator->alloc(split_capacity * list->size, list->allocator->arg),
        .node[IDL_NEXT] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arg),
        .node[IDL_PREV] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arg),

        .size = list->size, .capacity = split_capacity, .allocator = list->allocator,
    };
    error((!split.capacity || split.elements) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDL_NEXT]) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDL_PREV]) && "Memory allocation failed.");

    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[IDL_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[IDL_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        size_t const next = list->node[IDL_NEXT][current];
        _idouble_list_fill_hole(list, current);

        split_current = split.node[IDL_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // shrink resize original list if necessary
    size_t const list_capacity = _idouble_list_ceil_size(list->length);
    if (list->capacity != list_capacity) {
        _idouble_list_resize(list, list_capacity);
    }

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index) {
        list->head = current;
    }

    return split;
}

idouble_list_s extract_idouble_list(idouble_list_s * const list, filter_fn const filter) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // only create positive to save true filtered values
    idouble_list_s positive = { .size = list->size, .allocator = list->allocator, };

    size_t * pos = &(positive.head);
    size_t const length = list->length; // list length may change
    for (size_t i = 0, current = list->head; i < length; ++i) {
        char const * element = list->elements + (current * list->size);

        if (!filter(element)) { // if no extraction go to next list node and continue
            current = list->node[IDL_NEXT][current];
            continue;
        } // else extract and append list node into positive list

        (*pos) = positive.length; // set head and next nodes to next index
        if (positive.length == positive.capacity) { // expand capacity if needed
            size_t const capacity = positive.length ? positive.length * CERPEC_FACTOR : IDOUBLE_LIST_CHUNK;
            _idouble_list_resize(&positive, capacity);
        }
        positive.node[IDL_PREV][positive.length] = positive.length - 1; // set previous node indexes to one minus current
        positive.node[IDL_PREV][0] = positive.length; // set first node's prev to positive length

        // remove element from main list and add it to positive one
        memcpy(positive.elements + (positive.length * positive.size), element, list->size);
        positive.length++;
        list->length--;

        size_t const next = list->node[IDL_NEXT][current];
        if (list->head == current) {
            list->head = next;
        }

        _idouble_list_fill_hole(list, current);

        pos = positive.node[IDL_NEXT] + (positive.length - 1);
        current = (next == list->length) ? current : next; // next may point to last node in array, which gets swapped
    }
    (*pos) = 0;

    // shrink list if smaller chunk is available
    size_t const list_capacity = _idouble_list_ceil_size(list->length);
    if (list->capacity != list_capacity) {
        _idouble_list_resize(list, list_capacity);
    }

    return positive;
}

void each_next_idouble_list(idouble_list_s const * const list, handle_fn const handle, void * const ah) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(ah != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each forward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDL_NEXT][current]) {
        if (!handle(list->elements + (current * list->size), ah)) {
            break;
        }
    }
}

void each_prev_idouble_list(idouble_list_s const * const list, handle_fn const handle, void * const ah) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(ah != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each backward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[IDL_PREV][current];
        if (!handle(list->elements + (current * list->size), ah)) {
            break;
        }
    }
}

void apply_idouble_list(idouble_list_s const * const list, process_fn const process, void * const ap) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(ap != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arg);
    error((!list->length || elements_array) && "Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDL_NEXT][current]) {
        memcpy(elements_array + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(elements_array, list->length, ap);

    // copy elements back into list
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDL_NEXT][current]) {
        memcpy(list->elements + (current * list->size), elements_array + (i * list->size), list->size);
    }

    list->allocator->free(elements_array, list->allocator->arg);
}

void _idouble_list_resize(idouble_list_s * const list, size_t const size) {
    list->capacity = size;

    list->elements = list->allocator->realloc(list->elements, list->capacity * list->size, list->allocator->arg);
    error((!list->capacity || list->elements) && "Memory allocation failed.");

    list->node[IDL_NEXT] = list->allocator->realloc(list->node[IDL_NEXT], list->capacity * sizeof(size_t), list->allocator->arg);
    error((!list->capacity || list->node[IDL_NEXT]) && "Memory allocation failed.");

    list->node[IDL_PREV] = list->allocator->realloc(list->node[IDL_PREV], list->capacity * sizeof(size_t), list->allocator->arg);
    error((!list->capacity || list->node[IDL_PREV]) && "Memory allocation failed.");
}

void _idouble_list_fill_hole(idouble_list_s * list, const size_t hole) {
    if (list->head == list->length) { list->head = hole; }

    // cut current removed node's siblings from itself
    list->node[IDL_NEXT][list->node[IDL_PREV][hole]] = list->node[IDL_NEXT][hole];
    list->node[IDL_PREV][list->node[IDL_NEXT][hole]] = list->node[IDL_PREV][hole];

    // cut current removed node from its siblings
    list->node[IDL_PREV][hole] = list->node[IDL_NEXT][hole] = hole;

    // replace element at current index with popped last element like in a stack
    memmove(list->elements + (hole * list->size), list->elements + (list->length * list->size), list->size);
    list->node[IDL_NEXT][hole] = list->node[IDL_NEXT][list->length];
    list->node[IDL_PREV][hole] = list->node[IDL_PREV][list->length];

    // redirect array's last swapped node's siblings to hole
    list->node[IDL_PREV][list->node[IDL_NEXT][list->length]] = hole;
    list->node[IDL_NEXT][list->node[IDL_PREV][list->length]] = hole;
}

size_t _idouble_list_ceil_size(size_t const size) {
    size_t s = size > IDOUBLE_LIST_CHUNK ? size : size ? IDOUBLE_LIST_CHUNK : 0;

    if (!(s & (s - 1))) { return s; }

    for (size_t i = 1; i < sizeof(size_t) * CHAR_BIT; i *= 2) { s |= (s >> i); }

    return (s - (s / 2)) << 1;
}
