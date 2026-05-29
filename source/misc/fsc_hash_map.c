#include <misc/fsc_hash_map.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

/// @brief Fills hole produced by element removal in arrays of elements.
/// @param map Structure to remove hole from.
/// @param hole Index of hole in arrays.
void _fsc_hash_map_fill_hole(fsc_hash_map_s const * const map, size_t const hole);

fsc_hash_map_s create_fsc_hash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key) {
    error(hash_key && "Parameter can't be NULL.");
    error(compare_key && "Parameter can't be NULL.");
    error(key_size && "Parameter can't be zero.");
    error(value_size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");

    fsc_hash_map_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .max = max,
        .allocator = &standard, .compare_key = compare_key,

        .head = standard.alloc(max * sizeof(size_t), standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
        .prev = standard.alloc(max * sizeof(size_t), standard.arguments),

        .keys = standard.alloc(max * key_size, standard.arguments),
        .values = standard.alloc(max * value_size, standard.arguments),
        .hashes = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    error(table.head && "Memory allocation failed.");
    error(table.next && "Memory allocation failed.");
    error(table.prev && "Memory allocation failed.");
    error(table.keys && "Memory allocation failed.");
    error(table.values && "Memory allocation failed.");
    error(table.hashes && "Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = NIL;
    }

    return table;
}

fsc_hash_map_s make_fsc_hash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator) {
    error(hash_key && "Parameter can't be NULL.");
    error(compare_key && "Parameter can't be NULL.");
    error(key_size && "Parameter can't be zero.");
    error(value_size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");
    error(allocator && "Parameter can't be NULL.");

    fsc_hash_map_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .max = max,
        .allocator = allocator, .compare_key = compare_key,

        .head = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .prev = allocator->alloc(max * sizeof(size_t), allocator->arguments),

        .keys = allocator->alloc(max * key_size, allocator->arguments),
        .values = allocator->alloc(max * value_size, allocator->arguments),
        .hashes = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    error(table.head && "Memory allocation failed.");
    error(table.next && "Memory allocation failed.");
    error(table.prev && "Memory allocation failed.");
    error(table.keys && "Memory allocation failed.");
    error(table.values && "Memory allocation failed.");
    error(table.hashes && "Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = NIL;
    }

    return table;
}

void destroy_fsc_hash_map(fsc_hash_map_s * const map, set_fn const destroy_key, set_fn const destroy_value) {
    error(map && "Parameter can't be NULL.");
    error(destroy_key && "Parameter can't be NULL.");
    error(destroy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

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
    memset(map, 0, sizeof(fsc_hash_map_s));
}

void clear_fsc_hash_map(fsc_hash_map_s * map, const set_fn destroy_key, const set_fn destroy_value) {
    error(map && "Parameter can't be NULL.");
    error(destroy_key && "Parameter can't be NULL.");
    error(destroy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

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

fsc_hash_map_s copy_fsc_hash_map(fsc_hash_map_s const * const map, copy_fn const copy_key, copy_fn const copy_value) {
    error(map && "Parameter can't be NULL.");
    error(copy_key && "Parameter can't be NULL.");
    error(copy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    fsc_hash_map_s const replica = {
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
    error(replica.keys && "Memory allocation failed.");
    error(replica.values && "Memory allocation failed.");
    error(replica.hashes && "Memory allocation failed.");
    error(replica.head && "Memory allocation failed.");
    error(replica.next && "Memory allocation failed.");
    error(replica.prev && "Memory allocation failed.");

    memcpy(replica.head, map->head, map->max * sizeof(size_t)); // maximum since heads can't be moved

    memcpy(replica.next, map->next, map->length * sizeof(size_t));
    memcpy(replica.prev, map->prev, map->length * sizeof(size_t));
    memcpy(replica.hashes, map->hashes, map->length * sizeof(size_t));

    for (size_t i = 0; i < map->length; ++i) {
        copy_key(replica.keys + (i * replica.key_size), map->keys + (i * map->key_size));
        copy_value(replica.values + (i * replica.value_size), map->values + (i * map->value_size));
    }

    return replica;
}

bool is_empty_fsc_hash_map(fsc_hash_map_s const * const map) {
    error(map && "Parameter can't be NULL.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    return !(map->length); // if 0 return 'true'
}

bool is_full_fsc_hash_map(fsc_hash_map_s const * const map) {
    error(map && "Parameter can't be NULL.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    return (map->length == map->max); // if 0 return 'true'
}

void insert_fsc_hash_map(fsc_hash_map_s * const restrict map, void const * const restrict key, void const * const restrict value) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value && "Parameter can't be NULL.");
    error(map != key && "Parameters can't be equal.");
    error(map != value && "Parameters can't be equal.");
    error(value != key && "Parameters can't be equal.");
    error(map->length != map->max && "Structure is full.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

#ifndef NERROR
    // check if element is in map or not
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        void const * const current_key = map->keys + (n * map->key_size);
        error((hash != map->hashes[n] || map->compare_key(key, current_key)) && "Element already in map.");
    }
#endif

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

void remove_fsc_hash_map(fsc_hash_map_s * const restrict map, void const * const restrict key, void * const restrict key_buffer, void * const restrict value_buffer) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(key_buffer && "Parameter can't be NULL.");
    error(value_buffer && "Parameter can't be NULL.");
    error(key != key_buffer && "Parameters can't be the same.");
    error(key != value_buffer && "Parameters can't be the same.");
    error(key_buffer != value_buffer && "Parameters can't be the same.");
    error(map->length && "Structure is empty.");

    error(map != key && "Parameters can't be equal.");
    error(map != key_buffer && "Parameters can't be equal.");
    error(map != value_buffer && "Parameters can't be equal.");
    error(key != key_buffer && "Parameters can't be equal.");
    error(key != value_buffer && "Parameters can't be equal.");
    error(key_buffer != value_buffer && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        char const * current_key = map->keys + (n * map->key_size);
        if (hash != map->hashes[n] || map->compare_key(key, current_key)) { // if not equal continue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(key_buffer, current_key, map->key_size);
        memcpy(value_buffer, map->values + (n * map->value_size), map->value_size);
        map->length--;

        _fsc_hash_map_fill_hole(map, n);

        return; // return to avoid errorion and termination at the end of function if element wasn't found
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_fsc_hash_map(fsc_hash_map_s const * const restrict map, void const * const restrict key) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(map != key && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

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

void get_value_fsc_hash_map(fsc_hash_map_s const * const restrict map, void const * const restrict key, void * const restrict value_buffer) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value_buffer && "Parameter can't be NULL.");
    error(key != value_buffer && "Parameters can't be the same.");
    error(map->length && "Structure is empty.");

    error(map != key && "Parameters can't be equal.");
    error(map != value_buffer && "Parameters can't be equal.");
    error(value_buffer != key && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // for each node at index check if element is contained
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        char const * current_key = map->keys + (n * map->key_size);
        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            memcpy(value_buffer, map->values + (n * map->value_size), map->value_size); // copy retrieved element into buffer
            return; // return to avoid errorion and termination at the end of function if key wasn't found
        }
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void set_fsc_hash_map(fsc_hash_map_s * const restrict map, void const * const restrict key, void const * const restrict value, void * const restrict value_buffer) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value_buffer && "Parameter can't be NULL.");
    error(key != value_buffer && "Parameters can't be the same.");
    error(key != value && "Parameters can't be the same.");
    error(value != value_buffer && "Parameters can't be the same.");

    error(map != key && "Parameters can't be equal.");
    error(map != value && "Parameters can't be equal.");
    error(map != value_buffer && "Parameters can't be equal.");
    error(key != value && "Parameters can't be equal.");
    error(key != value_buffer && "Parameters can't be equal.");
    error(value != value_buffer && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->max;

    // for each node at index check if element is contained
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        char const * current_key = map->keys + (n * map->key_size);

        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            void * current_value = map->values + (n * map->value_size);
            memcpy(value_buffer, current_value, map->value_size); // copy retrieved element into buffer
            memcpy(current_value, value, map->value_size);
            return; // return to avoid insertion at the end of function if key wasn't found
        }
    }

    error(map->length != map->max && "Structure is full.");

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

void each_key_fsc_hash_map(fsc_hash_map_s const * const restrict map, handle_fn const handle, void * const restrict arguments) {
    error(map && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(map != arguments && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // iterate over each valid key in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->keys + (i * map->key_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void each_value_fsc_hash_map(fsc_hash_map_s const * const restrict map, handle_fn const handle, void * const restrict arguments) {
    error(map && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(map != arguments && "Parameters can't be equal.");

    valid(map->hash_key && "Parameter can't be NULL.");
    valid(map->key_size && "Parameter can't be zero.");
    valid(map->value_size && "Parameter can't be zero.");
    valid(map->length <= map->max && "Lenght can't be larger than maximum.");
    valid(map->allocator && "Paremeter can't be NULL.");

    // iterate over each valid value in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->values + (i * map->value_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void _fsc_hash_map_fill_hole(fsc_hash_map_s const * const map, size_t const hole) {
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
