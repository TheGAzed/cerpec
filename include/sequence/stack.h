#ifndef STACK_H
#define STACK_H

#include <cerpec.h>

#if !defined(STACK_CHUNK)
#   define STACK_CHUNK CERPEC_CHUNK
#endif

typedef struct stack {
    char * elements;
    size_t size, length, capacity;
} stack_s;

stack_s create_stack(const size_t size);
void destroy_stack(stack_s * stack, const destroy_fn destroy);
void clear_stack(stack_s * stack, const destroy_fn destroy);
stack_s copy_stack(const stack_s stack, const copy_fn copy);

bool is_empty_stack(const stack_s stack);
void push_stack(stack_s * stack, const void * buffer);
void pop_stack(stack_s * stack, void * buffer);
void peep_stack(const stack_s stack, void * buffer);

void foreach_stack(const stack_s stack, const operate_fn operate, void * arguments);
void map_stack(const stack_s stack, const manage_fn manage, void * arguments);

#endif // STACK_H
