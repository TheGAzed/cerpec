#include <sequence/istack.h>

#include <string.h>

/// @brief Resizes (reallocates) stack parameter arrays based on changed capacity.
/// @param stack Structure to resize.
/// @param size New size.
void _istack_resize(istack_s * const stack, size_t const size);

istack_s create_istack(size_t const size) {
    error(size && "Paremeter can't be zero.");

    return (istack_s) { .size = size, .allocator = &standard };
}

istack_s make_istack(size_t const size, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    return (istack_s) { .size = size, .allocator = allocator };
}

void destroy_istack(istack_s * const stack, set_fn const destroy, void * const ad) {
    error(stack && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, ad);
    }
    stack->allocator->free(stack->elements, stack->allocator->arg); // free elements array

    memset(stack, 0, sizeof(istack_s));
}

void clear_istack(istack_s * const stack, set_fn const destroy, void * const ad) {
    error(stack && "Paremeter can't be NULL.");
    error(destroy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // for each element in elements array call destroy
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e, ad);
    }
    stack->allocator->free(stack->elements, stack->allocator->arg); // free elements array

    // set lenght to zero
    stack->length = stack->capacity = 0;
    stack->elements = NULL;
}

istack_s copy_istack(istack_s const * const stack, copy_fn const copy, void * const ac) {
    error(stack && "Paremeter can't be NULL.");
    error(copy && "Paremeter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // create replica to initialize and return
    istack_s const replica = {
        .capacity = stack->capacity, .length = 0, .size = stack->size,
        .elements = stack->allocator->alloc(stack->capacity * stack->size, stack->allocator->arg),
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");

    // initialize replica's elements array with stack's elements
    for (size_t i = 0; i < stack->length; ++i) {
        size_t const offset = i * stack->size;
        copy(replica.elements + offset, stack->elements + offset, ac);
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

void push_istack(istack_s * const stack, void const * const element) {
    error(stack && "Paremeter can't be NULL.");
    error(element && "Paremeter can't be NULL.");
    error(stack != element && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    if (stack->length == stack->capacity) { // if length is equal to capacity the array must expand linearly
        size_t const capacity = stack->length ? stack->length * CERPEC_FACTOR : ISTACK_CHUNK;
        _istack_resize(stack, capacity);
    }

    // push element knowing the elements array can fit it
    memcpy(stack->elements + (stack->length * stack->size), element, stack->size);
    stack->length++;
}

void pop_istack(istack_s * const stack, void * const buffer) {
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

    if (stack->length <= stack->capacity / CERPEC_FACTOR && (stack->length > ISTACK_CHUNK || !stack->length)) {
        _istack_resize(stack, stack->length);
    }
}

void peep_istack(istack_s const * const stack, void * const buffer) {
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

void each_istack(istack_s const * const stack, handle_fn const handle, void * const ah) {
    error(stack && "Paremeter can't be NULL.");
    error(handle && "Paremeter can't be NULL.");
    error(stack != ah && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // iterate over each element from bottom to the top of stack
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size) && handle(e, ah); e += stack->size) {}
}

void apply_istack(istack_s const * const stack, process_fn const process, void * const ap) {
    error(stack && "Paremeter can't be NULL.");
    error(process && "Paremeter can't be NULL.");
    error(stack != ap && "Parameters can't be the same.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->length <= stack->capacity && "Length exceeds capacity.");
    valid(stack->allocator && "Allocator can't be NULL.");

    // process stack elements as an array (as a whole)
    process(stack->elements, stack->length, ap);
}

void _istack_resize(istack_s * const stack, size_t const size) {
    stack->capacity = size;

    stack->elements = stack->allocator->realloc(stack->elements, stack->capacity * stack->size, stack->allocator->arg);
    error(!stack->capacity || stack->elements && "Memory allocation failed.");
}
