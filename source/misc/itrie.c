#include <misc/itrie.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL (size_t)(-1)

void _itrie_resize(itrie_s * trie, const size_t size);
void _itrie_fill_hole(itrie_s * trie, const size_t hole);

itrie_s create_itrie(const size_t size, const size_t count, const hash_fn hash) {
    return (itrie_s) { .count = count, .size = size, .hash = hash, .root = NIL, };
}

void destroy_itrie(itrie_s * trie, const destroy_fn destroy) {
    for (size_t i = 0; i < trie->length; ++i) {
        destroy(trie->elements + (i * trie->size));
    }

    free(trie->elements);
    free(trie->nodes);
    free(trie->parent);
    free(trie->terminal);

    memset(trie, 0, sizeof(itrie_s));
}

void clear_itrie(itrie_s * trie, const destroy_fn destroy) {
    for (size_t i = 0; i < trie->length; ++i) {
        destroy(trie->elements + (i * trie->size));
    }

    free(trie->elements);
    free(trie->nodes);
    free(trie->parent);
    free(trie->terminal);

    trie->capacity = trie->count = trie->length = trie->root = 0;
    trie->elements = NULL;
    trie->nodes = trie->parent = NULL;
    trie->terminal = NULL;
}

itrie_s copy_itrie(const itrie_s trie, const copy_fn copy) {
    const itrie_s replica = {
        .root = trie.root, .hash = trie.hash,
        .capacity = trie.capacity, .length = trie.length, .size = trie.size, .count = trie.count,

        .elements = malloc(trie.size * trie.capacity),
        .nodes = malloc(trie.capacity * trie.count * sizeof(size_t)),
        .parent = malloc(trie.capacity * sizeof(size_t)),
        .terminal = malloc(trie.capacity * sizeof(bool)),
    };

    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.nodes) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.parent) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.terminal) && "[ERROR] Memory allocation failed.");

    memcpy(replica.nodes, trie.nodes, trie.length * trie.count * sizeof(size_t));
    memcpy(replica.parent, trie.parent, trie.length * sizeof(size_t));
    memcpy(replica.terminal, trie.terminal, trie.length * sizeof(bool));

    for (size_t i = 0; i < trie.length; ++i) {
        copy(replica.elements + (i * replica.size), trie.elements + (i * trie.size));
    }

    return replica;
}

bool is_empty_itrie(const itrie_s trie) {
    return !(trie.length);
}

void insert_itrie(itrie_s * trie, const void * array, const size_t length, const copy_fn copy) {
    if (!length) {
        return;
    }

    if (trie->length == trie->capacity) {
        _itrie_resize(trie, trie->capacity + ITRIE_CHUNK);
    }

    size_t * node = &(trie->root), i = 0, parent = NIL;
    bool * term = trie->terminal;
    for (; i < length && NIL != (*node); ++i) {
        const size_t hash = trie->hash(array + (i * trie->size));
        assert(hash < trie->count);

        term = trie->terminal + (*node);
        parent = (*node);
        node = trie->nodes + (trie->count * (*node)) + hash;
    }

    for (; i < length; ++i) {
        (*node) = trie->length;
        trie->parent[(*node)] = parent;

        copy(trie->elements + (trie->size * (*node)), array + (trie->size * i));
        trie->length++;

        term = trie->terminal + (*node);
        parent = (*node);
        node = trie->nodes + (trie->count * (*node)) + trie->hash(array + (i * trie->size));
    }
    (*term) = true;
}

bool contains_itrie(const itrie_s trie, const void * array, const size_t length) {
    size_t node = trie.root;
    for (size_t i = 0; i < length; ++i) {
        const size_t hash = trie.hash(array + (i * trie.size));
        if (hash >= trie.count || NIL == node) {
            return false;
        }

        node = trie.nodes[(trie.count * node) + hash];
    }

    // node is only NIL if array is empty (length is zero), which is a 'valid' array of elements
    // else array is only valid if terminal at node is set to true
    return (NIL == node) || trie.terminal[node];
}

void remove_itrie(itrie_s * trie, const void * array, const size_t length, const destroy_fn destroy) {
}
