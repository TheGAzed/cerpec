#include <sequence/fstack.h>

#include <string.h>

fstack_s create_fstack(size_t const size, size_t const max) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");

    // create constant initialized structure with checked allocated memory
    fstack_s const stack = {
        .elements = standard.alloc(max * size, standard.arguments),
        .max = max, .size = size, .allocator = &standard,
    };
    error(stack.elements && "Memory allocation failed.");

    return stack;
}

fstack_s make_fstack(size_t const size, size_t const max, memory_s const * const allocator) {
    error(size && "Paremeter can't be zero.");
    error(max && "Paremeter can't be zero.");
    error(allocator && "Paremeter can't be NULL.");

    // create constant initialized structure with custom checked allocated memory
    fstack_s const stack = {
        .elements = allocator->alloc(max * size, allocator->arguments),
        .max = max, .size = size, .allocator = allocator,
    };
    error(stack.elements && "Memory allocation failed.");

    return stack;
}

void destroy_fstack(fstack_s * const stack, set_fn const destroy) {
    error(stack && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // iterate over each element and call destroy function on it
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }

    // free elements array and set everything to zero/NULL
    stack->allocator->free(stack->elements, stack->allocator->arguments);

    stack->length = stack->max = stack->size = 0;
    stack->elements = NULL;
    stack->allocator = NULL;
}

void clear_fstack(fstack_s * const stack, set_fn const destroy) {
    error(stack && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // iterate over each element and call destroy function on it
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size); e += stack->size) {
        destroy(e);
    }

    // just clear length
    stack->length = 0;
}

fstack_s copy_fstack(fstack_s const * const stack, copy_fn const copy) {
    error(stack && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // create initialized constant copy of structure with checked allocated memory
    fstack_s const replica = {
        .elements = stack->allocator->alloc(stack->max * stack->size, stack->allocator->arguments),
        .length = stack->length, .max = stack->max, .size = stack->size,
        .allocator = stack->allocator,
    };
    error(replica.elements && "Memory allocation failed.");

    // iterate over each element in stack and copy it into replica using function pointer
    for (size_t i = 0; i < stack->length; ++i) {
        copy(replica.elements + (i * replica.size), stack->elements + (i * stack->size));
    }

    return replica;
}

bool is_empty_fstack(fstack_s const * const stack) {
    error(stack && "Parameter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    return !(stack->length); // return negated length
}

bool is_full_fstack(fstack_s const * const stack) {
    error(stack && "Parameter can't be NULL.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    return (stack->length == stack->max); // return comparison of length and max
}

void push_fstack(fstack_s * const restrict stack, void const * const restrict element) {
    error(stack && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(stack->length != stack->max && "Structure is full.");
    error(stack != element && "Parameters can't be equal.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // copy element beyond last element in structure's elements array
    memcpy(stack->elements + (stack->length * stack->size), element, stack->size);
    stack->length++;
}

void pop_fstack(fstack_s * const restrict stack, void * const restrict buffer) {
    error(stack && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(stack->length && "Structure is empty.");
    error(stack != buffer && "Parameters can't be equal.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // decrement length to get last's position and copy it into buffer
    stack->length--;
    memcpy(buffer, stack->elements + (stack->length * stack->size), stack->size);
}

void peep_fstack(fstack_s const * const restrict stack, void * const restrict buffer) {
    error(stack && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");
    error(stack->length && "Structure is empty.");
    error(stack != buffer && "Parameters can't be equal.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // just subtract 1 from length to get current position of topmost element to copy into buffer
    memcpy(buffer, stack->elements + ((stack->length - 1) * stack->size), stack->size);
}

void map_fstack(fstack_s const * const restrict stack, handle_fn const handle, void * const restrict arguments) {
    error(stack && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(stack != arguments && "Parameters can't be equal.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // empty for loop since all the logic can just fit into its expressions
    for (char * e = stack->elements; e < stack->elements + (stack->length * stack->size) && handle(e, arguments); e += stack->size) {}
}

void apply_fstack(fstack_s const * const restrict stack, process_fn const process, void * const restrict arguments) {
    error(stack && "Parameter can't be NULL.");
    error(process && "Parameter can't be NULL.");
    error(stack != arguments && "Parameters can't be equal.");

    valid(stack->size && "Size can't be zero.");
    valid(stack->max && "Maximum can't be zero.");
    valid(stack->elements && "Elements array can't be NULL.");
    valid(stack->allocator && "Allocator can't be NULL.");
    valid(stack->length <= stack->max && "Length exceeds maximum.");

    // simply call the function on the stack as everyting is continuous
    process(stack->elements, stack->length, arguments);
}
