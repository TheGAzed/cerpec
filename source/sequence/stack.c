#include <sequence/stack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

stack_s create_stack(const length_t size) {
    const stack_s stack = {
        .capacity = STACK_CHUNK, .count = 0, .size = size,
        .elements = malloc((STACK_CHUNK) * (size))
    };
    assert(stack.elements && "[ERROR] Memory allocation failed.");

    return stack;
}

void destroy_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(stack->elements && "[ERROR] Invalid elements pointer state.");

    for (char * e = stack->elements; e < (char*)(stack->elements) + (stack->count * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }
    free(stack->elements);

    stack->capacity = stack->count = stack->size = 0;
    stack->elements = NULL;
}

void clear_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(stack->elements && "[ERROR] Invalid elements pointer state.");
    assert(stack->count <= stack->capacity && "[ERROR] Count exceeds capacity.");
    assert(stack->capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack->count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    for (char * e = stack->elements; e < (char*)(stack->elements) + (stack->count * stack->size); e += stack->size) {
        destroy(e, stack->size);
    }
    free(stack->elements);

    stack->capacity = stack->count = 0;
    stack->elements = NULL;
}

stack_s copy_stack(const stack_s stack, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(stack.elements && "[ERROR] Invalid elements pointer state.");
    assert(stack.count <= stack.capacity && "[ERROR] Count exceeds capacity.");
    assert(stack.capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack.count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    const stack_s replica = {
        .capacity = stack.capacity, .count = 0, .elements = malloc((size_t)(stack.capacity * stack.size)), .size = stack.size,
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");

    for (char * s = stack.elements, * c = replica.elements; s < (char*)(stack.elements) + (stack.count * stack.size);) {
        copy(s, c, stack.size);
        s += stack.size;
        c += stack.size;
    }

    return replica;
}

bool is_empty_stack(const stack_s stack) {
    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(stack.elements && "[ERROR] Invalid elements pointer state.");
    assert(stack.count <= stack.capacity && "[ERROR] Count exceeds capacity.");
    assert(stack.capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack.count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    return !(stack.count);
}

void push_stack(stack_s * stack, const void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert((LENGTH_MAX - STACK_CHUNK) >= stack->capacity && "[ERROR] Capacity can't exceed maximum size");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(stack->elements && "[ERROR] Invalid elements pointer state.");
    assert(stack->count <= stack->capacity && "[ERROR] Count exceeds capacity.");
    assert(stack->capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack->count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    if (stack->size == stack->capacity) {
        stack->capacity += STACK_CHUNK;
        stack->elements = realloc(stack->elements, (size_t)(stack->capacity * stack->size));
        assert(stack->elements && "[ERROR] Memory allocation failed.");
    }

    memcpy(stack->elements + (stack->count * stack->size), buffer, (size_t)(stack->size));
    stack->count++;
}

void pop_stack(stack_s * stack, void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack->count && "[ERROR] Count can't be zero.");
    assert(stack->capacity && "[ERROR] Capacity can't be zero.");
    assert(stack->elements && "[ERROR] Elements array can't be NULL.");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(stack->elements && "[ERROR] Invalid elements pointer state.");
    assert(stack->count <= stack->capacity && "[ERROR] Count exceeds capacity.");
    assert(stack->capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack->count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    stack->count--;
    memcpy(buffer, stack->elements + (stack->count * stack->size), stack->size);

    if (stack->count == stack->capacity - (STACK_CHUNK) && stack->count) {
        stack->capacity -= STACK_CHUNK;
        stack->elements = realloc(stack->elements, (size_t)(stack->capacity * stack->size));
        assert(!stack->capacity || stack->elements && "[ERROR] Memory allocation failed.");
    }
}

void peep_stack(const stack_s stack, void * buffer) {
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(stack.count && "[ERROR] Count can't be zero.");
    assert(stack.capacity && "[ERROR] Capacity can't be zero.");
    assert(stack.elements && "[ERROR] Elements array can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(stack.elements && "[ERROR] Invalid elements pointer state.");
    assert(stack.count <= stack.capacity && "[ERROR] Count exceeds capacity.");
    assert(stack.capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack.count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    memcpy(buffer, stack.elements + ((stack.count - 1) * stack.size), stack.size);
}

void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(stack.elements && "[ERROR] Invalid elements pointer state.");
    assert(stack.count <= stack.capacity && "[ERROR] Count exceeds capacity.");
    assert(stack.capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack.count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    for (char * e = stack.elements; e < (char*)(stack.elements) + (stack.count * stack.size); e += stack.size) {
        operate(e, stack.size, arguments);
    }
}

void map_stack(const stack_s stack, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(stack.elements && "[ERROR] Invalid elements pointer state.");
    assert(stack.count <= stack.capacity && "[ERROR] Count exceeds capacity.");
    assert(stack.capacity <= LENGTH_MAX && "[ERROR] Invalid capacity length.");
    assert(stack.count <= LENGTH_MAX && "[ERROR] Invalid count length.");

    manage(stack.elements, stack.count, stack.size, arguments);
}


