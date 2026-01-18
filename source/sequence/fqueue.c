#include <sequence/fqueue.h>

#include <assert.h>
#include <string.h>

fqueue_s create_fqueue(size_t const size, size_t const max) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");

    // initializer constant queue structure with allocated memory and check if it was successful
    fqueue_s const queue = {
        .elements = standard.alloc(max * size, standard.arguments),
        .max = max, .size = size, .allocator = &standard,
    };
    assert(queue.elements && "[ERROR] Memory allocation failed.");

    return queue;
}

fqueue_s make_fqueue(size_t const size, size_t const max, memory_s const * const allocator) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");

    // initializer constant queue structure with allocated memory and check if it was successful
    fqueue_s const queue = {
        .elements = allocator->alloc(max * size, allocator->arguments),
        .max = max, .size = size, .allocator = allocator,
    };
    assert(queue.elements && "[ERROR] Memory allocation failed.");

    return queue;
}

void destroy_fqueue(fqueue_s * const queue, set_fn const destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = queue->current + queue->length;
    size_t const right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    size_t const left_length = queue->length - right_length;

    // iterate over rith and left elements and call destroy function on them
    for (size_t i = queue->current; i < right_length + queue->current; ++i) {
        destroy(queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i) {
        destroy(queue->elements + (i * queue->size));
    }

    // free memory array and set everything manually to zero/NULL
    queue->allocator->free(queue->elements, queue->allocator->arguments);

    queue->current = queue->length = queue->max = queue->size = 0;
    queue->elements = NULL;
    queue->allocator = NULL;
}

void clear_fqueue(fqueue_s * const queue, set_fn const destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = queue->current + queue->length;
    size_t const right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    size_t const left_length = queue->length - right_length;

    // iterate over rith and left elements and call destroy function on them
    for (size_t i = queue->current; i < right_length + queue->current; ++i) {
        destroy(queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i) {
        destroy(queue->elements + (i * queue->size));
    }

    // just clear size and set current to start (zero)
    queue->length = queue->current = 0;
}

fqueue_s copy_fqueue(fqueue_s const * const queue, copy_fn const copy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // initialize constant replica copy of queue
    fqueue_s const replica = {
        .elements = queue->allocator->alloc(queue->max * queue->size, queue->allocator->arguments),
        .max = queue->max, .size = queue->size, .length = queue->length,
        .allocator = queue->allocator,
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");

    // divide circular elements array into right and left part
    size_t const absolute = queue->current + queue->length;
    size_t const right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    size_t const left_length = queue->length - right_length;

    // set a current index for replica elements to start at index zero and copy elements
    size_t current = 0;
    for (size_t i = queue->current; i < right_length + queue->current; ++i, ++current) {
        copy(replica.elements + (current * replica.size), queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i, ++current) {
        copy(replica.elements + (current * replica.size), queue->elements + (i * queue->size));
    }

    return replica;
}

bool is_empty_fqueue(fqueue_s const * const queue) {
    assert(queue && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    return !(queue->length); // return length equal to zero
}

bool is_full_fqueue(fqueue_s const * const queue) {
    assert(queue && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    return (queue->length == queue->max); // return comparison of length and max
}

void enqueue_fqueue(fqueue_s * const queue, void const * const buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(queue->length != queue->max && "[ERROR] Structure is full.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // calculate position of last element in array
    size_t const position = (queue->current + queue->length) % queue->max;

    // set new element to the end of the elements array and increment queue's length
    memcpy(queue->elements + (position * queue->size), buffer, queue->size);
    queue->length++;
}

void dequeue_fqueue(fqueue_s * const queue, void * const buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(queue->length && "[ERROR] Structure is empty.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // copy element at the front to buffer, set new current position and decrement length
    memcpy(buffer, queue->elements + (queue->current * queue->size), queue->size);
    queue->current = (queue->current + 1) % queue->max;
    queue->length--;
}

void peek_fqueue(fqueue_s const * const queue, void * const buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(queue->length && "[ERROR] Structure is empty.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // just copy the front element at current index to buffer
    memcpy(buffer, queue->elements + (queue->current * queue->size), queue->size);
}

void map_fqueue(fqueue_s const * const queue, handle_fn const handle, void * const arguments) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = queue->current + queue->length;
    size_t const right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    size_t const left_length = queue->length - right_length;

    // save temporary handle flag to check if handler hasn't terminated during iteration
    bool is_handle = true;
    for (size_t i = queue->current; i < (right_length + queue->current) && is_handle; ++i) {
        is_handle = handle(queue->elements + (i * queue->size), arguments);
    }
    for (size_t i = 0; i < left_length && is_handle; ++i) {
        is_handle = handle(queue->elements + (i * queue->size), arguments);
    }
}

void apply_fqueue(fqueue_s const * const queue, process_fn const process, void * const arguments) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(process && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");
    assert(queue->current < queue->max && "[INVALID] Current exceeds maximum.");

    // create temporary straight elements array to save elements from queue's circular one
    char * elements_array = queue->allocator->alloc(queue->length * queue->size, queue->allocator->arguments);
    assert(elements_array && "[ERROR] Memory allocation failed.");

    // divide circular elements array into right and left part
    size_t const absolute = queue->current + queue->length;
    size_t const right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    size_t const left_length = queue->length - right_length;

    // copy left and right elements into temporary elements array
    memcpy(elements_array, queue->elements + (queue->current * queue->size), right_length * queue->size);
    memcpy(elements_array + (right_length * queue->size), queue->elements, left_length * queue->size);

    // process elements array
    process(elements_array, queue->length, arguments);

    // copy back elements from temporary array into queue
    memcpy(queue->elements + (queue->current * queue->size), elements_array, right_length * queue->size);
    memcpy(queue->elements, elements_array + (right_length * queue->size), left_length * queue->size);

    // free temporary elements array
    queue->allocator->free(elements_array, queue->allocator->arguments);
}
