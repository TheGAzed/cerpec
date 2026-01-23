#include <list/istraight_list.h>

#include <stdlib.h> // import exit()
#include <string.h>

#define NIL ((size_t)(-1))

/// @brief Resizes structure to new size.
/// @param list Strcuture to resize.
/// @param size New size to be used.
void _istraight_list_resize(istraight_list_s * const list, size_t const size);

istraight_list_s create_istraight_list(size_t const size) {
    error(size && "Paremeter can't be zero.");

    return (istraight_list_s) { .head = NIL, .empty = NIL, .size = size, .allocator = &standard, };
}

istraight_list_s make_istraight_list(size_t const size, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (istraight_list_s) { .head = NIL, .empty = NIL, .size = size, .allocator = allocator, };
}

void destroy_istraight_list(istraight_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; i != NIL; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    list->capacity = list->empty = list->head = list->length = list->size = 0;
    list->elements = NULL;
    list->next = NULL;

    list->allocator = NULL;
}

void clear_istraight_list(istraight_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; NIL != i; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    // clear (NOT destroy) list
    list->elements = NULL;
    list->next = NULL;
    list->length = list->capacity = 0;
    list->head = list->empty = NIL;
}

istraight_list_s copy_istraight_list(istraight_list_s const * const list, copy_fn const copy) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // create copy/replica list
    istraight_list_s replica = {
        .capacity = list->capacity, .empty = NIL, .head = NIL, .length = list->length, .size = list->size,
        .elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->capacity * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");
    error((!replica.capacity || replica.next) && "Memory allocation failed.");

    // copy each element into replica (aslo make new list hole-less)
    for (size_t i = list->head, * r = &(replica.head), index = 0; NIL != i; i = list->next[i], r = replica.next + index, index++) {
        (*r) = index;
        replica.next[index] = NIL;

        copy(replica.elements + (index * replica.size), list->elements + (i * list->size));
    }

    return replica;
}

bool is_empty_istraight_list(istraight_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    return !(list->length);
}

void insert_at_istraight_list(istraight_list_s * const restrict list, void const * const restrict element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(list != element && "Paremeters can't be equal.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(list != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // if list can't fit elements expand it
    if (list->length == list->capacity) {
        _istraight_list_resize(list, list->capacity + ISTRAIGHT_LIST_CHUNK);
    }

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // if list has holes then pop empty hole index, else continue with rightmost array index
    size_t hole = list->length;
    if (NIL != list->empty) {
        hole = list->empty;
        list->empty = list->next[list->empty];
    }

    // insert and redirect hole node
    list->next[hole] = (*node);
    (*node) = hole;

    // insert element into hole node
    memcpy(list->elements + (hole * list->size), element, list->size);
    list->length++;
}

void get_istraight_list(istraight_list_s const * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "List can't be empty.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // iterate until node at index isn't reached
    size_t node = list->head;
    for (size_t i = 0; i < index; ++i) {
        node = list->next[node];
    }

    // copy element into buffer
    memcpy(buffer, list->elements + (node * list->size), list->size);
}

void remove_first_istraight_list(istraight_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(list->length && "Paremeter can't be zero.");
    error(list != element && "Parameters can't be equal.");
    error(list != buffer && "Paremeters can't be the same.");
    error(element != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // iterate until node with element isn't reached
    for (size_t * i = &(list->head); NIL != *i; i = list->next + (*i)) {
        char const * found = list->elements + ((*i) * list->size); // save found element
        if (0 != compare(element, found)) { // if element isn't equal continue
            continue;
        }

        // copy found element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // save removed node as hole and redirect nodes
        size_t const hole = (*i);
        (*i) = list->next[(*i)];

        // push hole index onto stack (minimize holes by checking if node isn't the rightmost)
        if (hole != list->length) {
            list->next[hole] = list->empty;
            list->empty = hole;
        }

        // shrink list to save space if smaller capacity is available
        if (list->length == list->capacity - ISTRAIGHT_LIST_CHUNK) {
            _istraight_list_resize(list, list->capacity - ISTRAIGHT_LIST_CHUNK);
        }

        return; // leave function with found element
    }

    error(0 && "Element not found in list."); // if element is not found in list then that is an error
    // and exit failure is returned, since the function returns the removed element, element can contain allocated memory
    exit(EXIT_FAILURE);
}

void remove_at_istraight_list(istraight_list_s * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "Paremeter can't be zero.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // copy found element into buffer
    memcpy(buffer, list->elements + ((*node) * list->size), list->size);
    list->length--;

    // save removed node as hole and redirect nodes
    size_t const hole = (*node);
    (*node) = list->next[(*node)];

    // push hole index onto stack (minimize holes by checking if node isn't the rightmost)
    if (hole != list->length) {
        list->next[hole] = list->empty;
        list->empty = hole;
    }

    // shrink list to save space if smaller capacity is available
    if (list->length == list->capacity - ISTRAIGHT_LIST_CHUNK) {
        _istraight_list_resize(list, list->capacity - ISTRAIGHT_LIST_CHUNK);
    }
}

void reverse_istraight_list(istraight_list_s * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    size_t previous = NIL;
    for (size_t i = 0, current = list->head, next = NIL; i < list->length; ++i, previous = current, current = next) {
        next = list->next[current];
        list->next[current] = previous;
    }
    list->head = previous;
}

void splice_istraight_list(istraight_list_s * const restrict destination, istraight_list_s * const restrict source, size_t const index) {
    error(destination && "Paremeter can't be NULL.");
    error(source && "Paremeter can't be NULL.");
    error(index <= destination->length && "Paremeter can't be greater than length.");
    error(source->size == destination->size && "Element sizes must be equal.");
    error(destination != source && "Parameters can't be equal.");

    valid(destination->size && "Size can't be zero.");
    valid(destination->length <= destination->capacity && "Length exceeds capacity.");
    valid(destination->allocator && "Allocator can't be NULL.");

    valid(source->size && "Size can't be zero.");
    valid(source->length <= source->capacity && "Length exceeds capacity.");
    valid(source->allocator && "Allocator can't be NULL.");

    // calculate new destination length
    size_t const sum = destination->length + source->length;
    size_t const mod = sum % ISTRAIGHT_LIST_CHUNK;
    _istraight_list_resize(destination, mod ? sum - mod + ISTRAIGHT_LIST_CHUNK : sum);

    // go to destination node reference at index
    size_t * dest = &(destination->head);
    for (size_t i = 0; i < index; ++i) {
        dest = destination->next + (*dest);
    }

    // push source elements into destinaton's hole indexes
    size_t src = source->head;
    for (; NIL != destination->empty && NIL != src; src = source->next[src]) {
        size_t const hole = destination->empty;
        destination->empty = destination->next[destination->empty];

        destination->next[hole] = (*dest);
        (*dest) = hole;

        memcpy(destination->elements + (hole * destination->size), source->elements + (src * source->size), destination->size);
        destination->length++;

        dest = destination->next + hole;
    }

    // push remaining source elements into destinaton list
    for (; NIL != src; src = source->next[src]) {
        size_t const hole = destination->length;

        destination->next[hole] = (*dest);
        (*dest) = hole;

        memcpy(destination->elements + (hole * destination->size), source->elements + (src * source->size), destination->size);
        destination->length++;

        dest = destination->next + hole;
    }

    // clear (NOT destroy) source list
    source->allocator->free(source->elements, source->allocator->arguments);
    source->allocator->free(source->next, source->allocator->arguments);

    source->elements = NULL;
    source->next = NULL;
    source->length = source->capacity = 0;
    source->head = source->empty = NIL;
}

istraight_list_s split_istraight_list(istraight_list_s * const list, size_t const index, size_t const length) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Index can't be more than or equal length.");
    error(length <= list->length && "Paremeter can't be greater than length.");
    error(index + length <= list->length && "Size can't be more than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // create split list structure
    size_t const split_mod = length % ISTRAIGHT_LIST_CHUNK;
    size_t const split_capacity = split_mod ? length - split_mod + ISTRAIGHT_LIST_CHUNK : length;
    istraight_list_s split = {
        .capacity = split_capacity, .empty = NIL, .head = NIL, .size = list->size,
        .allocator = list->allocator,
        .elements = list->allocator->alloc(split_capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(split_capacity * sizeof(size_t), list->allocator->arguments),
    };
    error((!split_capacity || split.elements) && "Memory allocation failed.");
    error((!split_capacity || split.next) && "Memory allocation failed.");

    // push list elements into split list and redirect removed element in list
    for (size_t * s = &(split.head); split.length < length; (*node) = list->next[(*node)], s = split.next + (*s)) {
        (*s) = split.length;
        split.next[(*s)] = NIL;

        memcpy(split.elements + (split.length * split.size), list->elements + ((*node) * list->size), list->size);
        split.length++;
    }

    // create replica list structure for list to remove holes
    size_t const replica_length = list->length - length;
    size_t const replica_mod = replica_length % ISTRAIGHT_LIST_CHUNK;
    size_t const replica_capacity = replica_mod ? replica_length - replica_mod + ISTRAIGHT_LIST_CHUNK : replica_length;
    istraight_list_s replica = {
        .capacity = replica_capacity, .empty = NIL, .head = NIL, .size = list->size,
        .allocator = list->allocator,
        .elements = list->allocator->alloc(replica_capacity * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(replica_capacity * sizeof(size_t), list->allocator->arguments),
    };
    error((!replica_capacity || replica.elements) && "Memory allocation failed.");
    error((!replica_capacity || replica.next) && "Memory allocation failed.");

    // restart node reference at list head and push its elements into replica
    node = &(list->head);
    for (size_t * r = &(replica.head); replica.length < replica_length; (*node) = list->next[(*node)], r = replica.next + (*r)) {
        (*r) = replica.length;
        replica.next[(*r)] = NIL;

        memcpy(replica.elements + (replica.length * replica.size), list->elements + ((*node) * list->size), list->size);
        replica.length++;
    }

    // replace list with its replica
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    (*list) = replica;

    return split;
}

istraight_list_s extract_istraight_list(istraight_list_s * const restrict list, filter_fn const filter, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    // create lists that contain true and false filtered elements
    istraight_list_s negative = { .empty = NIL, .head = NIL, .size = list->size, .allocator = list->allocator, };
    istraight_list_s positive = { .empty = NIL, .head = NIL, .size = list->size, .allocator = list->allocator, };

    // for each element in list check if filter returns true for current element and pushes it propper list
    size_t * neg = &(negative.head), * pos = &(positive.head);
    for (size_t i = list->head, pos_idx = 0, neg_idx = 0; NIL != i; i = list->next[i]) {
        char const * element = list->elements + (i * list->size);


        if (filter(element, arguments)) { // if filter value is positive push it into positive
            (*pos) = pos_idx;
            if (positive.length == positive.capacity) {
                _istraight_list_resize(&positive, positive.capacity + ISTRAIGHT_LIST_CHUNK);
            }
            positive.next[pos_idx] = NIL;

            memcpy(positive.elements + (pos_idx * positive.size), element, list->size);
            positive.length++;

            pos = positive.next + pos_idx;
            pos_idx++;
        } else { // else push it into negative
            (*neg) = neg_idx;
            if (negative.length == negative.capacity) {
                _istraight_list_resize(&negative, negative.capacity + ISTRAIGHT_LIST_CHUNK);
            }
            negative.next[neg_idx] = NIL;

            memcpy(negative.elements + (neg_idx * negative.size), element, list->size);
            negative.length++;

            neg = negative.next + neg_idx;
            neg_idx++;
        }
    }

    // free list and change it into negative while returning positive
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    (*list) = negative;

    return positive;
}

void map_istraight_list(istraight_list_s const * const restrict list, handle_fn const handle, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    for (size_t i = list->head; NIL != i && handle(list->elements + (i * list->size), arguments); i = list->next[i]) {}
}

void apply_istraight_list(istraight_list_s const * const restrict list, process_fn const process, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->length <= list->capacity && "Length exceeds capacity.");
    valid(list->allocator && "Allocator can't be NULL.");

    char * elements = list->allocator->alloc(list->length * list->size, list->allocator->arguments);
    error((!list->length || elements) && "Memory allocation failed.");

    for (size_t i = list->head, index = 0; NIL != i; i = list->next[i]) {
        memcpy(elements + (index * list->size), list->elements + (i * list->size), list->size);
        index++;
    }

    process(elements, list->length, arguments);

    for (size_t i = list->head, index = 0; NIL != i; i = list->next[i]) {
        memcpy(list->elements + (i * list->size), elements + (index * list->size), list->size);
        index++;
    }

    list->allocator->free(elements, list->allocator->arguments);
}

void _istraight_list_resize(istraight_list_s * const list, size_t const size) {
    list->capacity = size;

    // if list expands or hole stack is empty then just expand/shrink the list and return
    if (list->capacity != list->length || NIL == list->empty) {
        list->elements = list->allocator->realloc(list->elements, list->capacity * list->size, list->allocator->arguments);
        list->next = list->allocator->realloc(list->next, list->capacity * sizeof(size_t), list->allocator->arguments);

        error((!list->capacity || list->elements) && "Memory allocation failed.");
        error((!list->capacity || list->next) && "Memory allocation failed.");

        return;
    } // else copy elements into new array in order, and clear hole stack

    char * elements = list->allocator->alloc(list->capacity * list->size, list->allocator->arguments);
    error((!list->capacity || elements) && "Memory allocation failed.");

    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->next[current]) {
        memcpy(elements + (i * list->size), list->elements + (current * list->size), list->size);
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->elements = elements;

    list->next = list->allocator->realloc(list->next, list->capacity * sizeof(size_t), list->allocator->arguments);
    error((!list->capacity || list->next) && "Memory allocation failed.");

    for (size_t i = 0, * current = &(list->head); i < list->length; current = list->next + i, i++) {
        (*current) = i;
        list->next[i] = NIL;
    }

    list->empty = NIL;
}
