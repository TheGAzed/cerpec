#include <sequence/ideque.h>

#include <assert.h>
#include <string.h>

ideque_s create_ideque(const size_t size) {
    assert(size && "[ERROR] Size can't be zero.");

    return (ideque_s) { .size = size, .allocator = &standard, };
}

void destroy_ideque(ideque_s * deque, const destroy_fn destroy) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(destroy && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct infinite_deque_node * current = deque->head; // save head index as current node
    for (size_t start = deque->current; deque->length; start = 0) { // for every node in deque list until size is not zero
        size_t i = start; // set i outside of nested for loop to save its value
        for (; i < (deque->length + start) && i < IDEQUE_CHUNK; ++i) { // destroy each element in node
            destroy(current->elements + (i * deque->size));
        }
        deque->length -= (i - start); // subtract absolute value of i and start from deque's size

        struct infinite_deque_node * temp = current; // save current node as temporary to free later
        current = current->next; // go to next node

        deque->allocator->free(temp->elements, deque->allocator->arguments); // free temporary current node elements
        deque->allocator->free(temp, deque->allocator->arguments); // free temporary current node
    }

    // set everything to zero
    deque->current = deque->length = deque->size = 0;
    deque->head = NULL;
    deque->allocator = NULL;
}

void clear_ideque(ideque_s * deque, const destroy_fn destroy) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(destroy && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct infinite_deque_node * current = deque->head; // save head index as current node
    for (size_t start = deque->current; deque->length; start = 0) { // for every node in deque list until size is not zero
        size_t i = start; // set i outside of nested for loop to save its value
        for (; i < (deque->length + start) && i < IDEQUE_CHUNK; ++i) { // destroy each element in node
            destroy(current->elements + (i * deque->size));
        }
        deque->length -= (i - start); // subtract absolute value of i and start from deque's size

        struct infinite_deque_node * temp = current; // save current node as temporary to free later
        current = current->next; // go to next node

        deque->allocator->free(temp->elements, deque->allocator->arguments); // free temporary current node elements
        deque->allocator->free(temp, deque->allocator->arguments); // free temporary current node
    }

    deque->current = deque->length = 0;
    deque->head = NULL;
}

ideque_s copy_ideque(const ideque_s * deque, const copy_fn copy) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(copy && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    ideque_s replika = { .current = deque->current, .head = NULL, .size = deque->size, .length = deque->length };

    struct infinite_deque_node const * current_deque = deque->head; // save head index as current node
    struct infinite_deque_node ** current_replika = &(replika.head);
    size_t remaining = deque->length; // save remaining size as parameter to decrement for loop
    for (size_t start = deque->current; remaining; start = 0) { // while remaining size is not zero
        // allocate node for replica
        struct infinite_deque_node * node = deque->allocator->alloc(sizeof(struct infinite_deque_node), deque->allocator->arguments);
        assert(node && "[ERROR] Memory allocation failed.");

        // allocate elements array for replica
        node->elements = deque->allocator->alloc(IDEQUE_CHUNK * deque->size, deque->allocator->arguments);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        // copy each element from old to replica
        size_t i = start;
        for (; i < (remaining + start) && i < IDEQUE_CHUNK; ++i) { // operate on each element in node
            copy(node->elements + (i * deque->size), current_deque->elements + (i * deque->size));
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

bool is_empty_ideque(const ideque_s * deque) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    return (deque->length == 0);
}

void enqueue_front_ideque(ideque_s * deque, const void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    if (!(deque->current)) { // if deque's previous current 'underflows' in node array due to inserting element to front
        deque->current = IDEQUE_CHUNK; // make current into list array chunk size to prevent future underflow

        // allocate node for replica
        struct infinite_deque_node * node = deque->allocator->alloc(sizeof(struct infinite_deque_node), deque->allocator->arguments);
        assert(node && "[ERROR] Memory allocation failed.");

        // allocate elements array for replica
        node->elements = deque->allocator->alloc(IDEQUE_CHUNK * deque->size, deque->allocator->arguments);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        if (deque->head) { // if head exists
            node->next = deque->head; // node's next is head
            node->prev = deque->head->prev; // node's previous is tail/head's previous

            deque->head = deque->head->prev = deque->head->prev->next = node;
        } else { // else head does not exist
            deque->head = node->next = node->prev = node; // node's next and previous is node and node becomes head
        }
    }

    deque->length++; // increment size for new element insertion
    deque->current--; // if current was 0 then current will be 'LIST_ARRAY_DEQUE_CHUNK - 1'
    memcpy(deque->head->elements + (deque->current * deque->size), buffer, deque->size); // copy element into deque's head
}

void enqueue_back_ideque(ideque_s * deque, const void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    const size_t next_index = ((deque->current + deque->length) % IDEQUE_CHUNK);
    if (!next_index) { // if next index to insert into is zero
        struct infinite_deque_node * node = deque->allocator->alloc(sizeof(struct infinite_deque_node), deque->allocator->arguments);
        assert(node && "[ERROR] Memory allocation failed.");

        node->elements = deque->allocator->alloc(IDEQUE_CHUNK * deque->size, deque->allocator->arguments);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        if (deque->head) { // if head exists
            node->next = deque->head; // node's next is head
            node->prev = deque->head->prev; // node's previous is tail/head's previous

            deque->head->prev = deque->head->prev->next = node; // node is tail's next and head's previous, and node becomes tail
        } else { // else head does not exist
            deque->head = node->next = node->prev = node; // node's next and previous is node and node becomes head
        }
    }

    deque->length++; // increment size for new element insertion
    memcpy(deque->head->prev->elements + (next_index * deque->size), buffer, deque->size); // copy element into deque's tail
}

void peek_front_ideque(const ideque_s * deque, void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");

    assert(deque->length && "[ERROR] Can't peek empty structure.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    memcpy(buffer, deque->head->elements + (deque->current * deque->size), deque->size);
}

void peek_back_ideque(const ideque_s * deque, void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(deque->length && "[ERROR] Can't peek empty structure.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->head->prev && "[INVALID] Head's prev (tail) can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    // calculate back element's location/index and copy it into buffer
    const size_t back_index = (deque->current + deque->length - 1) % IDEQUE_CHUNK;
    memcpy(buffer, deque->head->prev->elements + (back_index * deque->size), deque->size);
}

void dequeue_front_ideque(ideque_s * deque, void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(deque->length && "[ERROR] Can't dequeue empty structure.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    memcpy(buffer, deque->head->elements + (deque->current * deque->size), deque->size);
    deque->current++; // increment current index since it's removing from front (like a queue)
    deque->length--; // decrement deque's size since front elements gets removed (like a queue)
    // if deque's current index is equal to list chunk or deque is empty free head node
    if ((IDEQUE_CHUNK == deque->current) || !(deque->length)) {
        struct infinite_deque_node * head = deque->head; // temporary save pointer to head node

        deque->head->next->prev = deque->head->prev; // set head next's previous pointer to tail/head's previous
        deque->head->prev->next = deque->head->next; // set tail's next node to head's next node

        // if deque's size is zero then set head to NULL, else it's head's next node
        deque->head = deque->length ? deque->head->next : NULL;
        deque->current = 0; // reset current index to zero/beginning

        deque->allocator->free(head->elements, deque->allocator->arguments); // free temporary head elements
        deque->allocator->free(head, deque->allocator->arguments); // free temporary head node
    }
}

void dequeue_back_ideque(ideque_s * deque, void * buffer) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(deque->length && "[ERROR] Can't dequeue empty structure.");
    assert(buffer && "[ERROR] Parameter is NULL.");

    assert(deque->head && "[INVALID] Head can't be NULL.");
    assert(deque->head->prev && "[INVALID] Head's prev (tail) can't be NULL.");
    assert(deque->size && "[INVALID] Size can't be zero.");

    deque->length--; // decrement only size since it's removing from the back
    const size_t back_index = (deque->current + deque->length) % IDEQUE_CHUNK; // calculate back element's index
    memcpy(buffer, deque->head->prev->elements + (back_index * deque->size), deque->size);

    if (!deque->length) {
        deque->allocator->free(deque->head->elements, deque->allocator->arguments); // free head elements
        deque->allocator->free(deque->head, deque->allocator->arguments); // free head node

        deque->current = 0; // reset current index to 0 if deque is empty
        deque->head = NULL;
    } else if (!back_index) { // if current index at back is zero or deque is empty remove empty tail/head
        struct infinite_deque_node * tail = deque->head->prev; // temporary save pointer to head node

        deque->head->prev = tail->prev; // head's tail pointer equals tail's previous
        tail->prev->next = deque->head; // tail previous' next equals head

        deque->allocator->free(tail->elements, deque->allocator->arguments); // free temporary tail elements
        deque->allocator->free(tail, deque->allocator->arguments); // free temporary tail node
    }
}

void map_front_ideque(const ideque_s * deque, const handle_fn operate, void * args) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(operate && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    struct infinite_deque_node const * current = deque->head; // save head node as current pointer
    size_t remaining = deque->length; // save remaining size as parameter to decrement for loop
    for (size_t start = deque->current; remaining; start = 0) { // while remaining size is not zero
        size_t i = start;
        for (; i < (remaining + start) && i < IDEQUE_CHUNK; ++i) { // operate on each element in node
            // if operate returns false then terminate main function
            if (!operate(current->elements + (i * deque->size), args)) {
                return;
            }
        }

        remaining -= (i - start); // decrement remaining size by operated size of current node
        current = current->next; // go to next node
    }
}

void map_back_ideque(const ideque_s * deque, const handle_fn handle, void * args) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(handle && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    size_t remaining = deque->length;
    size_t last_index = deque->length + deque->current - 1; // may undeflow if lenght adn current are 0

    struct infinite_deque_node const * current = deque->head; // save head node as current pointer
    while (remaining) { // while there are element to handle
        current = current->prev; // fist go to tail and other previous nodes

        const size_t node_index = last_index % IDEQUE_CHUNK; // calculate last element index in node
        size_t i = 0; // save number of operated elements in node
        for (; i <= node_index && remaining; ++i, remaining--) { // until node index is reached and elements remain
            // handle on reversed i to start from last element in node
            if (!handle(current->elements + ((node_index - i) * deque->size), args)) {
                return;
            }
        }
        last_index -= i; // may undeflow
    }
}

void apply_ideque(const ideque_s * deque, const process_fn process, void * arguments) {
    assert(deque && "[ERROR] Parameter is NULL.");
    assert(process && "[ERROR] Parameter is NULL.");

    assert(deque->size && "[INVALID] Size can't be zero.");

    char * elements_array = deque->allocator->alloc(deque->length * deque->size, deque->allocator->arguments);
    assert(elements_array && "[ERROR] Memory alloction failed.");

    size_t index = 0, remaining = deque->length;
    struct infinite_deque_node const * current = deque->head;
    for (size_t start = deque->current; remaining; start = 0) {
        size_t i = start;
        for (; i < (remaining + start) && i < IDEQUE_CHUNK; ++i) {
            memcpy(elements_array + (index * deque->size), current->elements + (i * deque->size), deque->size);
            index++;
        }

        remaining -= (i - start);
        current = current->next;
    }

    process(elements_array, deque->length, arguments);

    index = 0, remaining = deque->length, current = deque->head;
    for (size_t start = deque->current; remaining; start = 0) {
        size_t i = start;
        for (; i < (remaining + start) && i < IDEQUE_CHUNK; ++i) {
            memcpy(current->elements + (i * deque->size), elements_array + (index * deque->size), deque->size);
            index++;
        }

        remaining -= (i - start);
        current = current->next;
    }

    deque->allocator->free(elements_array, deque->allocator->arguments);
}
