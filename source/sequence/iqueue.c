#include <sequence/iqueue.h>

#include <string.h>

iqueue_s create_iqueue(size_t const size) {
    error(size && "Parameter can't be zero.");

    return (iqueue_s) { .size = size, .allocator = &standard };
}

iqueue_s make_iqueue(size_t const size, memory_s const * const allocator) {
    error(size && "Parameter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (iqueue_s) { .size = size, .allocator = allocator };
}

void destroy_iqueue(iqueue_s * const queue, set_fn const destroy) {
    error(queue && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    // for each node starting from tail (previous to head)
    struct infinite_queue_node * previous = queue->tail;
    for (size_t start = queue->current; queue->length; start = 0) {
        // destroy elements in previous' next (so head and next)
        size_t i = start;
        for (; i < queue->length && i < IQUEUE_CHUNK; ++i) {
            destroy(previous->next->elements + (i * queue->size));
        }
        queue->length -= (i - start);

        // save destroyed node and cut it from queue list
        struct infinite_queue_node * temp = previous->next;
        previous->next = previous->next->next;

        // free destroyed node and its elements array
        queue->allocator->free(temp->elements, queue->allocator->arguments);
        queue->allocator->free(temp, queue->allocator->arguments);
    }

    // set everything to zero
    memset(queue, 0, sizeof(iqueue_s));
}

void clear_iqueue(iqueue_s * const queue, set_fn const destroy) {
    error(queue && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    // for each node starting from tail (previous to head)
    struct infinite_queue_node * previous = queue->tail;
    for (size_t start = queue->current; queue->length; start = 0) {
        // destroy elements in previous' next (so head and next)
        size_t i = start;
        for (; i < queue->length && i < IQUEUE_CHUNK; ++i) {
            destroy(previous->next->elements + (i * queue->size));
        }
        queue->length -= (i - start);

        // save destroyed node and cut it from queue list
        struct infinite_queue_node * temp = previous->next;
        previous->next = previous->next->next;

        // free destroyed node and its elements array
        queue->allocator->free(temp->elements, queue->allocator->arguments);
        queue->allocator->free(temp, queue->allocator->arguments);
    }

    queue->current = queue->length = 0;
    queue->tail = NULL;
}

iqueue_s copy_iqueue(iqueue_s const * const queue, copy_fn const copy) {
    error(queue && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    // create properly initialized replica
    iqueue_s replica = { .size = queue->size, .length = queue->length, .current = queue->current };

    // set originale queue's and replica's current nodes for iteration
    struct infinite_queue_node const * current_queue = queue->tail;
    struct infinite_queue_node ** current_copy = &(replica.tail); // two pointer list to remove special .head case

    struct infinite_queue_node * last = NULL;
    for (size_t r = queue->length, s = queue->current; r; s = 0) {
        // allocate new node for replica
        last = (*current_copy) = queue->allocator->alloc(sizeof(struct infinite_queue_node), queue->allocator->arguments);
        error((*current_copy) && "Memory allocation failed");

        // allocate new elements array for replica
        (*current_copy)->elements = queue->allocator->alloc(IQUEUE_CHUNK * queue->size, queue->allocator->arguments);
        error((*current_copy)->elements && "Memory allocation failed.");

        // redirect current replica's node with replica's tail fopr circularity
        (*current_copy)->next = replica.tail;
        current_queue = current_queue->next;

        // outside for loop to get copied chunk size, since it can either be 'remaining' or 'IQUEUE_CHUNK'
        size_t i = s;
        for (; i < r && i < IQUEUE_CHUNK; ++i) { // while i is less than both 'remaining' and 'IQUEUE_CHUNK'
            copy((*current_copy)->elements + (i * queue->size), current_queue->elements + (i * queue->size));
        }
        r -= i; // subtract copied size from remaining size using i

        current_copy = &((*current_copy)->next);
    }
    replica.tail = last; // set replica's tail to its last added node (i.e. its tail)

    return replica;
}

bool is_empty_iqueue(iqueue_s const * const queue) {
    error(queue && "Parameter can't be NULL.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    return (queue->length == 0);
}

void peek_iqueue(iqueue_s const * const restrict queue, void * const restrict buffer) {
    error(queue && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(queue->length && "Can't peek empty structure.");
    error(queue->tail && "Tail can't be NULL");
    error(queue != buffer && "Parameters can't be the same.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    memcpy(buffer, queue->tail->next->elements + (queue->current * queue->size), queue->size);
}

void enqueue_iqueue(iqueue_s * const restrict queue, void const * const restrict buffer) {
    error(queue && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(~(queue->length) && "Queue's size will overflow.");
    error(queue != buffer && "Parameters can't be the same.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    // index where the next element will be enqueued
    size_t const next_index = (queue->current + queue->length) % IQUEUE_CHUNK;
    if (!next_index) { // if head list array is full (is divisible) adds new list element to head
        struct infinite_queue_node * node = queue->allocator->alloc(sizeof(struct infinite_queue_node), queue->allocator->arguments);
        error(node && "Memory allocation failed");

        node->elements = queue->allocator->alloc(IQUEUE_CHUNK * queue->size, queue->allocator->arguments);
        error(node->elements && "Memory allocation failed.");

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

void dequeue_iqueue(iqueue_s * const restrict queue, void * const restrict buffer) {
    error(queue && "Parameter can't be NULL.");
    error(queue->length && "Strucutre can't be empty.");
    error(queue->tail && "Tail can't be NULL");
    error(buffer && "Parameter can't be NULL.");
    error(queue != buffer && "Parameters can't be the same.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    memcpy(buffer, queue->tail->next->elements + (queue->current * queue->size), queue->size);
    queue->length--; // decrement queue size
    queue->current = (queue->current + 1) % IQUEUE_CHUNK; // set current to next index in node array

    if (!queue->length) { // if queue is empty after extracting element thne free memory and reset everything to zero
        queue->allocator->free(queue->tail->elements, queue->allocator->arguments); // free empty tail/head node
        queue->allocator->free(queue->tail, queue->allocator->arguments); // free empty tail/head node

        queue->current = 0; // if queue is empty make current index 0 to not break enqueue_iqueue operation
        queue->tail = NULL; // set tail to NULL
    } else if (queue->current == 0) { // else if current index circles back, free start list element and shift to next
        struct infinite_queue_node * head = queue->tail->next; // get empty head node
        queue->tail->next = queue->tail->next->next; // set new head node to its next node

        queue->allocator->free(head->elements, queue->allocator->arguments); // free previous head node elements
        queue->allocator->free(head, queue->allocator->arguments); // free previous head node
    }
}

void each_iqueue(iqueue_s const * const restrict queue, handle_fn const handle, void * const restrict arguments) {
    error(queue && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL");
    error(queue != arguments && "Parameters can't be the same.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    size_t remaining = queue->length; // save queue size as remaining size for iteration
    struct infinite_queue_node const * previous = queue->tail;
    // while remaining size wasn't decremented to zero
    for (size_t start = queue->current; remaining; start = 0, previous = previous->next) {
        const struct infinite_queue_node * current = previous->next;

        size_t i = start; // save i outside loop to later use it in subtraction
        for (;i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than either remaining size or node's array size
            // handle on element and if zero is returned then end main function
            if (!handle(current->elements + (i * queue->size), arguments)) {
                return;
            }
        }
        remaining -= (i - start); // subtract absolute value of i and start from remaining size
    }
}

void apply_iqueue(iqueue_s const * const restrict queue, process_fn const process, void * const restrict arguments) {
    error(queue && "Parameter can't be NULL.");
    error(process && "Parameter can't be NULL");
    error(queue != arguments && "Parameters can't be the same.");

    valid(queue->size && "Size can't be zero.");
    valid(queue->allocator && "Allocator can't be NULL.");
    valid(queue->current < IQUEUE_CHUNK && "Current exceeds chunk.");

    // create elements array to temporary save element from circular linked list nodes
    char * elements_array = queue->allocator->alloc(queue->length * queue->size, queue->allocator->arguments);
    error((!queue->length || elements_array) && "Memory allocation failed.");

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

    process(elements_array, queue->length, arguments); // process initialized elements array

    index = 0, remaining = queue->length, previous = queue->tail;
    for (size_t start = queue->current; remaining; start = 0, previous = previous->next) { // save elements array back into queue
        size_t i = start; // extract i from for loop to later subtract it from remaining size
        for (;i < remaining && i < IQUEUE_CHUNK; ++i) { // while i is less than remaining size and node array size
            memcpy(previous->next->elements + (i * queue->size), elements_array + (index * queue->size), queue->size);
            index++;
        }
        remaining -= (i - start); // subtract absolute value of i and start from remaining size
    }

    queue->allocator->free(elements_array, queue->allocator->arguments);
}
