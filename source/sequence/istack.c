#include <sequence/istack.h>

#include <string.h>

istack_s create_istack(size_t const size) {
    error(size && "Paremeter can't be zero.");

    return (istack_s) { .size = size, .allocator = &standard };
}

istack_s make_istack(size_t const size, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (istack_s) { .size = size, .allocator = allocator };
}

void destroy_istack(istack_s * const stack, set_fn const destroy) {
    error(stack && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }
    stack->allocator->free(stack->elements, stack->allocator->arguments); // free elements array

    memset(stack, 0, sizeof(istack_s));
}

void clear_istack(istack_s * const stack, set_fn const destroy) {
    error(stack && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }
    stack->allocator->free(stack->elements, stack->allocator->arguments); // free elements array

    // set lenght to zero
    stack->length = stack->capacity = 0;
    stack->elements = NULL;
}

istack_s copy_istack(istack_s const * const stack, copy_fn const copy) {
    error(stack && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // create replica to initialize and return
    istack_s const replica = {
        .capacity = stack->capacity, .length = 0, .size = stack->size,
        .elements = stack->allocator->alloc(stack->capacity * stack->size, stack->allocator->arguments),
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");

    // initialize replica's elements array with stack's elements
    for (size_t i = 0; i < stack->length; ++i) {
        size_t const offset = i * stack->size;
        copy(replica.elements + offset, stack->elements + offset);
    }

    return replica;
}

bool is_empty_istack(istack_s const * const stack) {
    error(stack && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    return !(stack->length);
}

void push_istack(istack_s * const restrict stack, void const * const restrict element) {
    error(stack && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(stack != element && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    if (stack->length == stack->capacity) { // if length is equal to capacity the array must expand linearly
        stack->capacity += ISTACK_CHUNK;
        stack->elements = stack->allocator->realloc(stack->elements, stack->capacity * stack->size, stack->allocator->arguments);
        error(stack->elements && "Memory allocation failed.");
    }

    // push element knowing the elements array can fit it
    memcpy(stack->elements + (stack->length * stack->size), element, stack->size);
    stack->length++;
}

void pop_istack(istack_s * const restrict stack, void * const restrict buffer) {
    error(stack && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(stack->length && "Length can't be zero.");
    error(stack->elements && "Elements can't be NULL.");
    error(stack != buffer && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // remove element from elements array
    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);

    if (stack->length == (stack->capacity - ISTACK_CHUNK)) { // if array can be shrunk
        stack->capacity -= ISTACK_CHUNK;
        stack->elements = stack->allocator->realloc(stack->elements, stack->capacity * stack->size, stack->allocator->arguments);
        error(!stack->capacity || stack->elements && "Memory allocation failed.");
    }
}

void peep_istack(istack_s const * const restrict stack, void * const restrict buffer) {
    error(stack && "Paremeter can't be NULL.");
    error(buffer && "Paremeter can't be NULL.");
    error(stack->length && "Length can't be zero.");
    error(stack->elements && "Elements can't be NULL.");
    error(stack != buffer && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // only copy the top element into the buffer
    memcpy(buffer, stack->elements + ((stack->length - 1) * stack->size), stack->size);
}

void each_istack(istack_s const * const restrict stack, handle_fn const handle, void * const restrict arguments) {
    error(stack && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(stack != arguments && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // iterate over each element from bottom to the top of stack
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size) && handle(e, arguments); e += stack->size) {}
}

void apply_istack(istack_s const * const restrict stack, process_fn const process, void * const restrict arguments) {
    error(stack && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(stack != arguments && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // process stack elements as an array (as a whole)
    process(stack->elements, stack->length, arguments);
}
