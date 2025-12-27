#include <sequence/fqueue.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

fqueue_s create_fqueue(const size_t max, const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");

    const fqueue_s queue = {
        .elements = malloc(max * size),
        .max = max, .size = size,
    };
    assert(queue.elements && "[ERROR] Memory allocation failed.");

    return queue;
}

void destroy_fqueue(fqueue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    const size_t absolute = queue->current + queue->length;
    const size_t right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    const size_t left_length = queue->length - right_length;

    for (size_t i = queue->current; i < right_length + queue->current; ++i) {
        destroy(queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i) {
        destroy(queue->elements + (i * queue->size));
    }

    free(queue->elements);

    memset(queue, 0, sizeof(fqueue_s));
}

void clear_fqueue(fqueue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    const size_t absolute = queue->current + queue->length;
    const size_t right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    const size_t left_length = queue->length - right_length;

    for (size_t i = queue->current; i < right_length + queue->current; ++i) {
        destroy(queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i) {
        destroy(queue->elements + (i * queue->size));
    }

    queue->length = queue->current = 0;
}

fqueue_s copy_fqueue(const fqueue_s * queue, const copy_fn copy) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    const fqueue_s replica = {
        .elements = malloc(queue->max * queue->size),
        .max = queue->max, .size = queue->size, .length = queue->length,
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");

    const size_t absolute = queue->current + queue->length;
    const size_t right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    const size_t left_length = queue->length - right_length;

    size_t current = 0;
    for (size_t i = queue->current; i < right_length + queue->current; ++i, ++current) {
        copy(replica.elements + (current * replica.size), queue->elements + (i * queue->size));
    }
    for (size_t i = 0; i < left_length; ++i, ++current) {
        copy(replica.elements + (current * replica.size), queue->elements + (i * queue->size));
    }

    return replica;
}

bool is_empty_fqueue(const fqueue_s * queue) {
    assert(queue && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    return !(queue->length);
}

bool is_full_fqueue(const fqueue_s * queue) {
    assert(queue && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    return (queue->length == queue->max);
}

void enqueue_fqueue(fqueue_s * queue, const void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    const size_t position = (queue->current + queue->length) % queue->max;

    memcpy(queue->elements + (position * queue->size), buffer, queue->size);
    queue->length++;
}

void dequeue_fqueue(fqueue_s * queue, void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    memcpy(buffer, queue->elements + (queue->current * queue->size), queue->size);
    queue->current = (queue->current + 1) % queue->max;
    queue->length--;
}

void peek_fqueue(const fqueue_s * queue, void * buffer) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    memcpy(buffer, queue->elements + (queue->current * queue->size), queue->size);
}

void map_fqueue(const fqueue_s * queue, const handle_fn handle, void * arguments) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    const size_t absolute = queue->current + queue->length;
    const size_t right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    const size_t left_length = queue->length - right_length;

    bool is_handle = true;
    for (size_t i = queue->current; i < (right_length + queue->current) && is_handle; ++i) {
        is_handle = handle(queue->elements + (i * queue->size), arguments);
    }
    for (size_t i = 0; i < left_length && is_handle; ++i) {
        is_handle = handle(queue->elements + (i * queue->size), arguments);
    }
}

void apply_fqueue(const fqueue_s * queue, const process_fn process, void * arguments) {
    assert(queue && "[ERROR] Parameter can't be NULL.");
    assert(process && "[ERROR] Parameter can't be NULL.");

    assert(queue->size && "[INVALID] Size can't be zero.");
    assert(queue->max && "[INVALID] Maximum can't be zero.");
    assert(queue->elements && "[INVALID] Elements can't be NULL.");
    assert(queue->length <= queue->max && "[INVALID] Length exceeds maximum.");

    char * elements_array = malloc(queue->length * queue->size);
    assert(elements_array && "[ERROR] Memory allocation failed.");

    const size_t absolute = queue->current + queue->length;
    const size_t right_length = absolute > queue->max ? queue->max - queue->current : queue->length;
    const size_t left_length = queue->length - right_length;

    memcpy(elements_array, queue->elements + (queue->current * queue->size), right_length * queue->size);
    memcpy(elements_array + (right_length * queue->size), queue->elements, left_length * queue->size);

    process(elements_array, queue->length, arguments);

    memcpy(queue->elements + (queue->current * queue->size), elements_array, right_length * queue->size);
    memcpy(queue->elements, elements_array + (right_length * queue->size), left_length * queue->size);

    free(elements_array);
}
