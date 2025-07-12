#include <sequence/stack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

stack_s create_stack(const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");

    return (stack_s) { .size = size, 0 };
}

void destroy_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }
    free(stack->elements); // free elements array

    // set all parameters to zero/NULL
    stack->capacity = stack->length = stack->size = 0;
    stack->elements = NULL;
}

void clear_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }

    // set lenght to zero
    stack->length = 0;
}

stack_s copy_stack(const stack_s stack, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    // create replica to initialize and return
    const stack_s replica = {
        .capacity = stack.capacity, .length = 0, .elements = malloc(stack.capacity * stack.size), .size = stack.size,
    };
    assert(!replica.capacity || replica.elements && "[ERROR] Memory allocation failed.");

    // initialize replica's elements array with stack's elements
    for (char * s = stack.elements, * r = replica.elements; s < stack.elements + (stack.length * stack.size);) {
        copy(r, s, stack.size);
        s += stack.size;
        r += stack.size;
    }

    return replica;
}

bool is_empty_stack(const stack_s stack) {
    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    return !(stack.length);
}

void push_stack(stack_s * stack, const void * element) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(element && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    if (stack->length == stack->capacity) { // if length is equal to capacity the array must expand linearly
        stack->capacity += STACK_CHUNK;
        stack->elements = realloc(stack->elements, stack->capacity * stack->size);
        assert(stack->elements && "[ERROR] Memory allocation failed.");
    }

    // push element knowing the elements array can fit it
    memcpy(stack->elements + (stack->length * stack->size), element, stack->size);
    stack->length++;
}

void pop_stack(stack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack->length && "[ERROR] Count can't be zero.");
    assert(stack->capacity && "[ERROR] Capacity can't be zero.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    // remove element from elements array
    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);

    if (stack->length == (stack->capacity - STACK_CHUNK)) { // if array can be shrunk
        stack->capacity -= STACK_CHUNK;
        stack->elements = realloc(stack->elements, stack->capacity * stack->size);
        assert(!stack->capacity || stack->elements && "[ERROR] Memory allocation failed.");
    }
}

void peep_stack(const stack_s stack, void * buffer) {
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack.length && "[ERROR] Count can't be zero.");
    assert(stack.capacity && "[ERROR] Capacity can't be zero.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.elements && "[INVALID] Elements can' be NULL.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    // only copy the top element into the buffer
    memcpy(buffer, stack.elements + ((stack.length - 1) * stack.size), stack.size);
}

void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    // iterate over each element from bottom to the top of stack
    for (char * e = stack.elements; e < stack.elements + (stack.length * stack.size) && operate(e, stack.size, arguments); e += stack.size) {}
}

void map_stack(const stack_s stack, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    // manage stack elements as an array (as a whole)
    manage(stack.elements, stack.length, stack.size, arguments);
}
