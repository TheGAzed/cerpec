#include <list/istraight_list.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL ((size_t)(-1))

void _istraight_list_resize(istraight_list_s * list);

istraight_list_s create_istraight_list(const size_t size) {
    return (istraight_list_s) { .head = NIL, .empty = NIL, .size = size };
}

void destroy_istraight_list(istraight_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; i != NIL; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }
    free(list->elements);
    free(list->next);

    memset(list, 0, sizeof(istraight_list_s)); // set everything to zero/invalid
}

void clear_istraight_list(istraight_list_s * list, const destroy_fn destroy) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each node and destroy its element
    for (size_t i = list->head; NIL != i; i = list->next[i]) {
        destroy(list->elements + (i * list->size));
    }
    free(list->elements);
    free(list->next);

    // clear (NOT destroy) list
    list->elements = NULL;
    list->next = NULL;
    list->length = list->capacity = 0;
    list->head = list->empty = NIL;
}

istraight_list_s copy_istraight_list(const istraight_list_s list, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    // create copy/replica list
    istraight_list_s replica = {
        .capacity = list.capacity, .empty = NIL, .head = NIL, .length = list.length, .size = list.size,
        .elements = malloc(list.capacity * list.size),
        .next = malloc(list.capacity * sizeof(size_t))
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // copy each element into replica (aslo make new list hole-less)
    for (size_t i = list.head, * r = &(replica.head), index = 0; NIL != i; i = list.next[i], r = replica.next + index, index++) {
        (*r) = index;
        replica.next[index] = NIL;

        copy(replica.elements + (index * replica.size), list.elements + (i * list.size));
    }

    return replica;
}

bool is_empty_istraight_list(const istraight_list_s list) {
    assert(list.size && "[INVALID] Size can't be zero.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    return !(list.length);
}

void insert_at_istraight_list(istraight_list_s * restrict list, const void * restrict element, const size_t index) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");
    assert(list != element && "[ERROR] Paremeters can't be equal.");
    assert(index <= list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // if list can't fit elements expand it
    if (list->length == list->capacity) {
        list->capacity += ISTRAIGHT_LIST_CHUNK;
        _istraight_list_resize(list);
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

void get_istraight_list(const istraight_list_s list, const size_t index, void * buffer) {
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(list.length && "[ERROR] List can't be empty.");
    assert(index < list.length && "[ERROR] Paremeter can't be greater than length.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    // iterate until node at index isn't reached
    size_t node = list.head;
    for (size_t i = 0; i < index; ++i) {
        node = list.next[node];
    }

    // copy element into buffer
    memcpy(buffer, list.elements + (node * list.size), list.size);
}

void remove_first_istraight_list(istraight_list_s * restrict list, const void * restrict element, void * restrict buffer, const compare_fn compare) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(compare && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Paremeter can't be zero.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // iterate until node with element isn't reached
    for (size_t * i = &(list->head); NIL != *i; i = list->next + (*i)) {
        const char * found = list->elements + ((*i) * list->size); // save found element
        if (0 != compare(element, found)) { // if element isn't equal continue
            continue;
        }

        // copy found element into buffer
        memcpy(buffer, found, list->size);
        list->length--;

        // save removed node as hole and redirect nodes
        const size_t hole = (*i);
        (*i) = list->next[(*i)];

        // push hole index onto stack (minimize holes by checking if node isn't the rightmost)
        if (hole != list->length) {
            list->next[hole] = list->empty;
            list->empty = hole;
        }

        // shrink list to save space if smaller capacity is available
        if (list->length == list->capacity - ISTRAIGHT_LIST_CHUNK) {
            list->capacity -= ISTRAIGHT_LIST_CHUNK;
            _istraight_list_resize(list);
        }

        return; // leave function with found element
    }

    assert(0 && "[ERROR] Element not found in list."); // if element is not found in list then that is an error
    // and exit failure is returned, since the function returns the removed element, element can contain allocated memory
    exit(EXIT_FAILURE);
}

void remove_at_istraight_list(istraight_list_s * restrict list, const size_t index, void * restrict buffer) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(list->length && "[ERROR] Paremeter can't be zero.");
    assert(index < list->length && "[ERROR] Paremeter can't be greater than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // copy found element into buffer
    memcpy(buffer, list->elements + ((*node) * list->size), list->size);
    list->length--;

    // save removed node as hole and redirect nodes
    const size_t hole = (*node);
    (*node) = list->next[(*node)];

    // push hole index onto stack (minimize holes by checking if node isn't the rightmost)
    if (hole != list->length) {
        list->next[hole] = list->empty;
        list->empty = hole;
    }

    // shrink list to save space if smaller capacity is available
    if (list->length == list->capacity - ISTRAIGHT_LIST_CHUNK) {
        list->capacity -= ISTRAIGHT_LIST_CHUNK;
        _istraight_list_resize(list);
    }
}

void reverse_istraight_list(istraight_list_s * list) {
    assert(list && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    size_t previous = NIL;
    for (size_t i = 0, current = list->head, next = NIL; i < list->length; ++i, previous = current, current = next) {
        next = list->next[current];
        list->next[current] = previous;
    }
    list->head = previous;
}

void splice_istraight_list(istraight_list_s * restrict destination, istraight_list_s * restrict source, const size_t index) {
    assert(destination && "[ERROR] Paremeter can't be NULL.");
    assert(source && "[ERROR] Paremeter can't be NULL.");
    assert(index <= destination->length && "[ERROR] Paremeter can't be greater than length.");

    assert(destination->size && "[INVALID] Size can't be zero.");
    assert(destination->length <= destination->capacity && "[INVALID] Length exceeds capacity.");
    assert(source->size && "[INVALID] Size can't be zero.");
    assert(source->length <= source->capacity && "[INVALID] Length exceeds capacity.");
    assert(source->size == destination->size && "[INVALID] Element sizes must be equal.");

    // calculate new destination length
    const size_t sum = destination->length + source->length;
    const size_t mod = sum % ISTRAIGHT_LIST_CHUNK;
    destination->capacity = mod ? sum - mod + ISTRAIGHT_LIST_CHUNK : sum;
    _istraight_list_resize(destination);

    // go to destination node reference at index
    size_t * dest = &(destination->head);
    for (size_t i = 0; i < index; ++i) {
        dest = destination->next + (*dest);
    }

    // push source elements into destinaton's hole indexes
    size_t src = source->head;
    for (; NIL != destination->empty && NIL != src; src = source->next[src]) {
        const size_t hole = destination->empty;
        destination->empty = destination->next[destination->empty];

        destination->next[hole] = (*dest);
        (*dest) = hole;

        memcpy(destination->elements + (hole * destination->size), source->elements + (src * source->size), destination->size);
        destination->length++;

        dest = destination->next + hole;
    }

    // push remaining source elements into destinaton list
    for (; NIL != src; src = source->next[src]) {
        const size_t hole = destination->length;

        destination->next[hole] = (*dest);
        (*dest) = hole;

        memcpy(destination->elements + (hole * destination->size), source->elements + (src * source->size), destination->size);
        destination->length++;

        dest = destination->next + hole;
    }

    // clear (NOT destroy) source list
    free(source->elements);
    free(source->next);

    source->elements = NULL;
    source->next = NULL;
    source->length = source->capacity = 0;
    source->head = source->empty = NIL;
}

istraight_list_s split_istraight_list(istraight_list_s * list, const size_t index, const size_t length) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(index < list->length && "[ERROR] Index can't be more than or equal length.");
    assert(index + length <= list->length && "[ERROR] Size can't be more than length.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    // go to node reference at index
    size_t * node = &(list->head);
    for (size_t i = 0; i < index; ++i) {
        node = list->next + (*node);
    }

    // create split list structure
    const size_t split_mod = length % ISTRAIGHT_LIST_CHUNK;
    const size_t split_capacity = split_mod ? length - split_mod + ISTRAIGHT_LIST_CHUNK : length;
    istraight_list_s split = {
        .capacity = split_capacity, .empty = NIL, .head = NIL, .size = list->size,

        .elements = malloc(split_capacity * list->size),
        .next = malloc(split_capacity * sizeof(size_t)),
    };
    assert((!split_capacity || split.elements) && "[ERROR] Memory allocation failed.");
    assert((!split_capacity || split.next) && "[ERROR] Memory allocation failed.");

    // push list elements into split list and redirect removed element in list
    for (size_t * s = &(split.head); split.length < length; (*node) = list->next[(*node)], s = split.next + (*s)) {
        (*s) = split.length;
        split.next[(*s)] = NIL;

        memcpy(split.elements + (split.length * split.size), list->elements + ((*node) * list->size), list->size);
        split.length++;
    }

    // create replica list structure for list to remove holes
    const size_t replica_length = list->length - length;
    const size_t replica_mod = replica_length % ISTRAIGHT_LIST_CHUNK;
    const size_t replica_capacity = replica_mod ? replica_length - replica_mod + ISTRAIGHT_LIST_CHUNK : replica_length;
    istraight_list_s replica = {
        .capacity = replica_capacity, .empty = NIL, .head = NIL, .size = list->size,

        .elements = malloc(replica_capacity * list->size),
        .next = malloc(replica_capacity * sizeof(size_t)),
    };
    assert((!replica_capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica_capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // restart node reference at list head and push its elements into replica
    node = &(list->head);
    for (size_t * r = &(replica.head); replica.length < replica_length; (*node) = list->next[(*node)], r = replica.next + (*r)) {
        (*r) = replica.length;
        replica.next[(*r)] = NIL;

        memcpy(replica.elements + (replica.length * replica.size), list->elements + ((*node) * list->size), list->size);
        replica.length++;
    }

    // replace list with its replica
    free(list->elements);
    free(list->next);

    (*list) = replica;

    return split;
}

istraight_list_s extract_istraight_list(istraight_list_s * list, const filter_fn filter, void * arguments) {
    assert(list && "[ERROR] Paremeter can't be NULL.");
    assert(filter && "[ERROR] Paremeter can't be NULL.");

    assert(list->size && "[INVALID] Size can't be zero.");
    assert(list->length <= list->capacity && "[INVALID] Length exceeds capacity.");

    istraight_list_s negative = { .empty = NIL, .head = NIL, .size = list->size, };
    istraight_list_s positive = { .empty = NIL, .head = NIL, .size = list->size, };

    size_t * neg = &(negative.head), * pos = &(positive.head);
    for (size_t i = list->head, pos_idx = 0, neg_idx = 0; NIL != i; i = list->next[i]) {
        const char * element = list->elements + (i * list->size);

        if (filter(element, arguments)) {
            (*pos) = pos_idx;
            if (positive.length == positive.capacity) {
                positive.capacity += ISTRAIGHT_LIST_CHUNK;
                _istraight_list_resize(&positive);
            }
            positive.next[pos_idx] = NIL;

            memcpy(positive.elements + (pos_idx * positive.size), element, list->size);
            positive.length++;

            pos = positive.next + pos_idx;
            pos_idx++;
        } else {
            (*neg) = neg_idx;
            if (negative.length == negative.capacity) {
                negative.capacity += ISTRAIGHT_LIST_CHUNK;
                _istraight_list_resize(&negative);
            }
            negative.next[neg_idx] = NIL;

            memcpy(negative.elements + (neg_idx * negative.size), element, list->size);
            negative.length++;

            neg = negative.next + neg_idx;
            neg_idx++;
        }
    }
    free(list->elements);
    free(list->next);

    (*list) = negative;

    return positive;
}

void foreach_istraight_list(const istraight_list_s list, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    for (size_t i = list.head; NIL != i && operate(list.elements + (i * list.size), arguments); i = list.next[i]) {}
}

void map_istraight_list(const istraight_list_s list, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(list.size && "[INVALID] Size can't be zero.");
    assert(list.length <= list.capacity && "[INVALID] Length exceeds capacity.");

    char * elements = malloc(list.length * list.size);
    assert((!list.length || elements) && "[ERROR] Memory allocation failed.");

    for (size_t i = list.head, index = 0; NIL != i; i = list.next[i]) {
        memcpy(elements + (index * list.size), list.elements + (i * list.size), list.size);
        index++;
    }

    manage(elements, list.length, arguments);

    for (size_t i = list.head, index = 0; NIL != i; i = list.next[i]) {
        memcpy(list.elements + (i * list.size), elements + (index * list.size), list.size);
        index++;
    }

    free(elements);
}

void _istraight_list_resize(istraight_list_s * list) {
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

    for (size_t i = 0, current = list->head; i < list->length; ++i, current = list->next[current]) {
        memcpy(elements + (i * list->size), list->elements + (current * list->size), list->size);
    }
    free(list->elements);
    list->elements = elements;

    list->next = realloc(list->next, list->capacity * sizeof(size_t));
    assert((!list->capacity || list->next) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0, * current = &(list->head); i < list->length; current = list->next + i, i++) {
        (*current) = i;
        list->next[i] = NIL;
    }

    list->empty = NIL;
}
