#include <sequence/iqueue.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

iqueue_s create_iqueue(const size_t size) {
    assert(size && "[ERROR] Size can't be zero.");

    return (iqueue_s) { .size = size, 0 };
}

void destroy_iqueue(iqueue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");

    struct infinite_queue_node * previous = queue->tail;
    for (size_t start = queue->current; queue->length; start = 0) {
        size_t i = start;
        for (; i < queue->length && i < IQUEUE_CHUNK; ++i) {
            destroy(previous->next->elements + (i * queue->size));
        }
        queue->length -= (i - start);

        struct infinite_queue_node * temp = previous->next;
        previous->next = previous->next->next;

        free(temp->elements);
        free(temp);
    }

    memset(queue, 0, sizeof(iqueue_s)); // set everything to zero
}

void clear_iqueue(iqueue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");

    struct infinite_queue_node * previous = queue->tail;
    for (size_t start = queue->current; queue->length; start = 0) {
        size_t i = start;
        for (; i < queue->length && i < IQUEUE_CHUNK; ++i) {
            destroy(previous->next->elements + (i * queue->size));
        }
        queue->length -= (i - start);

        struct infinite_queue_node * temp = previous->next;
        previous->next = previous->next->next;

        free(temp->elements);
        free(temp);
    }

    queue->current = 0;
    queue->tail = NULL;
}

iqueue_s copy_iqueue(const iqueue_s * queue, const copy_fn copy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");

    iqueue_s queue_copy = { .size = queue->size, .tail = NULL, .length = queue->length, .current = queue->current };

    struct infinite_queue_node const * current_queue = queue->tail;
    struct infinite_queue_node ** current_copy = &(queue_copy.tail); // two pointer list to remove special .head case

    size_t remaining = queue->length, start_index = queue->current;
    struct infinite_queue_node * last_added = NULL;
    while (remaining) {
        last_added = (*current_copy) = malloc(sizeof(struct infinite_queue_node));
        assert((*current_copy) && "[ERROR] Memory allocation failed");

        last_added->elements = malloc(IQUEUE_CHUNK * queue->size);
        assert(last_added->elements && "[ERROR] Memory allocation failed.");

        (*current_copy)->next = queue_copy.tail; // make current/last node's next pointer point to tail
        current_queue = current_queue->next;

        // outside for loop to get copied chunk size, since it can either be 'remaining' or 'IQUEUE_CHUNK'
        size_t i = start_index;
        for (; i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than both 'remaining' and 'IQUEUE_CHUNK'
            copy((*current_copy)->elements + (i * queue->size), current_queue->elements + (i * queue->size));
        }
        remaining -= i; // subtract copied size from remaining size using i
        start_index = 0; // set start index to zero since we only need start index startinf from queue's currrent in tail node

        current_copy = &((*current_copy)->next);
    }
    queue_copy.tail = last_added;

    return queue_copy;
}

bool is_empty_iqueue(const iqueue_s * queue) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(queue->size && "[INVALID] Size can't be zero.");

    return (queue->length == 0);
}

void peek_iqueue(const iqueue_s * queue, void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(queue->length && "[ERROR] Can't peek empty structure.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->tail && "[INVALID] Tail can't be NULL");
    assert(queue->size && "[INVALID] Size can't be zero.");

    memcpy(buffer, queue->tail->next->elements + (queue->current * queue->size), queue->size);
}

void enqueue_iqueue(iqueue_s * queue, const void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(~(queue->length) && "[ERROR] Queue's size will overflow.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");

    // index where the next element will be enqueued
    const size_t next_index = (queue->current + queue->length) % IQUEUE_CHUNK;
    if (!next_index) { // if head list array is full (is divisible) adds new list element to head
        struct infinite_queue_node * node = malloc(sizeof(struct infinite_queue_node));
        assert(node && "[ERROR] Memory allocation failed");

        node->elements = malloc(IQUEUE_CHUNK * queue->size);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        if (queue->tail == NULL) {
            node->next = node; // create initial circle
        } else {
            node->next = queue->tail->next; // make temp's next node head node
            queue->tail->next = node; // make previous tail's next node point to temp
        }
        queue->tail = node;
    }

    memcpy(queue->tail->elements + (next_index * queue->size), buffer, queue->size);
    queue->length++;
}

void dequeue_iqueue(iqueue_s * queue, void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(queue->length && "[ERROR] Strucutre can't be empty.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->tail && "[INVALID] Tail can't be NULL");
    assert(queue->tail->next && "[INVALID] Tail's next (head) can't be NULL");
    assert(queue->size && "[INVALID] Size can't be zero.");

    memcpy(buffer, queue->tail->next->elements + (queue->current * queue->size), queue->size);
    queue->length--; // decrement queue size
    queue->current = (queue->current + 1) % IQUEUE_CHUNK; // set current to next index in node array

    if (!queue->length) { // if queue is empty after extracting element thne free memory and reset everything to zero
        free(queue->tail->elements); // free empty tail/head node
        free(queue->tail); // free empty tail/head node

        queue->current = 0; // if queue is empty make current index 0 to not break enqueue_iqueue operation
        queue->tail = NULL; // set tail to NULL
    } else if (queue->current == 0) { // else if current index circles back, free start list element and shift to next
        struct infinite_queue_node * head = queue->tail->next; // get empty head node
        queue->tail->next = queue->tail->next->next; // set new head node to its next node

        free(head->elements); // free previous head node elements
        free(head); // free previous head node
    }
}

void foreach_iqueue(const iqueue_s * queue, const operate_fn operate, void * args) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(operate && "[ERROR] Parameter can't be NULL");

    size_t remaining = queue->length; // save queue size as remaining size for iteration
    struct infinite_queue_node const * previous = queue->tail;
    // while remaining size wasn't decremented to zero
    for (size_t start = queue->current; remaining; start = 0, previous = previous->next) {
        struct infinite_queue_node * current = previous->next;

        size_t i = start; // save i outside loop to later use it in subtraction
        for (;i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than either remaining size or node's array size
            // operate on element and if zero is returned then end main function
            if (!operate(current->elements + (i * queue->size), args)) {
                return;
            }
        }
        remaining -= (i - start); // subtract absolute value of i and start from remaining size
    }
}

void map_iqueue(const iqueue_s * queue, const manage_fn manage, void * arguments) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(manage && "[ERROR] Parameter can't be NULL");

    assert(queue->size && "[INVALID] Size can't be zero.");

    // create elements array to temporary save element from circular linked list nodes
    char * elements_array = malloc(queue->length * queue->size);
    assert(elements_array && "[ERROR] Memory allocation failed.");

    size_t index = 0, remaining = queue->length; // temporary variable to save queue's size
    struct infinite_queue_node const * previous = queue->tail; // create node pointer to save previous nodes into
    for (size_t start = queue->current; remaining; start = 0, previous = previous->next) {
        size_t i = start; // extract i from for loop to later subtract it from remaining size
        for (;i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than remaining size and node array size
            // save previous' next elements (so current) to elements array at index, and increment index
            memcpy(elements_array + (index * queue->size), previous->next->elements + (i * queue->size), queue->size);
            index++;
        }
        remaining -= (i - start); // subtract absolute value of i and start from remaining size
    }

    manage(elements_array, queue->length, arguments); // manage initialized elements array

    index = 0, remaining = queue->length, previous = queue->tail;
    for (size_t start = queue->current; remaining; start = 0, previous = previous->next) { // save elements array back into queue
        size_t i = start; // extract i from for loop to later subtract it from remaining size
        for (;i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than remaining size and node array size
            memcpy(previous->next->elements + (i * queue->size), elements_array + (index * queue->size), queue->size);
            index++;
        }
        remaining -= (i - start); // subtract absolute value of i and start from remaining size
    }

    free(elements_array);
}
