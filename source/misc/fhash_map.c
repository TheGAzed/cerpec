#include <misc/fhash_map.h>

#include <assert.h>
#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

void _fhash_map_fill_hole(fhash_map_s const * const map, size_t const hole);

fhash_map_s create_fhash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key) {
    assert(hash_key && "[ERROR] Parameter can't be NULL.");
    assert(compare_key && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");

    fhash_map_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .max = max,
        .allocator = &standard, .compare_key = compare_key,

        .head = standard.alloc(max * sizeof(size_t), standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
        .prev = standard.alloc(max * sizeof(size_t), standard.arguments),

        .keys = standard.alloc(max * key_size, standard.arguments),
        .values = standard.alloc(max * value_size, standard.arguments),
        .hashes = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    assert(table.head && "[ERROR] Memory allocation failed.");
    assert(table.next && "[ERROR] Memory allocation failed.");
    assert(table.prev && "[ERROR] Memory allocation failed.");
    assert(table.keys && "[ERROR] Memory allocation failed.");
    assert(table.values && "[ERROR] Memory allocation failed.");
    assert(table.hashes && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = NIL;
    }

    return table;
}

fhash_map_s make_fhash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator) {
    assert(hash_key && "[ERROR] Parameter can't be NULL.");
    assert(compare_key && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");
    assert(allocator && "[ERROR] Parameter can't be NULL.");

    fhash_map_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .max = max,
        .allocator = allocator, .compare_key = compare_key,

        .head = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .prev = allocator->alloc(max * sizeof(size_t), allocator->arguments),

        .keys = allocator->alloc(max * key_size, allocator->arguments),
        .values = allocator->alloc(max * value_size, allocator->arguments),
        .hashes = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    assert(table.head && "[ERROR] Memory allocation failed.");
    assert(table.next && "[ERROR] Memory allocation failed.");
    assert(table.prev && "[ERROR] Memory allocation failed.");
    assert(table.keys && "[ERROR] Memory allocation failed.");
    assert(table.values && "[ERROR] Memory allocation failed.");
    assert(table.hashes && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = NIL;
    }

    return table;
}

void destroy_fhash_map(fhash_map_s * const map, set_fn const destroy_key, set_fn const destroy_value) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < map->length; ++i) {
        destroy_key(map->keys + (i * map->key_size));
        destroy_value(map->values + (i * map->value_size));
    }

    // free arrays
    map->allocator->free(map->keys, map->allocator->arguments);
    map->allocator->free(map->values, map->allocator->arguments);
    map->allocator->free(map->hashes, map->allocator->arguments);

    map->allocator->free(map->head, map->allocator->arguments);
    map->allocator->free(map->next, map->allocator->arguments);
    map->allocator->free(map->prev, map->allocator->arguments);

    // map everything to zero/false
    map->max = map->key_size = map->length = map->value_size = 0;
    map->hash_key = NULL;
    map->compare_key = NULL;
    map->keys = map->values = NULL;
    map->next = map->prev = map->hashes = NULL;
    map->allocator = NULL;
}

void clear_fhash_map(fhash_map_s * map, const set_fn destroy_key, const set_fn destroy_value) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < map->length; ++i) {
        destroy_key(map->keys + (i * map->key_size));
        destroy_value(map->values + (i * map->value_size));
    }

    for (size_t i = 0; i < map->max; ++i) {
        map->head[i] = NIL;
    }

    // only clear map (keep the map usable)
    map->length = 0;
}

fhash_map_s copy_fhash_map(fhash_map_s const * const map, copy_fn const copy_key, copy_fn const copy_value) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(copy_key && "[ERROR] Parameter can't be NULL.");
    assert(copy_value && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    fhash_map_s const replica = {
        .max = map->max, .hash_key = map->hash_key, .length = map->length,
        .key_size = map->key_size, .value_size = map->value_size,

        .keys = map->allocator->alloc(map->max * map->key_size, map->allocator->arguments),
        .values = map->allocator->alloc(map->max * map->value_size, map->allocator->arguments),
        .hashes = map->allocator->alloc(map->max * sizeof(size_t), map->allocator->arguments),

        .head = map->allocator->alloc(map->max * sizeof(size_t), map->allocator->arguments),
        .next = map->allocator->alloc(map->max * sizeof(size_t), map->allocator->arguments),
        .prev = map->allocator->alloc(map->max * sizeof(size_t), map->allocator->arguments),

        .allocator = map->allocator, .compare_key = map->compare_key,
    };
    assert(replica.keys && "[ERROR] Memory allocation failed.");
    assert(replica.values && "[ERROR] Memory allocation failed.");
    assert(replica.hashes && "[ERROR] Memory allocation failed.");
    assert(replica.head && "[ERROR] Memory allocation failed.");
    assert(replica.next && "[ERROR] Memory allocation failed.");
    assert(replica.prev && "[ERROR] Memory allocation failed.");

    memcpy(replica.head, map->head, map->max * sizeof(size_t)); // capacity since heads can't be move

    memcpy(replica.next, map->next, map->length * sizeof(size_t));
    memcpy(replica.prev, map->prev, map->length * sizeof(size_t));
    memcpy(replica.hashes, map->hashes, map->length * sizeof(size_t));

    for (size_t i = 0; i < map->length; ++i) {
        copy_key(replica.keys + (i * replica.key_size), map->keys + (i * map->key_size));
        copy_value(replica.values + (i * replica.value_size), map->values + (i * map->value_size));
    }

    return replica;
}

bool is_empty_fhash_map(fhash_map_s const * const map) {
    assert(map && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    return !(map->length); // if 0 return 'true'
}

bool is_full_fhash_map(fhash_map_s const * const map) {
    assert(map && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    return (map->length == map->max); // if 0 return 'true'
}

void insert_fhash_map(fhash_map_s * const map, void const * const restrict key, void const * const restrict value) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // check if element is in map or not
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        void const * const current_key = map->keys + (n * map->key_size);
        error((hash != map->hashes[n] || map->compare_key(key, current_key)) && "Element already in map.");
    }

    size_t const current = map->length; // index of currently inserted element

    // if head has an element then redirect its prev to current
    size_t const head = map->head[index];
    if (NIL != head) {
        map->prev[head] = current;
    }

    // node index redirection
    map->prev[current] = NIL;
    map->next[current] = head;
    map->head[index] = current;

    // copy element into elements array
    map->hashes[current] = hash;
    memcpy(map->keys + (current * map->key_size), key, map->key_size);
    memcpy(map->values + (current * map->value_size), value, map->value_size);

    map->length++;
}

void remove_fhash_map(fhash_map_s * const map, void const * const restrict key, void * const restrict key_buffer, void * const restrict value_buffer) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(key_buffer && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != key_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key_buffer != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(map->length && "[ERROR] Structure is empty.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        const char * current_key = map->keys + (n * map->key_size);
        if (hash != map->hashes[n] || map->compare_key(key, current_key)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(key_buffer, current_key, map->key_size);
        memcpy(value_buffer, map->values + (n * map->value_size), map->value_size);
        map->length--;

        _fhash_map_fill_hole(map, n);

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_fhash_map(fhash_map_s const * const map, void const * const key) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // for each node at index check if element is contained and return true or false
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        void const * const current_key = map->keys + (n * map->key_size);
        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            return true;
        }
    }

    return false;
}

void get_value_fhash_map(fhash_map_s const * const map, void const * const restrict key, void * const restrict value_buffer) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(map->length && "[ERROR] Structure is empty.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // for each node at index check if element is contained
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        const char * current_key = map->keys + (n * map->key_size);
        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            memcpy(value_buffer, map->values + (n * map->value_size), map->value_size); // copy retrieved element into buffer
            return; // return to avoid errorion and termination at the end of function if key wasn't found
        }
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void set_value_fhash_map(fhash_map_s const * const map, void const * const restrict key, void const * const restrict value, void * const restrict value_buffer) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value && "[ERROR] Parameters can't be the same.");
    assert(value != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(map->length && "[ERROR] Structure is empty.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // for each node at index check if element is contained
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        const char * current_key = map->keys + (n * map->key_size);

        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            void * current_value = map->values + (n * map->value_size);
            memcpy(value_buffer, current_value, map->value_size); // copy retrieved element into buffer
            memcpy(current_value, value, map->value_size);
            return; // return to avoid errorion and termination at the end of function if key wasn't found
        }
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void map_key_fhash_map(fhash_map_s const * const map, handle_fn const handle, void * const arguments) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid key in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->keys + (i * map->key_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void map_value_fhash_map(fhash_map_s const * const map, handle_fn const handle, void * const arguments) {
    assert(map && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(map->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(map->key_size && "[INVALID] Parameter can't be zero.");
    assert(map->value_size && "[INVALID] Parameter can't be zero.");
    assert(map->length <= map->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(map->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid value in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->values + (i * map->value_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void _fhash_map_fill_hole(fhash_map_s const * const map, size_t const hole) {
    if (NIL == map->prev[map->length]) {
        size_t const index = map->hashes[map->length] % map->max;
        map->head[index] = hole;
    }

    if (NIL == map->prev[hole]) {
        size_t const index = map->hashes[hole] % map->max;
        map->head[index] = map->next[hole];
    }

    // cut current removed node's siblings from itself
    if (NIL != map->prev[hole]) { map->next[map->prev[hole]] = map->next[hole]; }
    if (NIL != map->next[hole]) { map->prev[map->next[hole]] = map->prev[hole]; }

    // cut current removed node from its siblings
    map->next[hole] = map->prev[hole] = hole;

    // replace element at current index with popped last element like in a stack
    memmove(map->keys + (hole * map->key_size), map->keys + (map->length * map->key_size), map->key_size);
    memmove(map->values + (hole * map->value_size), map->values + (map->length * map->value_size), map->value_size);
    map->hashes[hole] = map->hashes[map->length];
    map->next[hole] = map->next[map->length];
    map->prev[hole] = map->prev[map->length];

    // redirect array's last swapped node's siblings to hole
    if (NIL != map->next[map->length]) { map->prev[map->next[map->length]] = hole; }
    if (NIL != map->prev[map->length]) { map->next[map->prev[map->length]] = hole; }
}
