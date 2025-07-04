#ifndef QUEUE_H
#define QUEUE_H

#include <cerpec.h>

#if !defined(QUEUE_CHUNK)
#   define QUEUE_CHUNK CERPEC_CHUNK
#endif

struct queue_node {
    void * elements;
    struct queue_node * next;
};

typedef struct queue {
    struct queue_node * tail;
    size_t size, count, current;
} queue_s;

queue_s create_queue(const size_t size);
void destroy_queue(queue_s * queue, const destroy_fn destroy);
void clear_queue(queue_s * queue, const destroy_fn destroy);
queue_s copy_queue(const queue_s queue, const copy_fn copy);

bool is_empty_queue(const queue_s queue);
void enqueue(queue_s * queue, const void * buffer);
void dequeue(queue_s * queue, void * buffer);
void peek_queue(const queue_s queue, void * buffer);

void foreach_queue(const queue_s queue, const operate_fn operate, void * arguments);
void map_queue(const queue_s queue, const manage_fn manage, void * arguments);

#endif // QUEUE_H
