#include <list/icircular_list.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL ((size_t)(-1))

void _icircular_list_resize(icircular_list_s * list);

icircular_list_s create_icircular_list(const size_t size) {
    return (icircular_list_s) { .empty = NIL, .size = size, .tail = 0 };
}

void destroy_icircular_list(icircular_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each element in list and call destroy function
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        destroy(list->elements + (current * list->size));
    }
    free(list->elements);
    free(list->next);

    memset(list, 0, sizeof(icircular_list_s)); // set everything to zero
}

void clear_icircular_list(icircular_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each element in list and call destroy function
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        destroy(list->elements + (current * list->size));
    }
    free(list->elements);
    free(list->next);

    // set only non important parameters to zero
    list->elements = NULL;
    list->next = NULL;
    list->length = list->capacity = list->tail = 0;
    list->empty = NIL;
}

icircular_list_s copy_icircular_list(const icircular_list_s list, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(!(list.capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    // create a replica/copy structure
    icircular_list_s replica = {
        .empty = NIL, .size = list.size, .capacity = list.capacity, .tail = 0, .length = 0,
        .elements = malloc(list.capacity * list.size),
        .next = malloc(list.capacity * list.size),
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // for each element in list copy it into the replica while keeping circularity and making replica hole-less
    for (size_t i = 0, l = list.tail, * r = &(replica.tail); i < list.length; ++i, l = list.next[l], r = replica.next + (*r)) {
        (*r) = i;
        replica.next[(*r)] = replica.tail;

        copy(replica.elements + ((*r) * replica.size), list.elements + (l * list.size));
    }

    return replica;
}

bool is_empty_icircular_list(const icircular_list_s list) {
    assert(list.size && "[INVALID] Size can't be zero.");
    assert(!(list.capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    return !(list.length);
}

void insert_at_icircular_list(icircular_list_s * restrict list, const void * restrict element, const size_t index) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(index <= list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // if length has reached capacity increase capacity linearly and call resize function to expand nodes
    if (list->length == list->capacity) {
        list->capacity += ICIRCULAR_LIST_CHUNK;
        _icircular_list_resize(list);
    }

    // get the empty node index either from underlying stack or next empty index in array (i.e. length)
    size_t hole = list->length;
    if (NIL != list->empty) { // if stack isn't empty pop hole index
        hole = list->empty;
        list->empty = list->next[list->empty];
    }

    // if list isn't empty get pointer to head node, else get list's tail node pointer
    size_t * current = list->length ? list->next + list->tail : &(list->tail);
    // while index wasn't reached and index isn't last node iterate
    for (size_t i = 0; (index != list->length) && i < index; ++i) {
        current = list->next + (*current);
    }
    // if length is zero (and tail index also) the next node from hole will circle around, else it's added normally
    list->next[hole] = (*current);
    // if length is zero (current points to list's tail index) the list will circle around, else it's added normally
    (*current) = hole;

    if (index == list->length) { // special case when index is equal to length (new tail is added)
        list->tail = hole;
    }

#if 0
    if (!list->length) {
        list->next[list->tail] = hole;
    } else if (index == list->length) {
        list->next[hole] = list->next[list->tail];
        list->next[list->tail] = hole;
        list->tail = hole;
    } else {
        size_t previous = list->tail;
        for (size_t i = 0; i < index; ++i) {
            previous = list->next[previous];
        }
        list->next[hole] = list->next[previous];
        list->next[previous] = hole;
    }
#endif
    memcpy(list->elements + (hole * list->size), element, list->size);
    list->length++;
}

void get_icircular_list(const icircular_list_s list, const size_t index, void * buffer) {
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(list.length && "[ERROR] Can't get element from empty list.");
    assert(index < list.length && "[ERROR] Paremeter can't be greater than length.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(!(list.capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    size_t current = list.tail;
    // iterate until current points to node at index, starting from tail
    for (size_t i = 0; (index != list.length - 1) && i <= index; ++i) {
        current = list.next[current];
    }

    memcpy(buffer, list.elements + (current * list.size), list.size);
}

void remove_first_icircular_list(icircular_list_s * restrict list, const void * restrict element, void * restrict buffer, const compare_fn compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't get element from empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each element until searched element is found
    for (size_t i = 0, previous = list->tail; i < list->length; ++i, previous = list->next[previous]) {
        const size_t current = list->next[previous]; // save current node index

        // if comparison is false (not zero) continue iterating (also avoids useless depth)
        if (0 != compare(list->elements + (current * list->size), element)) {
            continue;
        } // else element was found and gets removed

        // save removed element into buffer and decrecment list length
        memcpy(buffer, list->elements + (current * list->size), list->size);
        list->length--;

        // redirect previous node
        list->next[previous] = list->next[current];

        // appends into empty node stack the hole index left by removed node
        // checks if current isn't the last index in the node's array (to avoid useless holes in stack)
        if (current != list->length) {
            list->next[current] = list->empty;
            list->empty = current;
        }

        // if current removed index was the tail node then change tail to previous node
        if (current == list->tail) {
            list->tail = previous;
        }

        // if length has reached smaller capacity decrease capacity and resize the list
        if (list->length == list->capacity - ICIRCULAR_LIST_CHUNK) {
            list->capacity -= ICIRCULAR_LIST_CHUNK;
            _icircular_list_resize(list);
        }

        return; // leave function with found element
    }

    assert(0 && "[ERROR] Element not found in list."); // if element is not found in list then that is an error
    // and exit failure is returned, since the function returns the removed element, element can contain allocated memory
    exit(EXIT_FAILURE);
}

void remove_at_icircular_list(icircular_list_s * list, const size_t index, void * buffer) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't get element from empty list.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than or equal length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    size_t previous = list->tail;
    // iterate until node previous to index node is reached
    for (size_t i = 0; i < index; ++i) {
        previous = list->next[previous];
    }

    const size_t current = list->next[previous]; // save current node index

    // save removed element into buffer and decrecment list length
    memcpy(buffer, list->elements + (current * list->size), list->size);
    list->length--;

    list->next[previous] = list->next[current]; // redirect previous node

    // appends into empty node stack the hole index left by removed node
    // checks if current isn't the last index in the node's array (to avoid useless holes in stack)
    if (current != list->length) {
        list->next[current] = list->empty;
        list->empty = current;
    }

    // if current removed index was the tail node then change tail to previous node
    if (current == list->tail) {
        list->tail = previous;
    }

    // if length has reached smaller capacity decrease capacity and resize the list
    if (list->length == list->capacity - ICIRCULAR_LIST_CHUNK) {
        list->capacity -= ICIRCULAR_LIST_CHUNK;
        _icircular_list_resize(list);
    }
}

void reverse_icircular_list(icircular_list_s * list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    if (!list->length) { // if list is empty early return to not break the function code
        return;
    }

    // create prevoius, current and next node indexes starting from list tail
    size_t previous = list->tail, current = list->next[previous], next = list->next[current];
    list->tail = current; // save new tail as current/head node
    for (size_t i = 0; i < list->length; ++i) { // reverse next node indexes via iterating over the list
        list->next[current] = previous;
        previous = current;
        current = next;
        next = list->next[next];
    }
}

void shift_next_icircular_list(icircular_list_s * list, const size_t shift) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Can't shift empty list.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->tail = list->next[list->tail];
    }
}

void splice_icircular_list(icircular_list_s * restrict destination, icircular_list_s * restrict source, const size_t index) {
    assert(destination && "[ERROR] Paremeter can't be NULL.");
    assert(source && "[ERROR] Paremeter can't be NULL.");
    assert(index < destination->length && "[ERROR] Paremeter can't be greater than length.");

    assert(destination->size && "[INVALID] Size can't be zero.");
    assert(!(destination->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(destination->length <= destination->capacity && "[INVALID] Length exceeds capacity.");
    assert(source->size && "[INVALID] Size can't be zero.");
    assert(!(source->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(source->length <= source->capacity && "[INVALID] Length exceeds capacity.");
    assert(source->size == destination->size && "[INVALID] Element sizes must be equal.");

    const size_t sum = destination->length + source->length;
    const size_t mod = sum % ICIRCULAR_LIST_CHUNK;
    destination->capacity = mod ? sum - mod + ICIRCULAR_LIST_CHUNK : sum;
    _icircular_list_resize(destination);

    size_t dest_prev = destination->tail;
    // iterate to previous node from index
    for (size_t i = 0; i < index; ++i) {
        dest_prev = destination->next[dest_prev];
    }

    size_t src_prev = source->tail; // save previous source node
    // copy source elements into destination while destination's hole stack isn't empty and source has elements
    while (NIL != destination->empty && source->length) {
        // pop empty hole index from stack
        const size_t hole = destination->empty;
        destination->empty = destination->next[destination->empty];

        // redirect nodes
        destination->next[hole] = destination->next[dest_prev];
        destination->next[dest_prev] = hole;

        // copy source element into destination and change lengths
        const size_t src_curr = source->next[src_prev];
        memcpy(destination->elements + (hole * destination->size), source->elements + (src_curr * source->size), source->size);
        destination->length++;
        source->length--;

        // go to next destination and source node
        dest_prev = hole;
        src_prev = src_curr;
    }

    // copy source elements into destination while source has elements
    while (source->length) {
        const size_t hole = destination->length; // get rightmost array hole index

        // redirect nodes
        destination->next[hole] = destination->next[dest_prev];
        destination->next[dest_prev] = hole;

        // copy source element into destination and change lengths
        const size_t src_curr = source->next[src_prev];
        memcpy(destination->elements + (hole * destination->size), source->elements + (src_curr * source->size), source->size);
        destination->length++;
        source->length--;

        // go to next destination and source node
        dest_prev = hole;
        src_prev = src_curr;
    }

    // if index points to last/tail node (elemetn were appended), change tail to last added source node
    if (index == destination->tail) {
        destination->tail = dest_prev;
    }

    // clear (NOT DESTROY) source list
    free(source->elements);
    free(source->next);

    source->elements = NULL;
    source->next = NULL;
    source->length = source->capacity = source->tail = 0;
    source->empty = NIL;
}

icircular_list_s split_icircular_list(icircular_list_s * list, const size_t index, const size_t length) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Index can't be more than or equal length.");
    assert(length <= list->length && "[ERROR] Size can't be more than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    size_t previous = list->tail;
    // iterate to previous node from index
    for (size_t i = 0; i < index; ++i) {
        previous = list->next[previous];
    }

    // create split list
    const size_t split_mod = length % ICIRCULAR_LIST_CHUNK;
    const size_t split_capacity = split_mod ? length - split_mod + ICIRCULAR_LIST_CHUNK : length;
    icircular_list_s split = {
        .capacity = split_capacity, .empty = NIL, .tail = 0, .length = 0, .size = list->size,
        .elements = malloc(split_capacity * list->size), .next = malloc(split_capacity * sizeof(size_t)),
    };
    assert((!split_capacity || split.elements) && "[ERROR] Memory allocation failed.");
    assert((!split_capacity || split.next) && "[ERROR] Memory allocation failed.");

    // copy list elements into split list starting from index node (also redirect list's removed nodes)
    for (size_t * s = &(split.tail); split.length < length; s = split.next + (*s)) {
        const size_t current = list->next[previous]; // save current node index

        list->next[previous] = list->next[current]; // redirect removed node

        // append removed node into split list
        (*s) = split.tail = split.length;
        split.next[(*s)] = 0;

        // copy list's removed node into split list
        memcpy(split.elements + ((*s) * split.size), list->elements + (current * list->size), list->size);
        split.length++;
    }

    // if list's tail gets split into new list, then change tail to node after last split
    if ((index <= list->length - 1) && (index + length >= list->length - 1)) {
        list->tail = previous;
    }

    // create replica of parameter list to remove holes in it
    const size_t replica_length = list->length - length;
    const size_t replica_mod = replica_length % ICIRCULAR_LIST_CHUNK;
    const size_t replica_capacity = replica_mod ? replica_length - replica_mod + ICIRCULAR_LIST_CHUNK : replica_length;
    icircular_list_s replica = {
        .capacity = replica_capacity, .empty = NIL, .tail = 0, .length = 0, .size = list->size,
        .elements = malloc(replica_capacity * list->size), .next = malloc(replica_capacity * sizeof(size_t)),
    };
    assert((!replica_capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica_capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // copy remaining list element into replica
    for (size_t * r = &(replica.tail); replica.length < replica_length; previous = list->next[previous], r = replica.next + (*r)) {
        // append remaining list node into replica list
        (*r) = replica.length;
        replica.next[(*r)] = replica.tail;

        // copy list's removed node into replica list
        memcpy(replica.elements + ((*r) * replica.size), list->elements + (previous * list->size), list->size);
        replica.length++;
    }
    free(list->elements);
    free(list->next);

    (*list) = replica; // change list into hole-less replica

    return split;
}

icircular_list_s extract_icircular_list(icircular_list_s * list, const filter_fn filter, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(filter && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(!(list->capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // create temporary lists to save filtered elements
    icircular_list_s negative = { .empty = NIL, .tail = 0, .size = list->size, };
    icircular_list_s positive = { .empty = NIL, .tail = 0, .size = list->size, };

    // iterate over each element in list while calling filter function
    size_t * neg = &(negative.tail), * pos = &(positive.tail);
    size_t previous = list->tail;
    for (size_t i = 0, pos_idx = 0, neg_idx = 0; i < list->length; ++i) {
        const size_t current = list->next[previous]; // get current node

        const char * element = list->elements + (current * list->size); // save current element
        if (filter(element, arguments)) { // if element is valid push into positive list
            (*pos) = pos_idx;
            if (positive.length == positive.capacity) { // expand capacity if needed
                positive.capacity += ICIRCULAR_LIST_CHUNK;
                _icircular_list_resize(&positive);
            }
            positive.next[pos_idx] = 0; // make list circular
            // copy element into list
            memcpy(positive.elements + (pos_idx * positive.size), element, list->size);
            positive.length++;

            pos = positive.next + pos_idx; // go to positive next node
            pos_idx++;
        } else { // else push into negative list
            (*neg) = neg_idx;
            if (negative.length == negative.capacity) { // expand capacity if needed
                negative.capacity += ICIRCULAR_LIST_CHUNK;
                _icircular_list_resize(&negative);
            }
            negative.next[neg_idx] = 0; // make list circular
            // copy element into list
            memcpy(negative.elements + (neg_idx * negative.size), element, list->size);
            negative.length++;

            neg = negative.next + neg_idx; // go to negative next node
            neg_idx++;
        }

        previous = current; // go to next previous node
    }
    // redirect tail node
    positive.tail = positive.length ? positive.length - 1 : 0;
    negative.tail = negative.length ? negative.length - 1 : 0;

    // change list into negative and return positive one
    free(list->elements);
    free(list->next);
    (*list) = negative;

    return positive;
}

void foreach_icircular_list(const icircular_list_s list, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(!(list.capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each element calling operate function
    for (size_t i = 0, current = list.tail; i < list.length; ++i) {
        current = list.next[current];
        if (!operate(list.elements + (current * list.size), arguments)) {
            break;
        }
    }
}

void map_icircular_list(const icircular_list_s list, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(!(list.capacity % ICIRCULAR_LIST_CHUNK) && "[INVALID] Capacity must be modulo of chunk size.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    char * elements = malloc(list.length * list.size);
    assert((!list.length || elements) && "[ERROR] Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list.tail; i < list.length; ++i) {
        current = list.next[current];
        memcpy(elements + (i * list.size), list.elements + (current * list.size), list.size);
    }

    // manage elements
    manage(list.elements, list.length, arguments);

    // copy elements back into list
    for (size_t i = 0, current = list.tail; i < list.length; ++i) {
        current = list.next[current];
        memcpy(list.elements + (current * list.size), elements + (i * list.size), list.size);
    }

    free(elements);
}

void _icircular_list_resize(icircular_list_s * list) {
    // if list expands or hole stack is empty then just expand/shrink the list and return
    if (list->capacity != list->length || NIL == list->empty) {
        list->elements = realloc(list->elements, list->capacity * list->size);
        list->next = realloc(list->next, list->capacity * sizeof(size_t));

        assert((!list->capacity || list->elements) && "[ERROR] Memory allocation failed.");
        assert((!list->capacity || list->next) && "[ERROR] Memory allocation failed.");

        return;
    } // else copy elements into new array in order, and clear hole stack

    char * elements = malloc(list->capacity * list->size);
    assert((!list->capacity || elements) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0, current = list->tail; i < list->length; ++i, current = list->next[current]) {
        memcpy(elements + (i * list->size), list->elements + (current * list->size), list->size);
    }
    free(list->elements);
    list->elements = elements;

    for (size_t i = 0, * current = &(list->tail); i < list->length; ++i, current = list->next + i) {
        (*current) = i;
        list->next[i] = list->tail;
    }

    list->empty = NIL;
}
