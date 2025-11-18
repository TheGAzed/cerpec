#include <sequence/istack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

istack_s create_istack(const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");

    return (istack_s) { .size = size, 0 };
}

void destroy_istack(istack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }
    free(stack->elements); // free elements array

    // set all parameters to zero/NULL
    stack->capacity = stack->length = stack->size = 0;
    stack->elements = NULL;
}

void clear_istack(istack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }

    // set lenght to zero
    stack->length = 0;
}

istack_s copy_istack(const istack_s * stack, const copy_fn copy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // create replica to initialize and return
    const istack_s replica = {
        .capacity = stack->capacity, .length = 0, .elements = malloc(stack->capacity * stack->size), .size = stack->size,
    };
    assert(!replica.capacity || replica.elements && "[ERROR] Memory allocation failed.");

    // initialize replica's elements array with stack's elements
    for (char * s = stack->elements, * r = replica.elements; s < stack->elements + (stack->length * stack->size);) {
        copy(r, s);
        s += stack->size;
        r += stack->size;
    }

    return replica;
}

bool is_empty_istack(const istack_s * stack) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    return !(stack->length);
}

void push_istack(istack_s * stack, const void * element) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    if (stack->length == stack->capacity) { // if length is equal to capacity the array must expand linearly
        stack->capacity += ISTACK_CHUNK;
        stack->elements = realloc(stack->elements, stack->capacity * stack->size);
        assert(stack->elements && "[ERROR] Memory allocation failed.");
    }

    // push element knowing the elements array can fit it
    memcpy(stack->elements + (stack->length * stack->size), element, stack->size);
    stack->length++;
}

void pop_istack(istack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack->length && "[ERROR] Length can't be zero.");
    assert(stack->capacity && "[ERROR] Capacity can't be zero.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // remove element from elements array
    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);

    if (stack->length == (stack->capacity - ISTACK_CHUNK)) { // if array can be shrunk
        stack->capacity -= ISTACK_CHUNK;
        stack->elements = realloc(stack->elements, stack->capacity * stack->size);
        assert(!stack->capacity || stack->elements && "[ERROR] Memory allocation failed.");
    }
}

void peep_istack(const istack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack->length && "[ERROR] Length can't be zero.");
    assert(stack->capacity && "[ERROR] Capacity can't be zero.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // only copy the top element into the buffer
    memcpy(buffer, stack->elements + ((stack->length - 1) * stack->size), stack->size);
}

void map_istack(const istack_s * stack, const handle_fn handle, void * arguments) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(handle && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // iterate over each element from bottom to the top of stack
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size) && handle(e, arguments); e += stack->size) {}
}

void apply_istack(const istack_s * stack, const process_fn process, void * arguments) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(process && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Length exceeds capacity.");

    // process stack elements as an array (as a whole)
    process(stack->elements, stack->length, arguments);
}
