#include <list/fstraight_list.h>

#include <stdlib.h> // import exit()
#include <string.h>

#define NIL ((size_t)(-1))

fstraight_list_s create_fstraight_list(size_t const size, size_t const max) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");

    fstraight_list_s const list = {
        .allocator = &standard, .max = max, .size = size,
        .elements = standard.alloc(max * size, standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
        .head = NIL, .empty = NIL,
    };
    error(list.elements && "Memory allocation failed.");
    error(list.next && "Memory allocation failed.");

    return list;
}

fstraight_list_s make_fstraight_list(size_t const size, size_t const max, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    fstraight_list_s const list = {
        .allocator = allocator, .max = max, .size = size,
        .elements = allocator->alloc(max * size, allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .head = NIL, .empty = NIL,
    };
    error(list.elements && "Memory allocation failed.");
    error(list.next && "Memory allocation failed.");

    return list;
}

void destroy_fstraight_list(fstraight_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; i != NIL; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    memset(list, 0, sizeof(fstraight_list_s));
}

void clear_fstraight_list(fstraight_list_s * const list, set_fn const destroy) {
    error(list && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; NIL != i; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }

    // clear (NOT destroy) list
    list->length = 0;
    list->head = list->empty = NIL;
}

fstraight_list_s copy_fstraight_list(fstraight_list_s const * const list, copy_fn const copy) {
    error(list && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // create copy/replica list
    fstraight_list_s replica = {
        .max = list->max, .empty = NIL, .head = NIL, .length = list->length, .size = list->size,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error(replica.elements && "Memory allocation failed.");
    error(replica.next && "Memory allocation failed.");

    // copy each element into replica (aslo make new list hole-less)
    for (size_t i = list->head, * r = &(replica.head), index = 0; NIL != i; i = list->next[i], r = replica.next + index, index++) {
        (*r) = index;
        replica.next[index] = NIL;

        copy(replica.elements + (index * replica.size), list->elements + (i * list->size));
    }

    return replica;
}

bool is_empty_fstraight_list(fstraight_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    return !(list->length);
}

bool is_full_fstraight_list(fstraight_list_s const * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    return (list->length == list->max);
}

void insert_at_fstraight_list(fstraight_list_s * const restrict list, void const * const restrict element, size_t const index) {
    error(list && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(list != element && "Paremeters can't be equal.");
    error(index <= list->length && "Paremeter can't be greater than length.");
    error(list != element && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

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

void get_fstraight_list(fstraight_list_s const * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "List can't be empty.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // iterate until node at index isn't reached
    size_t node = list->head;
    for (size_t i = 0; i < index; ++i) {
        node = list->next[node];
    }

    // copy element into buffer
    memcpy(buffer, list->elements + (node * list->size), list->size);
}

void remove_first_fstraight_list(fstraight_list_s * const restrict list, void const * const restrict element, void * const restrict buffer, compare_fn const compare) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(compare && "Paremeter can't be NULL.");
    error(list->length && "Paremeter can't be zero.");
    error(list != element && "Parameters can't be equal.");
    error(list != buffer && "Parameters can't be equal.");
    error(element != buffer && "Paremeters can't be the same.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

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

        if (!list->length) {
            list->empty = NIL;
        }

        return; // leave function with found element
    }

    error(0 && "Element not found in list."); // if element is not found in list then that is an error
    // and exit failure is returned, since the function returns the removed element, element can contain allocated memory
    exit(EXIT_FAILURE);
}

void remove_at_fstraight_list(fstraight_list_s * const restrict list, size_t const index, void * const restrict buffer) {
    error(list && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(list->length && "Paremeter can't be zero.");
    error(index < list->length && "Paremeter can't be greater than length.");
    error(list != buffer && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

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

    // reset empty stack if list is empty since holes don't matter atp
    if (!list->length) {
        list->empty = NIL;
    }
}

void reverse_fstraight_list(fstraight_list_s * const list) {
    error(list && "Paremeter can't be NULL.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    size_t previous = NIL;
    for (size_t i = 0, current = list->head, next = NIL; i < list->length; ++i, previous = current, current = next) {
        next = list->next[current];
        list->next[current] = previous;
    }
    list->head = previous;
}

void splice_fstraight_list(fstraight_list_s * const restrict destination, fstraight_list_s * const restrict source, size_t const index) {
    error(destination && "Paremeter can't be NULL.");
    error(source && "Paremeter can't be NULL.");
    error(index <= destination->length && "Paremeter can't be greater than length.");
    error(destination->length + source->length <= destination->max && "Spliced length exceeds maximum.");
    error(source->size == destination->size && "Element sizes must be equal.");
    error(destination != source && "Parameters can't be equal.");

    valid(destination->size && "Size can't be zero.");
    valid(destination->max && "Maximum can't be zero.");
    valid(destination->allocator && "Allocator can't be NULL.");
    valid(destination->length <= destination->max && "Length exceeds maximum.");

    valid(source->size && "Size can't be zero.");
    valid(source->max && "Maximum can't be zero.");
    valid(source->allocator && "Allocator can't be NULL.");
    valid(source->length <= source->max && "Length exceeds maximum.");

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

    // stack-based push remaining source elements into destinaton list
    for (; NIL != src; src = source->next[src]) {
        size_t const hole = destination->length;

        destination->next[hole] = (*dest);
        (*dest) = hole;

        memcpy(destination->elements + (hole * destination->size), source->elements + (src * source->size), destination->size);
        destination->length++;

        dest = destination->next + hole;
    }

    // clear (NOT destroy) source list
    source->length = 0;
    source->head = source->empty = NIL;
}

fstraight_list_s split_fstraight_list(fstraight_list_s * const list, size_t const index, size_t const length) {
    error(list && "Paremeter can't be NULL.");
    error(index < list->length && "Index can't be more than or equal length.");
    error(length <= list->length && "Paremeter can't be greater than length.");
    error(index + length <= list->length && "Size can't be more than length.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // create split list structure
    fstraight_list_s split = {
        .max = list->max, .empty = NIL, .head = NIL, .size = list->size,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };
    error(split.elements && "Memory allocation failed.");
    error(split.next && "Memory allocation failed.");

    // push list elements into split list and redirect removed element in list
    for (size_t * s = &(split.head); split.length < length; (*node) = list->next[(*node)], s = split.next + (*s)) {
        (*s) = split.length;
        split.next[(*s)] = NIL;

        memcpy(split.elements + (split.length * split.size), list->elements + ((*node) * list->size), list->size);
        split.length++;
    }

    // create replica list structure for list to remove holes
    fstraight_list_s replica = {
        .max = list->max, .empty = NIL, .head = NIL, .size = list->size,
        .allocator = list->allocator,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
    };
    error(replica.elements && "Memory allocation failed.");
    error(replica.next && "Memory allocation failed.");

    // restart node reference at list head and push its elements into replica
    size_t const replica_length = list->length - length;
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

fstraight_list_s extract_fstraight_list(fstraight_list_s * const restrict list, filter_fn const filter, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(filter && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // create lists that will contain positive and negative filtered elements
    fstraight_list_s negative = {
        .empty = NIL, .head = NIL, .size = list->size, .max = list->max,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };

    fstraight_list_s positive = {
        .empty = NIL, .head = NIL, .size = list->size, .max = list->max,
        .elements = list->allocator->alloc(list->max * list->size, list->allocator->arguments),
        .next = list->allocator->alloc(list->max * sizeof(size_t), list->allocator->arguments),
        .allocator = list->allocator,
    };

    // for each element in list check if filter returns true for current element and pushes it propper list
    size_t * neg = &(negative.head), * pos = &(positive.head);
    for (size_t i = list->head, pos_idx = 0, neg_idx = 0; NIL != i; i = list->next[i]) {
        char const * element = list->elements + (i * list->size);

        if (filter(element, arguments)) { // if filter value is positive push it into positive
            (*pos) = pos_idx;
            positive.next[pos_idx] = NIL;

            memcpy(positive.elements + (pos_idx * positive.size), element, list->size);
            positive.length++;

            pos = positive.next + pos_idx;
            pos_idx++;
        } else { // else push it into negative
            (*neg) = neg_idx;
            negative.next[neg_idx] = NIL;

            memcpy(negative.elements + (neg_idx * negative.size), element, list->size);
            negative.length++;

            neg = negative.next + neg_idx;
            neg_idx++;
        }
    }

    // replace list with its negative
    list->allocator->free(list->elements, list->allocator->arguments);
    list->allocator->free(list->next, list->allocator->arguments);

    (*list) = negative;

    return positive;
}

void each_fstraight_list(fstraight_list_s const * const restrict list, handle_fn const handle, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    for (size_t i = list->head; NIL != i && handle(list->elements + (i * list->size), arguments); i = list->next[i]) {}
}

void apply_fstraight_list(fstraight_list_s const * const restrict list, process_fn const process, void * const restrict arguments) {
    error(list && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(list != arguments && "Parameters can't be equal.");

    valid(list->size && "Size can't be zero.");
    valid(list->max && "Maximum can't be zero.");
    valid(list->allocator && "Allocator can't be NULL.");
    valid(list->length <= list->max && "Length exceeds maximum.");

    // create temporary array to contain scattered elements
    char * elements_array = list->allocator->alloc(list->length * list->size, list->allocator->arguments);
    error((!list->length || elements_array) && "Memory allocation failed.");

    // copy scattered elements_array into elements_array array
    for (size_t i = list->head, index = 0; NIL != i; i = list->next[i]) {
        memcpy(elements_array + (index * list->size), list->elements + (i * list->size), list->size);
        index++;
    }

    process(elements_array, list->length, arguments);

    for (size_t i = list->head, index = 0; NIL != i; i = list->next[i]) {
        memcpy(list->elements + (i * list->size), elements_array + (index * list->size), list->size);
        index++;
    }

    list->allocator->free(elements_array, list->allocator->arguments);
}
