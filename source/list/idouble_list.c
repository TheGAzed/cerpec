#include <list/idouble_list.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/// @brief Resizes structure to new size.
/// @param list Strcuture to resize.
/// @param size New size to be used.
void _idouble_list_resize(idouble_list_s * list, const size_t size);

/// @brief Fills hole/empty node index with last array-based node and fixes/redirects siblings.
/// @param list Strcuture to fill.
/// @param hole Index of hole node.
void _idouble_list_fill_hole(idouble_list_s * list, const size_t hole);

idouble_list_s create_idouble_list(const size_t size) {
    return (idouble_list_s) { .size = size };
}

void destroy_idouble_list(idouble_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // call destroy function for each element in list
    for (size_t current = list->head; list->length; list->length--) {
        destroy(list->elements + (current * list->size));
        current = list->node[IDOUBLE_LIST_NEXT][current];
    }

    // free list's node arrays
    free(list->elements);
    free(list->node[IDOUBLE_LIST_NEXT]);
    free(list->node[IDOUBLE_LIST_PREV]);

    memset(list, 0, sizeof(idouble_list_s)); // set list to zero/make it invalid
}

void clear_idouble_list(idouble_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // call destroy function for each element in list
    for (size_t current = list->head; list->length; list->length--) {
        destroy(list->elements + (current * list->size));
        current = list->node[IDOUBLE_LIST_NEXT][current];
    }

    // free list's node arrays
    free(list->elements);
    free(list->node[IDOUBLE_LIST_NEXT]);
    free(list->node[IDOUBLE_LIST_PREV]);

    // make list clear, but still usable
    list->node[IDOUBLE_LIST_NEXT] = list->node[IDOUBLE_LIST_PREV] = NULL;
    list->elements = NULL;
    list->capacity = list->head = 0;
}

idouble_list_s copy_idouble_list(const idouble_list_s * list, const copy_fn copy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // allocate and set replica of list
    const idouble_list_s replica = {
        .capacity = list->capacity, .head = list->head, .length = list->length, .size = list->size,
        .node[IDOUBLE_LIST_NEXT] = malloc(list->capacity * sizeof(size_t)),
        .node[IDOUBLE_LIST_PREV] = malloc(list->capacity * sizeof(size_t)),
        .elements = malloc(list->capacity * list->size),
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.node[IDOUBLE_LIST_NEXT]) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.node[IDOUBLE_LIST_PREV]) && "[ERROR] Memory allocation failed.");

    // copy nodes (elements and indexes) into list
    for (size_t i = 0; i < list->length; ++i) {
        copy(replica.elements + (i * replica.size), list->elements + (i * list->size));
    }
    memcpy(replica.node[IDOUBLE_LIST_NEXT], list->node[IDOUBLE_LIST_NEXT], list->length * sizeof(size_t));
    memcpy(replica.node[IDOUBLE_LIST_PREV], list->node[IDOUBLE_LIST_PREV], list->length * sizeof(size_t));

    return replica;
}

bool is_empty_idouble_list(const idouble_list_s * list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    return !(list->length);
}

void insert_at_idouble_list(idouble_list_s * restrict list, const void * restrict element, const size_t index) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(index <= list->length && "[ERROR] Paremeter can't be greater than length.");
    assert(element != list && "[ERROR] Paremeters can't be the same.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // if list is full resize (expand) it
    if (list->length == list->capacity) {
        _idouble_list_resize(list, list->capacity + IDOUBLE_LIST_CHUNK);
    }

    // determine closest direction to index and go there
    size_t current = list->head;
    const size_t real_index = index <= (list->length >> 1) ? index : list->length - index;
    const bool node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
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

void get_idouble_list(const idouble_list_s * list, const size_t index, void * buffer) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // determine closest direction to index and go there
    size_t current = list->head;
    const size_t real_index = index <= (list->length >> 1) ? index : list->length - index;
    const bool node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // copy retrieved element into buffer
    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_idouble_list(idouble_list_s * restrict list, const void * restrict element, void * restrict buffer, const compare_fn compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(list != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list != element && "[ERROR] Paremeters can't be the same.");
    assert(element != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list->elements && "[ERROR] Array can't be NULL.");
    assert(list->node[IDOUBLE_LIST_NEXT] && "[ERROR] Array can't be NULL.");
    assert(list->node[IDOUBLE_LIST_PREV] && "[ERROR] Array can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // for each element in list travel forward
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        const char * found = list->elements + (current * list->size);
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
    assert(false && "[ERROR] Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_last_idouble_list(idouble_list_s * restrict list, const void * restrict element, void * restrict buffer, const compare_fn compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(list != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list != element && "[ERROR] Paremeters can't be the same.");
    assert(element != buffer && "[ERROR] Paremeters can't be the same.");
    assert(list->elements && "[ERROR] Array can't be NULL.");
    assert(list->node[IDOUBLE_LIST_NEXT] && "[ERROR] Array can't be NULL.");
    assert(list->node[IDOUBLE_LIST_PREV] && "[ERROR] Array can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // for each element in list travel backwards
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[IDOUBLE_LIST_PREV][current];

        const char * found = list->elements + (current * list->size);
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
    assert(false && "[ERROR] Element not found in list.");
    exit(EXIT_FAILURE);
}

void remove_at_idouble_list(idouble_list_s * restrict list, const size_t index, void * restrict buffer) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");
    assert(list != buffer && "[ERROR] Paremeters can't be the same.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // determine closest direction to index and go there
    size_t current = list->head;
    const size_t real_index = index <= (list->length >> 1) ? index : list->length - index;
    const bool node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
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

void reverse_idouble_list(idouble_list_s * list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    size_t current = list->head;
    for (size_t i = 0; i < list->length; ++i) {
        list->head = current;

        // swap next and previous node indexes
        const size_t next = list->node[IDOUBLE_LIST_NEXT][current];
        list->node[IDOUBLE_LIST_NEXT][current] = list->node[IDOUBLE_LIST_PREV][current];
        list->node[IDOUBLE_LIST_PREV][current] = next;

        current = list->node[IDOUBLE_LIST_PREV][current];
    }
}

void shift_next_idouble_list(idouble_list_s * list, const size_t shift) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't shift empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[IDOUBLE_LIST_NEXT][list->head];
    }
}

void shift_prev_idouble_list(idouble_list_s * list, const size_t shift) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't shift empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->head = list->node[IDOUBLE_LIST_PREV][list->head];
    }
}

void splice_idouble_list(idouble_list_s * restrict destination, idouble_list_s * restrict source, const size_t index) {
    assert(destination && "[ERROR] Paremeter can't be NULL.");
    assert(source && "[ERROR] Paremeter can't be NULL.");
    assert(index <= destination->length && "[ERROR] Paremeter can't be greater than length.");
    assert(destination != source && "[ERROR] Paremeters can't be the same.");

    assert(destination->size && "[INVALID] Size can't be zero.");
    assert(!(destination->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(destination->length <= destination->capacity && "[INVALID] Length exceeds capacity.");

    assert(source->size && "[INVALID] Size can't be zero.");
    assert(!(source->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(source->length <= source->capacity && "[INVALID] Length exceeds capacity.");

    assert(source->size == destination->size && "[INVALID] Element sizes must be equal.");

    // calculate new capacity of destination list and resize it
    const size_t sum = destination->length + source->length;
    const size_t mod = sum % IDOUBLE_LIST_CHUNK;
    _idouble_list_resize(destination, mod ? sum - mod + IDOUBLE_LIST_CHUNK : sum);

    // determine closest direction to index and go there
    size_t current = destination->head;
    const size_t real_index = index <= (destination->length >> 1) ? index : destination->length - index;
    const bool node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
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
    free(source->elements);
    free(source->node[IDOUBLE_LIST_NEXT]);
    free(source->node[IDOUBLE_LIST_PREV]);

    source->node[IDOUBLE_LIST_NEXT] = source->node[IDOUBLE_LIST_PREV] = NULL;
    source->elements = NULL;
    source->length = source->capacity = source->head = 0;
}

idouble_list_s split_idouble_list(idouble_list_s * list, const size_t index, const size_t length) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");
    assert(length <= list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // determine closest direction to index and go there
    size_t current = list->head;
    const size_t real_index = index <= (list->length >> 1) ? index : list->length - index;
    const bool node_index = real_index == index ? IDOUBLE_LIST_NEXT : IDOUBLE_LIST_PREV;
    for (size_t i = 0; i < real_index; ++i) {
        current = list->node[node_index][current];
    }

    // create split list
    const size_t split_mod = length % IDOUBLE_LIST_CHUNK;
    const size_t split_capacity = split_mod ? length - split_mod + IDOUBLE_LIST_CHUNK : length;
    idouble_list_s split = {
        .elements = malloc(split_capacity * list->size),
        .node[IDOUBLE_LIST_NEXT] = malloc(split_capacity * sizeof(size_t)),
        .node[IDOUBLE_LIST_PREV] = malloc(split_capacity * sizeof(size_t)),

        .head = 0, .size = list->size, .length = 0, .capacity = split_capacity,
    };
    assert((!split.capacity || split.elements) && "[ERROR] Memory allocation failed.");
    assert((!split.capacity || split.node[IDOUBLE_LIST_NEXT]) && "[ERROR] Memory allocation failed.");
    assert((!split.capacity || split.node[IDOUBLE_LIST_PREV]) && "[ERROR] Memory allocation failed.");

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

idouble_list_s extract_idouble_list(idouble_list_s * list, const filter_fn filter, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(filter && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    idouble_list_s positive = { .size = list->size, };

    size_t * pos = &(positive.head);
    const size_t length = list->length; // list length may change
    for (size_t i = 0, current = list->head; i < length; ++i) {
        const char * element = list->elements + (current * list->size);

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

        const size_t next = list->node[IDOUBLE_LIST_NEXT][current];
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

void map_next_idouble_list(const idouble_list_s * list, const handle_fn operate, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // for each forward element in list call operate function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->node[IDOUBLE_LIST_NEXT][current]) {
        if (!operate(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void map_prev_idouble_list(const idouble_list_s * list, const handle_fn handle, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(handle && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // for each backward element in list call handle function and break if it returns false
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        current = list->node[IDOUBLE_LIST_PREV][current];
        if (!handle(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void apply_idouble_list(const idouble_list_s * list, const process_fn process, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(process && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % IDOUBLE_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    char * elements = malloc(list->length * list->size);
    assert((!list->length || elements) && "[ERROR] Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        memcpy(elements + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(list->elements, list->length, arguments);

    // copy elements back into list
    for (size_t i = 0, current = list->head; i < list->length; ++i) {
        memcpy(list->elements + (current * list->size), elements + (i * list->size), list->size);
    }

    free(elements);
}

void _idouble_list_resize(idouble_list_s * list, const size_t size) {
    list->capacity = size;

    list->elements = realloc(list->elements, list->capacity * list->size);
    list->node[IDOUBLE_LIST_NEXT] = realloc(list->node[IDOUBLE_LIST_NEXT], list->capacity * sizeof(size_t));
    list->node[IDOUBLE_LIST_PREV] = realloc(list->node[IDOUBLE_LIST_PREV], list->capacity * sizeof(size_t));

    assert((!list->capacity || list->elements) && "[ERROR] Memory allocation failed.");
    assert((!list->capacity || list->node[IDOUBLE_LIST_NEXT]) && "[ERROR] Memory allocation failed.");
    assert((!list->capacity || list->node[IDOUBLE_LIST_PREV]) && "[ERROR] Memory allocation failed.");
}

void _idouble_list_fill_hole(idouble_list_s * list, const size_t hole) {
    if (list->head == list->length) {
        list->head = hole;
    }

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
