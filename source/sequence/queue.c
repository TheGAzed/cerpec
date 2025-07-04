#include <sequence/queue.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

queue_s create_queue(const size_t size) {
    assert(size && "[ERROR] Paremeter can't be zero.");

    return (queue_s) { .tail = NULL, .count = 0, .current = 0, .size = size, };
}

void destroy_queue(queue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(queue->size && "[ERROR] Element's size can't be zero.");
    assert(queue->count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    size_t i = queue->current;
    for (struct queue_node * head = queue->tail; queue->count;) {
        head = head->next;

        for (; i < queue->count && i < QUEUE_CHUNK; ++i) {
            destroy((char*)head->elements + (i * queue->size), queue->size);
        }
        queue->count -= i;
        i = 0;

        free(head->elements);

        struct queue_node * temp = head->next;
        free(head);
        head = temp;
    }

    queue->tail = NULL;
    queue->count = queue->current = queue->size = 0;
}

void clear_queue(queue_s * queue, const destroy_fn destroy) {
    assert(queue && "[ERROR] Paremeter can't be NULL.");
    assert(destroy && "[ERROR] Paremeter can't be NULL.");

    assert(queue->size && "[ERROR] Element's size can't be zero.");
    assert(queue->count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    size_t i = queue->current;
    for (struct queue_node * head = queue->tail; queue->count;) {
        head = head->next;

        for (; i < queue->count && i < QUEUE_CHUNK; ++i) {
            destroy((char*)head->elements + (i * queue->size), queue->size);
        }
        queue->count -= i;
        i = 0;

        free(head->elements);

        struct queue_node * temp = head->next;
        free(head);
        head = temp;
    }

    queue->tail->next = NULL; // head will be tail, thus making the node circular to itself
    queue->count = queue->current = 0;
}

queue_s copy_queue(const queue_s queue, const copy_fn copy) {
    assert(copy && "[ERROR] Paremeter can't be NULL.");

    assert(queue.size && "[ERROR] Element's size can't be zero.");
    assert(queue.count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    queue_s replica = {
        .tail = NULL, .count = 0, .current = queue.current, .size = queue.size,
    };

    size_t i = queue.current, remaining_size = queue.size;
    struct queue_node * last = NULL;
    for (struct queue_node * q_node = queue.tail, ** r_node = &(replica.tail); replica.count != queue.count;) {
        q_node = q_node->next;

        struct queue_node * node = last = malloc(sizeof(struct queue_node));
        assert(node && "[ERROR] Memory allocation failed.");

        (*r_node) = node;
        (*r_node)->next = replica.tail;
        r_node = &(node->next);

        node->elements = malloc(QUEUE_CHUNK * replica.size);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        for (; i < remaining_size && i < QUEUE_CHUNK; ++i) {
            const size_t index = i * queue.size;
            copy((char*)node->elements + index, (char*)q_node->elements + index, queue.size);
        }
        remaining_size -= i;
        i = 0;
    }
    replica.tail = last;

    return replica;
}

bool is_empty_queue(const queue_s queue) {
    assert(queue.size && "[ERROR] Element's size can't be zero.");
    assert(queue.count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    return !(queue.count);
}

void enqueue(queue_s * queue, const void * buffer) {
    assert(queue && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");

    assert(queue->size && "[ERROR] Element's size can't be zero.");
    assert(queue->count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    const size_t next_index = (queue->current + queue->count) % QUEUE_CHUNK;
    if (!next_index) {
        struct queue_node * node = malloc(sizeof(struct queue_node));
        assert(node && "[ERROR] Memory allocation failed.");

        node->elements = malloc(QUEUE_CHUNK * queue->size);
        assert(node->elements && "[ERROR] Memory allocation failed.");

        if (queue->tail) {
            node->next = queue->tail->next;
            queue->tail->next = node;
        } else {
            node->next = node;
        }
        queue->tail = node;
    }

    memcpy((char*)(queue->tail->elements) + next_index, buffer, queue->size);
    queue->count++;
}

void dequeue(queue_s * queue, void * buffer) {
    assert(queue && "[ERROR] Paremeter can't be NULL.");
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(queue->count && "[ERROR] Count can't be zero.");
    assert(queue->tail && "[ERROR] Tail can't be NULL.");

    assert(queue->size && "[ERROR] Element's size can't be zero.");
    assert(queue->count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    memcpy(buffer, (queue->tail->next->elements) + queue->current, queue->size);
    queue->current++;
    queue->count--;

    if (queue->current == QUEUE_CHUNK || !queue->count) {
        struct queue_node * removed = queue->tail->next;
        queue->tail = removed->next;

        free(removed->elements);
        free(removed);

        if (!queue->count) {
            queue->tail = NULL;
            queue->current = 0;
        }
    }
}

void peek_queue(const queue_s queue, void * buffer) {
    assert(buffer && "[ERROR] Paremeter can't be NULL.");
    assert(queue.count && "[ERROR] Count can't be zero.");
    assert(queue.tail && "[ERROR] Tail can't be NULL.");

    assert(queue.size && "[ERROR] Element's size can't be zero.");
    assert(queue.count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    memcpy(buffer, (queue.tail->next->elements) + queue.current, queue.size);
}

void foreach_queue(const queue_s queue, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Paremeter can't be NULL.");

    assert(queue.size && "[ERROR] Element's size can't be zero.");
    assert(queue.count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    size_t i = queue.current, remaining = queue.count;
    for (struct queue_node const * head = queue.tail; queue.count;) {
        head = head->next;

        for (; i < queue.count && i < QUEUE_CHUNK; ++i) {
            if (!operate((char*)head->elements + (i * queue.size), queue.size, arguments)) {
                return;
            }
        }
        remaining -= i;
        i = 0;
    }
}

void map_queue(const queue_s queue, const manage_fn manage, void * arguments) {
    assert(manage && "[ERROR] Paremeter can't be NULL.");

    assert(queue.size && "[ERROR] Element's size can't be zero.");
    assert(queue.count <= QUEUE_CHUNK && "[ERROR] Count exceeds capacity.");

    void * elements = malloc(queue.count * queue.size);
    assert(elements && "[ERROR] Memory allocation failed.");

    size_t i = queue.current, remaining = queue.count, index = 0;
    for (struct queue_node const * head = queue.tail; remaining;) {
        head = head->next;

        const size_t copy_size = remaining < QUEUE_CHUNK - i ? remaining : QUEUE_CHUNK - i;
        memcpy((char*)(elements) + index, (char*)(head->elements) + i, copy_size);

        remaining -= copy_size;
        i = 0;
    }

    manage(elements, queue.count, queue.size, arguments);

    i = queue.current;
    remaining = queue.count;
    index = 0;
    for (struct queue_node const * head = queue.tail; remaining;) {
        head = head->next;

        const size_t copy_size = remaining < QUEUE_CHUNK - i ? remaining : QUEUE_CHUNK - i;
        memcpy((char*)(head->elements) + i, (char*)(elements) + index, copy_size);

        remaining -= copy_size;
        i = 0;
    }

    free(elements);
}
