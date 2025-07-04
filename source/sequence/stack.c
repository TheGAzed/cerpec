#include <sequence/stack.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

stack_s create_stack(const length_t size) {
    return (stack_s) { .size = size, 0 };
}

void destroy_stack(stack_s * stack, const destroy_fn destroy) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(!stack->count || stack->elements && "[ERROR] Invalid elements pointer state.");

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
    assert(!stack->count || stack->elements && "[ERROR] Invalid elements pointer state.");

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
    assert(!stack.capacity || stack.elements && "[ERROR] Invalid elements pointer state.");

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
    assert(!stack.capacity || stack.elements && "[ERROR] Invalid elements pointer state.");

    return !(stack.count);
}

void push_stack(stack_s * stack, const void * buffer) {
    assert(stack && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert((LENGTH_MAX >> 1) >= stack->capacity && "[ERROR] Capacity can't exceed maximum size");

    assert(stack->size && "[ERROR] Element's size can't be zero.");
    assert(!stack->count || stack->elements && "[ERROR] Invalid elements pointer state.");

    if (stack->size == stack->capacity) {
        stack->capacity <<= 1;
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
    assert(!stack->count || stack->elements && "[ERROR] Invalid elements pointer state.");

    stack->count--;
    memcpy(buffer, stack->elements + (stack->count * stack->size), stack->size);

    const length_t half_capacity = stack->capacity >> 1;
    if (stack->count == half_capacity) {
        stack->capacity = half_capacity;
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
    assert(!stack.count || stack.elements && "[ERROR] Invalid elements pointer state.");

    memcpy(buffer, stack.elements + ((stack.count - 1) * stack.size), stack.size);
}

void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(!stack.count || stack.elements && "[ERROR] Invalid elements pointer state.");

    for (char * e = stack.elements; e < (char*)(stack.elements) + (stack.count * stack.size); e += stack.size) {
        operate(e, stack.size, arguments);
    }
}

void map_stack(const stack_s stack, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(stack.size && "[ERROR] Element's size can't be zero.");
    assert(!stack.count || stack.elements && "[ERROR] Invalid elements pointer state.");

    manage(stack.elements, stack.count, stack.size, arguments);
}


