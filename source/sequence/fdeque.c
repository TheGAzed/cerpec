#include <sequence/fdeque.h>

#include <string.h>

fdeque_s create_fdeque(size_t const size, size_t const max) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");

    // create finite queue with allocated memory and check if allocation succeded
    fdeque_s const deque = {
        .elements = standard.alloc(size * max, standard.arguments),
        .size = size, .max = max,
        .allocator = &standard,
    };
    error(deque.elements && "Memory allocation failed.");

    return deque;
}

fdeque_s make_fdeque(size_t const size, size_t const max, memory_s const * allocator) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    // create finite queue with custom allocated memory and check if allocation succeded
    fdeque_s const deque = {
        .elements = allocator->alloc(size * max, allocator->arguments),
        .size = size, .max = max,
        .allocator = allocator,
    };
    error(deque.elements && "Memory allocation failed.");

    return deque;
}

void destroy_fdeque(fdeque_s * const deque, set_fn const destroy) {
    error(deque && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // iterate over elements on the right, if they exist with current offset
    for (size_t i = deque->current; i < right_length + deque->current; ++i) {
        destroy(deque->elements + (i * deque->size));
    }
    // iterate over elements on the left size starting from zero, if they exist
    for (size_t i = 0; i < left_length; ++i) {
        destroy(deque->elements + (i * deque->size));
    }

    // free elements array and set all values to zero/NULL
    deque->allocator->free(deque->elements, deque->allocator->arguments);

    memset(deque, 0, sizeof(fdeque_s));
}

void clear_fdeque(fdeque_s * const deque, set_fn const destroy) {
    error(deque && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // iterate over elements on the right, if they exist with current offset
    for (size_t i = deque->current; i < right_length + deque->current; ++i) {
        destroy(deque->elements + (i * deque->size));
    }
    // iterate over elements on the left size starting from zero, if they exist
    for (size_t i = 0; i < left_length; ++i) {
        destroy(deque->elements + (i * deque->size));
    }

    // only set parameters than will only clear the structure (it must remain usable)
    deque->length = deque->current = 0;
}

fdeque_s copy_fdeque(fdeque_s const * const deque, copy_fn const copy) {
    error(deque && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // create initial replica structure with allocated memory (current remains 0)
    fdeque_s const replica = {
        .elements = deque->allocator->alloc(deque->max * deque->size, deque->allocator->arguments),
        .max = deque->max, .size = deque->size, .length = deque->length,
        .allocator = deque->allocator,
    };
    error(replica.elements && "Memory allocation failed.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // copy elements from original into replica while replica's start from begining
    size_t current = 0;
    // iterate over elements on the right, if they exist with current offset
    for (size_t i = deque->current; i < right_length + deque->current; ++i, ++current) {
        copy(replica.elements + (current * replica.size), deque->elements + (i * deque->size));
    }
    // iterate over elements on the left size starting from zero, if they exist
    for (size_t i = 0; i < left_length; ++i, ++current) {
        copy(replica.elements + (current * replica.size), deque->elements + (i * deque->size));
    }

    return replica;
}

bool is_empty_fdeque(fdeque_s const * const deque) {
    error(deque && "Parameter can't be NULL.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    return !(deque->length); // return the negation
}

bool is_full_fdeque(fdeque_s const * const deque) {
    error(deque && "Parameter can't be NULL.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    return (deque->length == deque->max); // compare if length is max
}

void enqueue_front_fdeque(fdeque_s * const restrict deque, void const * const restrict element) {
    error(deque && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(deque->length != deque->max && "Structure is full.");
    error(deque != element && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // if current is zero then set it to max and then it decrements to empty last position
    if (!(deque->current)) {
        deque->current = deque->max;
    }
    // decrement to get empty front position, set element element to array and increment length
    deque->current--;
    memcpy(deque->elements + (deque->current * deque->size), element, deque->size);
    deque->length++;
}

void enqueue_back_fdeque(fdeque_s * const restrict deque, void const * const restrict element) {
    error(deque && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(deque->length != deque->max && "Structure is full.");
    error(deque != element && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // calculate position of empty index after last element
    size_t const position = (deque->current + deque->length) % deque->max;

    // copy element element to deque's array and increment length
    memcpy(deque->elements + (position * deque->size), element, deque->size);
    deque->length++;
}

void dequeue_front_fdeque(fdeque_s * const restrict deque, void * const restrict buffer) {
    error(deque && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(deque->length && "Structure is empty.");
    error(deque != buffer && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // copy front element to buffer, increment current index while keeping circularity and decrement length
    memcpy(buffer, deque->elements + (deque->current * deque->size), deque->size);
    deque->current = (deque->current + 1) % deque->max;
    deque->length--;
}

void dequeue_back_fdeque(fdeque_s * const restrict deque, void * const restrict buffer) {
    error(deque && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(deque->length && "Structure is empty.");
    error(deque != buffer && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // decrement length and calculate position of back element, copy it to buffer
    deque->length--;
    size_t const position = (deque->current + deque->length) % deque->max;

    memcpy(buffer, deque->elements + (position * deque->size), deque->size);
}

void peek_front_fdeque(fdeque_s const * const restrict deque, void * const restrict buffer) {
    error(deque && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(deque->length && "Structure is empty.");
    error(deque != buffer && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // copy element at current (front) position
    memcpy(buffer, deque->elements + (deque->current * deque->size), deque->size);
}

void peek_back_fdeque(fdeque_s const * const restrict deque, void * const restrict buffer) {
    error(deque && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(deque->length && "Structure is empty.");
    error(deque != buffer && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // calculate location of back element and copy it into buffer
    size_t const position = (deque->current + deque->length - 1) % deque->max;
    memcpy(buffer, deque->elements + (position * deque->size), deque->size);
}

void each_front_fdeque(fdeque_s const * const restrict deque, handle_fn const handle, void * const restrict arguments) {
    error(deque && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(deque != arguments && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // create flag to save handle return value when handling is terminated
    bool is_handle = true;
    for (size_t i = deque->current; i < (right_length + deque->current) && is_handle; ++i) {
        is_handle = handle(deque->elements + (i * deque->size), arguments);
    }
    for (size_t i = 0; i < left_length && is_handle; ++i) {
        is_handle = handle(deque->elements + (i * deque->size), arguments);
    }
}

void each_back_fdeque(fdeque_s const * const restrict deque, handle_fn const handle, void * const restrict arguments) {
    error(deque && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(deque != arguments && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // create flag to save handle return value when handling is terminated
    // also reverse the order of element access compared to 'front' alternative
    bool is_handle = true;
    for (size_t i = 0; i < left_length && is_handle; ++i) {
        const size_t reverse = left_length - i - 1;
        is_handle = handle(deque->elements + (reverse * deque->size), arguments);
    }
    for (size_t i = 0; i < right_length && is_handle; ++i) {
        const size_t reverse = right_length - i - 1;
        is_handle = handle(deque->elements + ((reverse + deque->current) * deque->size), arguments);
    }
}

void apply_fdeque(fdeque_s const * const restrict deque, process_fn const process, void * const restrict arguments) {
    error(deque && "Parameter can't be NULL.");
    error(process && "Parameter can't be NULL.");
    error(deque != arguments && "Parameters can't be equal.");

    valid(deque->size && "Size can't be zero.");
    valid(deque->max && "Maximum can't be zero.");
    valid(deque->elements && "Elements array can't be NULL.");
    valid(deque->allocator && "Allocator can't be NULL.");
    valid(deque->length <= deque->max && "Length exceeds maximum.");
    valid(deque->current < deque->max && "Current exceeds maximum.");

    // initialize temporary elements array to save elements from circular field into straight
    char * elements_array = deque->allocator->alloc(deque->length * deque->size, deque->allocator->arguments);
    error((!deque->length || elements_array) && "Memory allocation failed.");

    // divide circular elements array into right and left part
    size_t const absolute = deque->current + deque->length;
    size_t const right_length = absolute > deque->max ? deque->max - deque->current : deque->length;
    size_t const left_length = deque->length - right_length;

    // copy left and right side elements into temporary array
    memcpy(elements_array, deque->elements + (deque->current * deque->size), right_length * deque->size);
    memcpy(elements_array + (right_length * deque->size), deque->elements, left_length * deque->size);

    // process temporary array
    process(elements_array, deque->length, arguments);

    // save elements from temporary array back into structure while keeping its order
    memcpy(deque->elements + (deque->current * deque->size), elements_array, right_length * deque->size);
    memcpy(deque->elements, elements_array + (right_length * deque->size), left_length * deque->size);

    // free temporary array
    deque->allocator->free(elements_array, deque->allocator->arguments);
}
