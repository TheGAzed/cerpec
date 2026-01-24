#include <list/idouble_list.h>

#include <stdlib.h> // import exit()
#include <string.h>

/// @brief Resizes structure to new size.
/// @param list Strcuture to resize.
/// @param size New size to be used.
void _idouble_list_resize(idouble_list_s * const list, size_t const size);

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

void destroy_idouble_list(idouble_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size));
        current = list->node[IDOUBLE_LIST_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->node[IDOUBLE_LIST_NEXT], list->allocator->arguments);
    list->allocator->free(list->node[IDOUBLE_LIST_PREV], list->allocator->arguments);

    list->capacity = list->head = list->length = list->size = 0;
    list->node[IDOUBLE_LIST_PREV] = list->node[IDOUBLE_LIST_NEXT] = NULL;
    list->elements = NULL;

    list->allocator = NULL;
}

void clear_idouble_list(idouble_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // call destroy function for each element in list
    for (size_t current = list->head, i = list->length; i; i--) {
        destroy(list->elements + (current * list->size));
        current = list->node[IDOUBLE_LIST_NEXT][current];
    }

    // free list's node arrays
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->node[IDOUBLE_LIST_NEXT], list->allocator->arguments);
    list->allocator->free(list->node[IDOUBLE_LIST_PREV], list->allocator->arguments);

    // make list clear, but still usable
    list->node[IDOUBLE_LIST_NEXT] = list->node[IDOUBLE_LIST_PREV] = NULL;
    list->elements = NULL;
    list->capacity = list->head = list->length = 0;
}

idouble_list_s copy_idouble_list(idouble_list_s const * const list, copy_fn const copy) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // allocate and set replica of list
    idouble_list_s const replica = {
        .capacity = list->capacity, .head = list->head, .length = list->length, .size = list->size,
        .node[IDOUBLE_LIST_NEXT] = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arguments),
        .node[IDOUBLE_LIST_PREV] = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arguments),
        .elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arguments),
        .allocator = list->allocator,
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");
    error((!replica.capacity || replica.node[IDOUBLE_LIST_NEXT]) && "Memory allocation failed.");
    error((!replica.capacity || replica.node[IDOUBLE_LIST_PREV]) && "Memory allocation failed.");

    // copy nodes (elements and indexes) into list
    for (size_t i = 0; i < list->length; ++i) {
        copy(replica.elements + (i * replica.size), list->elements + (i * list->size));
    }
    memcpy(replica.node[IDOUBLE_LIST_NEXT], list->node[IDOUBLE_LIST_NEXT], list->length * sizeof(size_t));
    memcpy(replica.node[IDOUBLE_LIST_PREV], list->node[IDOUBLE_LIST_PREV], list->length * sizeof(size_t));

    return replica;
}

bool is_empty_idouble_list(idouble_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    return !(list->length);
}

void insert_at_idouble_list(idouble_list_s * const restrict list, void const * const restrict element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(element != list && "Paremeters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // if list is full resize (expand) it
    if (list->length == list->capacity) {
        _idouble_list_resize(list, list->capacity + IDOUBLE_LIST_CHUNK);
    }

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    list->node[IDOUBLE_LIST_NEXT][list->length] = current;
    if (list->length) { // if list is not empty perform complex insertion
        list->node[IDOUBLE_LIST_PREV][list->length] = list->node[IDOUBLE_LIST_PREV][current];

        list->node[IDOUBLE_LIST_NEXT][list->node[IDOUBLE_LIST_PREV][current]] = list->length;
        list->node[IDOUBLE_LIST_PREV][current] = list->length;
    } else { // else list is empty and thus needs only a simple redirection to itself
        list->node[IDOUBLE_LIST_PREV][list->length] = current;
    }

    if (!index) { // if index is zero then list's head must become last empty array node
        list->head = list->length;
    }

    // copy element into list by pushing it as last array node
    memcpy(list->elements + (list->length * list->size), element, list->size);
    list->length++;
}

void get_idouble_list(idouble_list_s const * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(buffer != list && "Paremeters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // copy retrieved element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_idouble_list(idouble_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare) {
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
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        char const * const found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[IDOUBLE_LIST_NEXT][current];
        }

        _idouble_list_fill_hole(list, current);

        // if smaller capacity exists then resize (shrink) list
        if (list->length == list->capacity - IDOUBLE_LIST_CHUNK) {
            _idouble_list_resize(list, list->capacity - IDOUBLE_LIST_CHUNK);
        }

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_last_idouble_list(idouble_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare) {
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
        current = list->node[IDOUBLE_LIST_PREV][current];

        char const * found = list->elements + (current * list->size);
        if (0 != compare(element, found)) { // if element isn't found continue
            continue;
        } // else remove element and return successfully

        // copy removed element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // if current is head then change head to next node
        if (current == list->head) {
            list->head = list->node[IDOUBLE_LIST_NEXT][current];
        }

        _idouble_list_fill_hole(list, current);

        // if smaller capacity exists then resize (shrink) list
        if (list->length == list->capacity - IDOUBLE_LIST_CHUNK) {
            _idouble_list_resize(list, list->capacity - IDOUBLE_LIST_CHUNK);
        }

        return;
    }

    // if element wasn't found indicate error
    error(false && "Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_at_idouble_list(idouble_list_s * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(buffer != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // copy removed node element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
    list->length--;

    // if current is head then change head to next node
    if (current == list->head) {
        list->head = list->node[IDOUBLE_LIST_NEXT][current];
    }

    _idouble_list_fill_hole(list, current);

    // if smaller capacity exists then resize (shrink) list
    if (list->length == list->capacity - IDOUBLE_LIST_CHUNK) {
        _idouble_list_resize(list, list->capacity - IDOUBLE_LIST_CHUNK);
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
        size_t const next = list->node[IDOUBLE_LIST_NEXT][current];
        list->node[IDOUBLE_LIST_NEXT][current] = list->node[IDOUBLE_LIST_PREV][current];
        list->node[IDOUBLE_LIST_PREV][current] = next;

        current = list->node[IDOUBLE_LIST_PREV][current];
    }
}

void shift_next_idouble_list(idouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");
    error(list->length && "Can't shift empty list.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[IDOUBLE_LIST_NEXT][list->head];
    }
}

void shift_prev_idouble_list(idouble_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");
    error(list->length && "Can't shift empty list.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[IDOUBLE_LIST_PREV][list->head];
    }
}

void splice_idouble_list(idouble_list_s * const restrict destination, idouble_list_s * const restrict source, size_t const index) {
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
    size_t const mod = sum % IDOUBLE_LIST_CHUNK;
    _idouble_list_resize(destination, mod ? sum - mod + IDOUBLE_LIST_CHUNK : sum);

    // determine closest direction to index and go there
    size_t current = destination->head;
    size_t const real_index = index <= (destination->length >> 1) ? index : destination->length - index;
    bool const node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = destination->node[node_index][current];
    }

    // just copy source's elements and indexes relative to destination's node array (just increment them by length)
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    for (size_t i = 0; i < source->length; ++i) {
        destination->node[IDOUBLE_LIST_NEXT][destination->length + i] = source->node[IDOUBLE_LIST_NEXT][i] + destination->length;
        destination->node[IDOUBLE_LIST_PREV][destination->length + i] = source->node[IDOUBLE_LIST_PREV][i] + destination->length;
    }

    // redirect destination nodes if it and source had any
    if (destination->length && source->length) {
        const size_t first_destination = current;
        const size_t last_destination  = destination->node[IDOUBLE_LIST_PREV][current];

        const size_t first_source = source->head + destination->length;
        const size_t last_source  = source->node[IDOUBLE_LIST_PREV][source->head] + destination->length;

        destination->node[IDOUBLE_LIST_NEXT][last_destination] = first_source;
        destination->node[IDOUBLE_LIST_PREV][first_source] = last_destination;

        destination->node[IDOUBLE_LIST_NEXT][last_source] = first_destination;
        destination->node[IDOUBLE_LIST_PREV][first_destination] = last_source;
    }

    // if source isn't empty and index points to head node destination's make head to source's
    if (!index && source->length) {
        destination->head = source->head + destination->length;
    }

    destination->length += source->length;

    // clear (NOT DESTROY) source list
    source->allocator->free(source->elements, source->allocator->arguments);
    source->allocator->free(source->node[IDOUBLE_LIST_NEXT], source->allocator->arguments);
    source->allocator->free(source->node[IDOUBLE_LIST_PREV], source->allocator->arguments);

    source->node[IDOUBLE_LIST_NEXT] = source->node[IDOUBLE_LIST_PREV] = NULL;
    source->elements = NULL;
    source->length = source->capacity = source->head = 0;
}

idouble_list_s split_idouble_list(idouble_list_s * const list, size_t const index, size_t const length) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(length <= list->length && "Paremeter can't be greater than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // determine closest direction to index and go there
    size_t current = list->head;
    size_t const real_index = index <= (list->length >> 1) ? index : list->length - index;
    bool const node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // create split list
    size_t const split_mod = length % IDOUBLE_LIST_CHUNK;
    size_t const split_capacity = split_mod ? length - split_mod + IDOUBLE_LIST_CHUNK : length;
    idouble_list_s split = {
        .elements = list->allocator->alloc(split_capacity * list->size, list->allocator->arguments),
        .node[IDOUBLE_LIST_NEXT] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arguments),
        .node[IDOUBLE_LIST_PREV] = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arguments),

        .head = 0, .size = list->size, .length = 0, .capacity = split_capacity, .allocator = list->allocator,
    };
    error((!split.capacity || split.elements) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDOUBLE_LIST_NEXT]) && "Memory allocation failed.");
    error((!split.capacity || split.node[IDOUBLE_LIST_PREV]) && "Memory allocation failed.");

    // push list elements into split list (includes pointer magic)
    size_t * split_current = &(split.head);
    while (split.length < length) {
        (*split_current) = split.length; // set head and next nodes to next index
        split.node[IDOUBLE_LIST_PREV][split.length] = split.length - 1; // set previous node indexes to one minus current
        split.node[IDOUBLE_LIST_PREV][0] = length - 1; // set first node's prev to last element in list

        memcpy(split.elements + (split.length * split.size), list->elements + (current * list->size), list->size);
        split.length++;
        list->length--;

        const size_t next = list->node[IDOUBLE_LIST_NEXT][current];
        if (list->head == current) {
            list->head = next;
        }

        _idouble_list_fill_hole(list, current);

        // shrink list if smaller chunk is available
        if (list->length == list->capacity - IDOUBLE_LIST_CHUNK) {
            _idouble_list_resize(list, list->capacity - IDOUBLE_LIST_CHUNK);
        }

        split_current = split.node[IDOUBLE_LIST_NEXT] + (split.length - 1);
        current = (next == list->length) ? current : next;
    }
    (*split_current) = 0;

    // if split list contains head node change list's head to current (or last non removed) node
    if (!index || (index >= list->length)) {
        list->head = current;
    }

    return split;
}

idouble_list_s extract_idouble_list(idouble_list_s * const restrict list, filter_fn const filter, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");
    error(arguments != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // only create positive to save true filtered values
    idouble_list_s positive = { .size = list->size, .allocator = list->allocator, };

    size_t * pos = &(positive.head);
    size_t const length = list->length; // list length may change
    for (size_t i = 0, current = list->head; i < length; ++i) {
        char const * element = list->elements + (current * list->size);

        if (!filter(element, arguments)) { // if no extraction go to next list node and continue
            current = list->node[IDOUBLE_LIST_NEXT][current];
            continue;
        } // else extract and append list node into positive list

        (*pos) = positive.length; // set head and next nodes to next index
        if (positive.length == positive.capacity) { // expand capacity if needed
            _idouble_list_resize(&positive, positive.capacity + IDOUBLE_LIST_CHUNK);
        }
        positive.node[IDOUBLE_LIST_PREV][positive.length] = positive.length - 1; // set previous node indexes to one minus current
        positive.node[IDOUBLE_LIST_PREV][0] = positive.length; // set first node's prev to positive length

        // remove element from main list and add it to positive one
        memcpy(positive.elements + (positive.length * positive.size), element, list->size);
        positive.length++;
        list->length--;

        size_t const next = list->node[IDOUBLE_LIST_NEXT][current];
        if (list->head == current) {
            list->head = next;
        }

        _idouble_list_fill_hole(list, current);

        // shrink list if smaller chunk is available
        if (list->length == list->capacity - IDOUBLE_LIST_CHUNK) {
            _idouble_list_resize(list, list->capacity - IDOUBLE_LIST_CHUNK);
        }

        pos = positive.node[IDOUBLE_LIST_NEXT] + (positive.length - 1);
        current = (next == list->length) ? current : next; // nex may point to last node in array, which gets swapped
    }
    (*pos) = 0;

    return positive;
}

void map_next_idouble_list(idouble_list_s const * const restrict list, handle_fn const operate, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(operate && "Paremeter can't be NULL.");
    error(arguments != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each forward element in list call operate function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        if (!operate(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void map_prev_idouble_list(idouble_list_s const * const restrict list, handle_fn const handle, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(arguments != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // for each backward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[IDOUBLE_LIST_PREV][current];
        if (!handle(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void apply_idouble_list(idouble_list_s const * const restrict list, process_fn const process, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(arguments != list && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arguments);
    error((!list->length || elements_array) && "Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        memcpy(elements_array + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(elements_array, list->length, arguments);

    // copy elements back into list
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        memcpy(list->elements + (current * list->size), elements_array + (i * list->size), list->size);
    }

    list->allocator->free(elements_array, list->allocator->arguments);
}

void _idouble_list_resize(idouble_list_s * const list, size_t const size) {
    list->capacity = size;

    list->elements = list->allocator->realloc(list->elements, list->capacity * list->size, list->allocator->arguments);
    error((!list->capacity || list->elements) && "Memory allocation failed.");

    list->node[IDOUBLE_LIST_NEXT] = list->allocator->realloc(list->node[IDOUBLE_LIST_NEXT], list->capacity * sizeof(size_t), list->allocator->arguments);
    error((!list->capacity || list->node[IDOUBLE_LIST_NEXT]) && "Memory allocation failed.");

    list->node[IDOUBLE_LIST_PREV] = list->allocator->realloc(list->node[IDOUBLE_LIST_PREV], list->capacity * sizeof(size_t), list->allocator->arguments);
    error((!list->capacity || list->node[IDOUBLE_LIST_PREV]) && "Memory allocation failed.");
}

void _idouble_list_fill_hole(idouble_list_s * list, const size_t hole) {
    if (list->head == list->length) { list->head = hole; }

    // cut current removed node's siblings from itself
    list->node[IDOUBLE_LIST_NEXT][list->node[IDOUBLE_LIST_PREV][hole]] = list->node[IDOUBLE_LIST_NEXT][hole];
    list->node[IDOUBLE_LIST_PREV][list->node[IDOUBLE_LIST_NEXT][hole]] = list->node[IDOUBLE_LIST_PREV][hole];

    // cut current removed node from its siblings
    list->node[IDOUBLE_LIST_PREV][hole] = list->node[IDOUBLE_LIST_NEXT][hole] = hole;

    // replace element at current index with popped last element like in a stack
    memmove(list->elements + (hole * list->size), list->elements + (list->length * list->size), list->size);
    list->node[IDOUBLE_LIST_NEXT][hole] = list->node[IDOUBLE_LIST_NEXT][list->length];
    list->node[IDOUBLE_LIST_PREV][hole] = list->node[IDOUBLE_LIST_PREV][list->length];

    // redirect array's last swapped node's siblings to hole
    list->node[IDOUBLE_LIST_PREV][list->node[IDOUBLE_LIST_NEXT][list->length]] = hole;
    list->node[IDOUBLE_LIST_NEXT][list->node[IDOUBLE_LIST_PREV][list->length]] = hole;
}
