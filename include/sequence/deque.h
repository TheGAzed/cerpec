#ifndef DEQUE_H
#define DEQUE_H

#include <cerpec.h>

#if !defined(DEQUE_CHUNK)
#   define DEQUE_CHUNK CERPEC_CHUNK
#endif

struct deque_node {
    struct deque_node * next;
    struct deque_node * prev;
    char elements[DEQUE_CHUNK];
};

typedef struct deque {
    struct deque_node * head;
    size_t current, size, length;
} deque_s;

deque_s create_deque(const size_t size);
void destroy_deque(deque_s * deque, const destroy_fn destroy);
void clear_deque(deque_s * deque, const destroy_fn destroy);
deque_s copy_deque(const deque_s deque, const copy_fn copy);

bool is_empty_deque(const deque_s deque);
void enqueue_front(deque_s * deque, const void * buffer);
void enqueue_back(deque_s * deque, const void * buffer);
void dequeue_front(deque_s * deque, void * buffer);
void dequeue_back(deque_s * deque, void * buffer);
void peek_front_deque(const deque_s deque, void * buffer);
void peek_back_deque(const deque_s deque, void * buffer);

void foreach_front_deque(const deque_s deque, const operate_fn operate, void * arguments);
void foreach_back_deque(const deque_s deque, const operate_fn operate, void * arguments);
void map_deque(const deque_s deque, const manage_fn manage, void * arguments);

#endif //DEQUE_H
