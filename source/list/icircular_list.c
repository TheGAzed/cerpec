#include <list/icircular_list.h>

#include <stdlib.h> // import exit()
#include <string.h>

#define NIL ((size_t)(-1))

/// @brief Resizes structure to new size.
/// @param list Strcuture to resize.
/// @param size New size to be used.
void _icircular_list_resize(icircular_list_s * const list, size_t const size);

icircular_list_s create_icircular_list(size_t const size) {
    error(size && "Paremeter can't be zero.");

    return (icircular_list_s) { .empty = NIL, .size = size, .allocator = &standard };
}

icircular_list_s make_icircular_list(size_t const size, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (icircular_list_s) { .empty = NIL, .size = size, .allocator = allocator };
}

void destroy_icircular_list(icircular_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // iterate over each element in list and call destroy function
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        destroy(list->elements + (current * list->size));
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    // set everything to zero
    list->capacity = list->empty = list->length = list->size = list->tail = 0;
    list->elements = NULL;
    list->next = NULL;
    list->allocator = NULL;
}

void clear_icircular_list(icircular_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // iterate over each element in list and call destroy function
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        destroy(list->elements + (current * list->size));
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    // set only non important parameters to zero/nil
    list->length = list->capacity = list->tail = 0;
    list->elements = NULL;
    list->next = NULL;

    list->empty = NIL;
}

icircular_list_s copy_icircular_list(icircular_list_s const * const list, copy_fn const copy) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // create a replica/copy structure
    icircular_list_s replica = {
        .empty = NIL, .size = list->size, .capacity = list->capacity, .tail = 0, .length = 0,
        .elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");
    error((!replica.capacity || replica.next) && "Memory allocation failed.");

    // for each element in list copy it into the replica while keeping circularity and making replica hole-less
    for (size_t l = list->tail, * r = &(replica.tail); replica.length < list->length; l = list->next[l], r = replica.next + (*r)) {
        (*r) = replica.length++;
        replica.next[(*r)] = replica.tail;

        copy(replica.elements + ((*r) * replica.size), list->elements + (l * list->size));
    }

    return replica;
}

bool is_empty_icircular_list(icircular_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    return !(list->length);
}

void insert_at_icircular_list(icircular_list_s * const restrict list, void const * const restrict element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(list != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // if length has reached capacity increase capacity linearly and call resize function to expand nodes
    if (list->length == list->capacity) {
        _icircular_list_resize(list, list->capacity + ICIRCULAR_LIST_CHUNK);
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

    memcpy(list->elements + (hole * list->size), element, list->size);
    list->length++;
}

void get_icircular_list(icircular_list_s const * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "Can't get element from empty list->");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    size_t current = list->tail;
    // iterate until current points to node at index, starting from tail
    for (size_t i = 0; (index != list->length - 1) && i <= index; ++i) {
        current = list->next[current];
    }

    memcpy(buffer, list->elements + (current * list->size), list->size);
}

void remove_first_icircular_list(icircular_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(list->length && "Can't get element from empty list.");
    error(list != element && "Parameters can't be equal.");
    error(list != buffer && "Parameters can't be equal.");
    error(element != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

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
            _icircular_list_resize(list, list->capacity - ICIRCULAR_LIST_CHUNK);
        }

        return; // leave function with found element
    }

    error(0 && "Element not found in list."); // if element is not found in list then that is an error
    // and exit failure is returned, since the function returns the removed element, element can contain allocated memory
    exit(EXIT_FAILURE);
}

void remove_at_icircular_list(icircular_list_s * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "Can't get element from empty list.");
    error(index < list->length && "Paremeter can't be greater than or equal length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    size_t previous = list->tail;
    // iterate until node previous to index node is reached
    for (size_t i = 0; i < index; ++i) {
        previous = list->next[previous];
    }

    size_t const current = list->next[previous]; // save current node index

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
        _icircular_list_resize(list, list->capacity - ICIRCULAR_LIST_CHUNK);
    }
}

void reverse_icircular_list(icircular_list_s * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

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

void shift_next_icircular_list(icircular_list_s * const list, size_t const shift) {
    error(list && "Paremeter can't be NULL.");
    error(list->length && "Can't shift empty list.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // shift tail node by iterating shift number of times
    for (size_t i = 0; i < shift; ++i) {
        list->tail = list->next[list->tail];
    }
}

void splice_icircular_list(icircular_list_s * const restrict destination, icircular_list_s * const restrict source, size_t const index) {
    error(destination && "Paremeter can't be NULL.");
    error(source && "Paremeter can't be NULL.");
    error(index <= destination->length && "Paremeter can't be greater than length.");
    error(source->size == destination->size && "Element sizes must be equal.");
    error(source != destination && "Parameters can't be equal.");

    valid(destination->size && "Size can't be zero.");
    valid(destination->length <= destination->capacity && "Length exceeds capacity.");
    valid(destination->allocator && "Allocator can't be NULL.");
    valid(destination->tail != NIL && "Tail can't be NIL.");

    valid(source->size && "Size can't be zero.");
    valid(source->length <= source->capacity && "Length exceeds capacity.");
    valid(source->allocator && "Allocator can't be NULL.");
    valid(source->tail != NIL && "Tail can't be NIL.");

    size_t const dest_length = destination->length;

    size_t const sum = destination->length + source->length;
    size_t const mod = sum % ICIRCULAR_LIST_CHUNK;
    _icircular_list_resize(destination, mod ? sum - mod + ICIRCULAR_LIST_CHUNK : sum);

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
    if (index == dest_length) {
        destination->tail = dest_prev;
    }

    // clear (NOT DESTROY) source list
    source->allocator->free(source->elements, source->allocator->arguments);
    source->allocator->free(source->next, source->allocator->arguments);

    source->elements = NULL;
    source->next = NULL;
    source->length = source->capacity = source->tail = 0;

    source->empty = NIL;
}

icircular_list_s split_icircular_list(icircular_list_s * const list, size_t const index, size_t const length) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Index can't be more than or equal length.");
    error(length <= list->length && "Size can't be more than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    size_t previous = list->tail;
    // iterate to previous node from index
    for (size_t i = 0; i < index; ++i) {
        previous = list->next[previous];
    }

    // create split list
    size_t const split_mod = length % ICIRCULAR_LIST_CHUNK;
    size_t const split_capacity = split_mod ? length - split_mod + ICIRCULAR_LIST_CHUNK : length;
    icircular_list_s split = {
        .capacity = split_capacity, .empty = NIL, .size = list->size,
        .elements = list->allocator->alloc(split_capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error((!split_capacity || split.elements) && "Memory allocation failed.");
    error((!split_capacity || split.next) && "Memory allocation failed.");

    // copy list elements into split list starting from index node (also redirect list's removed nodes)
    for (size_t * s = &(split.tail); split.length < length; s = split.next + (*s)) {
        size_t const current = list->next[previous]; // save current node index

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
    size_t const replica_length = list->length - length;
    size_t const replica_mod = replica_length % ICIRCULAR_LIST_CHUNK;
    size_t const replica_capacity = replica_mod ? replica_length - replica_mod + ICIRCULAR_LIST_CHUNK : replica_length;
    icircular_list_s replica = {
        .capacity = replica_capacity, .empty = NIL, .size = list->size,
        .elements = list->allocator->alloc(replica_capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(replica_capacity * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error((!replica_capacity || replica.elements) && "Memory allocation failed.");
    error((!replica_capacity || replica.next) && "Memory allocation failed.");

    // copy remaining list element into replica
    for (size_t * r = &(replica.tail); replica.length < replica_length; previous = list->next[previous], r = replica.next + (*r)) {
        // append remaining list node into replica list
        (*r) = replica.length;
        replica.next[(*r)] = replica.tail;

        // copy list's removed node into replica list
        memcpy(replica.elements + ((*r) * replica.size), list->elements + (previous * list->size), list->size);
        replica.length++;
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    (*list) = replica; // change list into hole-less replica

    return split;
}

icircular_list_s extract_icircular_list(icircular_list_s * const restrict list, filter_fn const filter, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // create temporary lists to save filtered elements
    icircular_list_s negative = { .empty = NIL, .size = list->size, .allocator = list->allocator, };
    icircular_list_s positive = { .empty = NIL, .size = list->size, .allocator = list->allocator, };

    // iterate over each element in list while calling filter function
    size_t * neg = &(negative.tail), * pos = &(positive.tail);
    size_t previous = list->tail;
    for (size_t i = 0, pos_idx = 0, neg_idx = 0; i < list->length; ++i) {
        size_t const current = list->next[previous]; // get current node

        char const * element = list->elements + (current * list->size); // save current element
        if (filter(element, arguments)) { // if element is valid push into positive list
            (*pos) = pos_idx;
            if (positive.length == positive.capacity) { // expand capacity if needed
                _icircular_list_resize(&positive, positive.capacity + ICIRCULAR_LIST_CHUNK);
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
                _icircular_list_resize(&negative, negative.capacity + ICIRCULAR_LIST_CHUNK);
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
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);
    (*list) = negative;

    return positive;
}

void map_icircular_list(icircular_list_s const * const restrict list, handle_fn const handle, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    // iterate over each element calling handle function
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        if (!handle(list->elements + (current * list->size), arguments)) {
            break;
        }
    }
}

void apply_icircular_list(icircular_list_s const * const restrict list, process_fn const process, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->tail != NIL && "Tail can't be NIL.");

    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arguments);
    error((!list->length || elements_array) && "Memory allocation failed.");

    // push list elements into elements array inorder
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        memcpy(elements_array + (i * list->size), list->elements + (current * list->size), list->size);
    }

    // process elements
    process(elements_array, list->length, arguments);

    // copy elements back into list
    for (size_t i = 0, current = list->tail; i < list->length; ++i) {
        current = list->next[current];
        memcpy(list->elements + (current * list->size), elements_array + (i * list->size), list->size);
    }

    list->allocator->free(elements_array, list->allocator->arguments);
}

void _icircular_list_resize(icircular_list_s * const list, size_t const size) {
    list->capacity = size;

    // if list expands or hole stack is empty then just expand/shrink the list and return
    if (list->capacity != list->length || NIL == list->empty) {
        list->elements = list->allocator->realloc(list->elements, list->capacity * list->size, list->allocator->arguments);
        error((!list->capacity || list->elements) && "Memory allocation failed.");

        list->next = list->allocator->realloc(list->next, list->capacity * sizeof(size_t), list->allocator->arguments);
        error((!list->capacity || list->next) && "Memory allocation failed.");

        return;
    } // else copy elements into new array in order, and clear hole stack

    // allocate new array to save alements linearly without holes
    char * elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arguments);
    error((!list->capacity || elements) && "Memory allocation failed.");

    // copy each element inside new array and set it to elements list starting from tail
    for (size_t i = 0, current = list->tail; i < list->length; ++i, current = list->next[current]) {
        memcpy(elements + (i * list->size), list->elements + (current * list->size), list->size);
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->elements = elements;

    // reallocate next indexes and set them from
    list->next = list->allocator->realloc(list->next, list->capacity * sizeof(size_t), list->allocator->arguments);
    error((!list->capacity || list->next) && "Memory allocation failed.");

    for (size_t i = 0, * current = &(list->tail); i < list->length; current = list->next + i, i++) {
        (*current) = i; // set index (including tail)
        list->next[i] = list->tail; // make next index circle back to tail
    }

    list->empty = NIL;
}
