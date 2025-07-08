#include <sequence/deque.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

deque_s create_deque(const size_t size) {
    assert(size && "[ERROR] Size can't be zero.");

    return (deque_s) { .size = size };
}

void destroy_deque(deque_s * deque, const destroy_fn destroy) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(destroy && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct deque_node * current = deque->head; // save head index as current node
    for (size_t start = deque->current; deque->length; start = 0) { // for every node in deque list until size is not zero
        size_t i = start; // set i outside of nested for loop to save its value
        for (; i < (deque->length + start) && i < DEQUE_CHUNK; ++i) { // destroy each element in node
            destroy(current->elements + (i * deque->size), deque->size);
        }
        deque->length -= (i - start); // subtract absolute value of i and start from deque's size

        struct deque_node * temp = current; // save current node as temporary to free later
        current = current->next; // go to next node
        free(temp); // free temporary current node
    }

    memset(deque, 0, sizeof(deque_s)); // set everything to zero
}

void clear_deque(deque_s * deque, const destroy_fn destroy) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(destroy && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct deque_node * current = deque->head; // save head index as current node
    for (size_t start = deque->current; deque->length; start = 0) { // for every node in deque list until size is not zero
        size_t i = start; // set i outside of nested for loop to save its value
        for (; i < (deque->length + start) && i < DEQUE_CHUNK; ++i) { // destroy each element in node
            destroy(current->elements + (i * deque->size), deque->size);
        }
        deque->length -= (i - start); // subtract absolute value of i and start from deque's size

        struct deque_node * temp = current; // save current node as temporary to free later
        current = current->next; // go to next node
        free(temp); // free temporary current node
    }

    deque->head = NULL;
    deque->current = 0;
}

deque_s copy_deque(const deque_s deque, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter is NULL.");

    assert(deque.size && "[INVALID] Size can't be zero.");

    deque_s replika = { .current = deque.current, .head = NULL, .size = deque.size, .length = deque.length };

    struct deque_node const * current_deque = deque.head; // save head index as current node
    struct deque_node ** current_replika = &(replika.head);
    size_t remaining = deque.length; // save remaining size as parameter to decrement for loop
    for (size_t start = deque.current; remaining; start = 0) { // while remaining size is not zero
        struct deque_node * node = malloc(sizeof(struct deque_node));
        assert(node && "[ERROR] Memory allocation failed.");

        size_t i = start;
        for (; i < remaining + start && i < DEQUE_CHUNK; ++i) { // operate on each element in node
            copy(node->elements + (i * deque.size), current_deque->elements + (i * deque.size), deque.size);
        }

        (*current_replika) = node->prev = node; // *current_copy and node's prev will point to node
        node->next = replika.head; // node's next points to head (if *current_copy is head then node's next points to node)
        // node's prev points to list_copy.head's prev (if *current_copy is head then head/node->prev is node, else node->prev is previous node)
        node->prev = replika.head->prev;
        replika.head->prev = node; // head's prev points to node (if *current_copy is head then head/node prev is node, else head prev is last node)

        remaining -= (i - start); // decrement remaining size by operated size of current node

        current_replika = &((*current_replika)->next); // go to next's node pointer
        current_deque = current_deque->next; // go to next node
    }

    return replika;
}

bool is_empty_deque(const deque_s deque) {
    assert(deque.size && "[INVALID] Size can't be zero.");

    return (deque.length == 0);
}

void enqueue_front(deque_s * deque, const void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    if (!(deque->current)) { // if deque's previous current 'underflows' in node array due to inserting element to front
        deque->current = DEQUE_CHUNK; // make current into list array chunk size to prevent future underflow

        struct deque_node * node = malloc(sizeof(struct deque_node));
        assert(node && "[ERROR] Memory allocation failed.");

        if (deque->head) { // if head exists
            node->next = deque->head; // node's next is head
            node->prev = deque->head->prev; // node's previous is tail/head's previous

            deque->head = deque->head->prev = node; // head's previous is node and node becomes head
        } else { // else head does not exist
            deque->head = node->next = node->prev = node; // node's next and previous is node and node becomes head
        }
    }

    deque->length++; // increment size for new element insertion
    deque->current--; // if current was 0 then current will be 'LIST_ARRAY_DEQUE_CHUNK - 1'
    memcpy(deque->head->elements + (deque->current * deque->size), &buffer, deque->size); // copy element into deque's head
}

void enqueue_back(deque_s * deque, const void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    const size_t next_index = ((deque->current + deque->length) % DEQUE_CHUNK);
    if (!next_index) { // if next index to insert into is zero
        struct deque_node * node = malloc(sizeof(struct deque_node));
        assert(node && "[ERROR] Memory allocation failed.");

        if (deque->head) { // if head exists
            node->next = deque->head; // node's next is head
            node->prev = deque->head->prev; // node's previous is tail/head's previous

            deque->head->prev = deque->head->prev->next = node; // node is tail's next and head's previous, and node becomes tail
        } else { // else head does not exist
            deque->head = node->next = node->prev = node; // node's next and previous is node and node becomes head
        }
    }

    deque->length++; // increment size for new element insertion
    memcpy(deque->head->prev->elements + (next_index * deque->size), &buffer, deque->size); // copy element into deque's tail
}

void peek_front(const deque_s deque, void * buffer) {
    assert(deque.length && "[ERROR] Can't peek empty deque.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque.head && "[INVALID] Head can't be NULL.");
    assert(deque.size && "[INVALID] Size can't be zero.");

    memcpy(buffer, deque.head->elements + (deque.current * deque.size), deque.size);
}

void peek_back(const deque_s deque, void * buffer) {
    assert(deque.length && "[ERROR] Can't peek empty deque.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque.head && "[INVALID] Head can't be NULL.");
    assert(deque.head->prev && "[INVALID] Head's prev (tail) can't be NULL.");
    assert(deque.size && "[INVALID] Size can't be zero.");

    const size_t back_index = (deque.current + deque.length - 1) % DEQUE_CHUNK;
    memcpy(buffer, deque.head->prev->elements + (back_index * deque.size), deque.size);
}

void dequeue_front(deque_s * deque, void * buffer) {
    assert(deque->length && "[ERROR] Can't dequeue empty deque.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    memcpy(buffer, deque->head->elements + (deque->current * deque->size), deque->size);
    deque->current++; // increment current index since it's removing from front (like a queue)
    deque->length--; // decrement deque's size since front elements gets removed (like a queue)
    // if deque's current index is equal to list chunk or deque is empty free head node
    if ((DEQUE_CHUNK == deque->current) || !(deque->length)) {
        struct deque_node * empty_head = deque->head; // temporary save pointer to head node

        deque->head->next->prev = deque->head->prev; // set head next's previous pointer to tail/head's previous
        deque->head->prev->next = deque->head->next; // set tail's next node to head's next node

        deque->head = deque->length ? deque->head->next : NULL; // if deque's size is zero then set head to NULL, else it's head's next node
        free(empty_head); // free temporary saved head node
        deque->current = 0; // reset current index to zero/beginning
    }
}

void dequeue_back(deque_s * deque, void * buffer) {
    assert(deque->length && "[ERROR] Can't dequeue empty deque.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->head->prev && "[INVALID] Head's prev (tail) can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    const size_t back_index = (deque->current + deque->length - 1) % DEQUE_CHUNK; // calculate back element's index
    memcpy(buffer, deque->head->prev->elements + (back_index * deque->size), deque->size);
    deque->length--; // decrement only size since it's removing from the back (like a stack)

    if (!back_index || !(deque->length)) { // if current index at back is zero or deque is empty remove empty tail/head
        struct deque_node * empty_tail = deque->head->prev; // temporary save pointer to head node

        deque->head->prev = deque->head->prev->prev; // head's tail pointer equals tail's previous
        deque->head->prev->prev->next = deque->head; // tail previous' next equals head

        if (!deque->length) { // if deque's size is zero then make its head NULL
            deque->head = NULL;
        }

        free(empty_tail); // free temporary saved tail node
    }
}

void foreach_front_deque(deque_s const * deque, const operate_fn operate, void * args) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(operate && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct deque_node * current = deque->head; // save head node as current pointer
    size_t remaining = deque->length; // save remaining size as parameter to decrement for loop
    for (size_t start = deque->current; remaining; start = 0) { // while remaining size is not zero
        size_t i = start;
        for (; i < remaining && i < DEQUE_CHUNK; ++i) { // operate on each element in node
            // if operate returns false then terminate main function
            if (!operate(current->elements + (i * deque->size), deque->size, args)) {
                return;
            }
        }

        remaining -= (i - start); // decrement remaining size by operated size of current node
        current = current->next; // go to next node
    }
}

void foreach_back_deque(deque_s const * deque, const operate_fn operate, void * args) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(operate && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct deque_node * current = deque->head; // save head node as current pointer
    size_t remaining = deque->length; // save remaining size as parameter to decrement for loop
    size_t end = (deque->current + deque->length) % DEQUE_CHUNK; // calculate end index
    // for loop starts from tail backwards until remaining size starting from current index is not less than chunk size
    for (;(deque->current + remaining) >= DEQUE_CHUNK; end = DEQUE_CHUNK) {
        current = current->prev; // early go to previous node to start from tail

        size_t i = 0; // extract i index to use in subtraction
        // while i is less than the smaller size
        for (const size_t smaller = end < remaining ? end : remaining; i < smaller; ++i) {
            if (!operate(current->elements + ((smaller - i - 1) * deque->size), deque->size, args)) {
                return;
            }
        }

        remaining -= i;
    }

    for (size_t i = 0; i < remaining; ++i) { // if remaining size remains at head node
        // operate on current/head node
        const size_t reverse = (remaining - i - 1) + deque->current;
        if (!operate(current->elements + (reverse * deque->size), deque->size, args)) {
            return;
        }
    }
}

void map_deque(deque_s const * deque, const manage_fn manage, void * arguments) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(manage && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    char * elements_array = malloc(deque->length * deque->size);
    assert(elements_array && "[ERROR] Memory alloction failed.");

    size_t index = 0, remaining = deque->length;
    struct deque_node * current = deque->head;
    for (size_t start = deque->current; remaining; start = 0) {
        size_t i = start;
        for (; i < remaining && i < DEQUE_CHUNK; ++i) {
            memcpy(elements_array + (index * deque->size), current->elements + (i * deque->size), deque->size);
            index++;
        }

        remaining -= (i - start);
        current = current->next;
    }

    manage(elements_array, deque->length, deque->size, arguments);

    index = 0, remaining = deque->length, current = deque->head;
    for (size_t start = deque->current; remaining; start = 0) {
        size_t i = start;
        for (; i < remaining && i < DEQUE_CHUNK; ++i) {
            memcpy(current->elements + (i * deque->size), elements_array + (index * deque->size), deque->size);
            index++;
        }

        remaining -= (i - start);
        current = current->next;
    }

    free(elements_array);
}
