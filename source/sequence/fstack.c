#include <sequence/fstack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

fstack_s create_fstack(const size_t max, const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");
    assert(max && "[ERROR] Paremeter can't be zero.");

    const fstack_s stack = {
        .elements = malloc(max * size),
        .max = max, .size = size,
    };
    assert(stack.elements && "[ERROR] Memory allocation failed.");

    return stack;
}

void destroy_fstack(fstack_s * stack, const set_fn destroy) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }
    free(stack->elements);

    memset(stack, 0, sizeof(fstack_s));
}

void clear_fstack(fstack_s * stack, const set_fn destroy) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }
    stack->length = 0;
}

fstack_s copy_fstack(const fstack_s * stack, const copy_fn copy) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    const fstack_s replica = {
        .elements = malloc(stack->max * stack->size),
        .length = stack->length, .max = stack->max, .size = stack->size
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < stack->length; ++i) {
        copy(replica.elements + (i * replica.size), stack->elements + (i * stack->size));
    }

    return replica;
}

bool is_empty_fstack(const fstack_s * stack) {
    assert(stack && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    return !(stack->length);
}

bool is_full_fstack(const fstack_s * stack) {
    assert(stack && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    return (stack->length == stack->max);
}

void push_fstack(fstack_s * stack, const void * buffer) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(stack->length != stack->max && "[ERROR] Structure is full.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    memcpy(stack->elements + (stack->length * stack->size), buffer, stack->size);
    stack->length++;
}

void pop_fstack(fstack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(stack->length && "[ERROR] Structure is empty.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);
}

void peep_fstack(const fstack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(stack->length && "[ERROR] Structure is empty.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    memcpy(buffer, stack->elements + ((stack->length - 1) * stack->size), stack->size);
}

void map_fstack(const fstack_s * stack, const handle_fn handle, void * arguments) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    for (char * e = stack->elements; e < stack->elements + (stack->length + stack->size) && handle(e, arguments); e += stack->size) {}
}

void apply_fstack(const fstack_s * stack, const process_fn process, void * arguments) {
    assert(stack && "[ERROR] Parameter can't be NULL.");
    assert(process && "[ERROR] Parameter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->max && "[INVALID] Maximum can't be zero.");
    assert(stack->elements && "[INVALID] Elements can't be NULL.");
    assert(stack->length <= stack->max && "[INVALID] Length exceeds maximum.");

    process(stack->elements, stack->length, arguments);
}
