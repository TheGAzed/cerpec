#include <sequence/stack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

stack_s create_stack(const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");

    const stack_s stack = {
        .capacity = STACK_CHUNK, .length = 0, .size = size,
        .elements = malloc((STACK_CHUNK) * (size))
    };
    assert(stack.elements && "[ERROR] Memory allocation failed.");

    return stack;
}

void destroy_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }
    free(stack->elements);

    stack->capacity = stack->length = stack->size = 0;
    stack->elements = NULL;
}

void clear_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }
    stack->elements = realloc(stack->elements, STACK_CHUNK * (stack->size));

    stack->capacity = stack->length = 0;
}

stack_s copy_stack(const stack_s stack, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.elements && "[INVALID] Elements can' be NULL.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    const stack_s replica = {
        .capacity = stack.capacity, .length = 0, .elements = malloc(stack.capacity * stack.size), .size = stack.size,
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");

    for (char * s = stack.elements, * c = replica.elements; s < stack.elements + (stack.length * stack.size);) {
        copy(s, c, stack.size);
        s += stack.size;
        c += stack.size;
    }

    return replica;
}

bool is_empty_stack(const stack_s stack) {
    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.elements && "[INVALID] Elements can' be NULL.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    return !(stack.length);
}

void push_stack(stack_s * stack, const void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert((((size_t)(-1)) - STACK_CHUNK) >= stack->capacity && "[ERROR] Capacity can't exceed maximum size");

    assert(stack->size && "[INVALID] Size can't be zero.");
    assert(stack->elements && "[INVALID] Elements can' be NULL.");
    assert(stack->length <= stack->capacity && "[INVALID] Count exceeds capacity.");

    if (stack->size == stack->capacity) {
        stack->capacity += STACK_CHUNK;
        stack->elements = realloc(stack->elements, stack->capacity * stack->size);
        assert(stack->elements && "[ERROR] Memory allocation failed.");
    }

    memcpy(stack->elements + (stack->length * stack->size), buffer, stack->size);
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

    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);

    if (stack->length == stack->capacity - (STACK_CHUNK) && stack->length) {
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

    memcpy(buffer, stack.elements + ((stack.length - 1) * stack.size), stack.size);
}

void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.elements && "[INVALID] Elements can' be NULL.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    for (char * e = stack.elements; e < stack.elements + (stack.length * stack.size) && operate(e, stack.size, arguments); e += stack.size) {}
}

void map_stack(const stack_s stack, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[INVALID] Size can't be zero.");
    assert(stack.elements && "[INVALID] Elements can' be NULL.");
    assert(stack.length <= stack.capacity && "[INVALID] Count exceeds capacity.");

    manage(stack.elements, stack.length, stack.size, arguments);
}
