#include <misc/ihash_map.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

/// Resizes (reallocates) table parameter arrays based on changed capacity.
/// @param table Structure to resize.
/// @param size New size.
void _ihash_table_resize(ihash_map_s * const table, size_t const size);

/// @brief Fills hole/empty node index with last array-based node and fixes/redirects siblings.
/// @param map Strcuture to fill.
/// @param hole Index of hole node.
void _ihash_map_fill_hole(ihash_map_s const * const map, size_t const hole);

ihash_map_s create_ihash_map(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key) {
    error(hash_key && "Parameter can't be NULL.");
    error(compare_key && "Parameter can't be NULL.");
    error(key_size && "Parameter can't be zero.");
    error(value_size && "Parameter can't be zero.");

    return (ihash_map_s) {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key,
        .compare_key = compare_key, .allocator = &standard,
    };
}

ihash_map_s make_ihash_map(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator) {
    error(hash_key && "Parameter can't be NULL.");
    error(compare_key && "Parameter can't be NULL.");
    error(key_size && "Parameter can't be zero.");
    error(value_size && "Parameter can't be zero.");
    error(allocator && "Parameter can't be NULL.");

    return (ihash_map_s) {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .compare_key = compare_key,
        .allocator = allocator,
    };
}

void destroy_ihash_map(ihash_map_s * const map, set_fn const destroy_key, set_fn const destroy_value) {
    error(map && "Parameter can't be NULL.");
    error(destroy_key && "Parameter can't be NULL.");
    error(destroy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // for each element since they are all layered continuosly in array
    for (size_t i = 0; i < map->length; ++i) {
        destroy_key(map->keys + (i * map->key_size));
        destroy_value(map->values + (i * map->value_size));
    }

    // free arrays
    map->allocator->free(map->head, map->allocator->arguments);
    map->allocator->free(map->next, map->allocator->arguments);
    map->allocator->free(map->prev, map->allocator->arguments);
    map->allocator->free(map->hashes, map->allocator->arguments);
    map->allocator->free(map->keys, map->allocator->arguments);
    map->allocator->free(map->values, map->allocator->arguments);

    // map everything to zero/false
    memset(map, 0, sizeof(ihash_map_s));
}

void clear_ihash_map(ihash_map_s * map, const set_fn destroy_key, const set_fn destroy_value) {
    error(map && "Parameter can't be NULL.");
    error(destroy_key && "Parameter can't be NULL.");
    error(destroy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // for each element since they are all layered continuosly in array
    for (size_t i = 0; i < map->capacity; ++i) {
        for (size_t n = map->head[i]; NIL != n; n = map->next[n]) {
            destroy_key(map->keys + (n * map->key_size));
            destroy_value(map->values + (n * map->value_size));
        }
    }

    // free arrays
    map->allocator->free(map->keys, map->allocator->arguments);
    map->allocator->free(map->values, map->allocator->arguments);
    map->allocator->free(map->hashes, map->allocator->arguments);
    map->allocator->free(map->head, map->allocator->arguments);
    map->allocator->free(map->next, map->allocator->arguments);
    map->allocator->free(map->prev, map->allocator->arguments);

    // only clear map (keep the map usable)
    map->capacity = map->length = 0;
    map->head = map->next = map->prev = map->hashes = NULL;
    map->keys = map->values = NULL;
}

ihash_map_s copy_ihash_map(ihash_map_s const * const map, copy_fn const copy_key, copy_fn const copy_value) {
    error(map && "Parameter can't be NULL.");
    error(copy_key && "Parameter can't be NULL.");
    error(copy_value && "Parameter can't be NULL.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // create replica with allocated memory based on capacity
    ihash_map_s const replica = {
        .capacity = map->capacity, .hash_key = map->hash_key, .length = map->length,
        .key_size = map->key_size, .value_size = map->value_size, .compare_key = map->compare_key,

        .keys = map->allocator->alloc(map->capacity * map->key_size, map->allocator->arguments),
        .values = map->allocator->alloc(map->capacity * map->value_size, map->allocator->arguments),
        .hashes = map->allocator->alloc(map->capacity * sizeof(size_t), map->allocator->arguments),

        .head = map->allocator->alloc(map->capacity * sizeof(size_t), map->allocator->arguments),
        .next = map->allocator->alloc(map->capacity * sizeof(size_t), map->allocator->arguments),
        .prev = map->allocator->alloc(map->capacity * sizeof(size_t), map->allocator->arguments),

        .allocator = map->allocator,
    };
    error((!replica.capacity || replica.keys) && "Memory allocation failed.");
    error((!replica.capacity || replica.values) && "Memory allocation failed.");
    error((!replica.capacity || replica.hashes) && "Memory allocation failed.");
    error((!replica.capacity || replica.head) && "Memory allocation failed.");
    error((!replica.capacity || replica.next) && "Memory allocation failed.");
    error((!replica.capacity || replica.prev) && "Memory allocation failed.");

    memcpy(replica.head, map->head, map->capacity * sizeof(size_t)); // capacity since heads can't be move

    memcpy(replica.next, map->next, map->length * sizeof(size_t));
    memcpy(replica.prev, map->prev, map->length * sizeof(size_t));
    memcpy(replica.hashes, map->hashes, map->length * sizeof(size_t));

    for (size_t i = 0; i < map->length; ++i) {
        copy_key(replica.keys + (i * replica.key_size), map->keys + (i * map->key_size));
        copy_value(replica.values + (i * replica.value_size), map->values + (i * map->value_size));
    }

    return replica;
}

bool is_empty_ihash_map(ihash_map_s const * const map) {
    error(map && "Parameter can't be NULL.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    return !(map->length); // if 0 return 'true'
}

void insert_ihash_map(ihash_map_s * const restrict map, void const * const restrict key, void const * const restrict value) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value && "Parameter can't be NULL.");
    error(map != key && "Parameters can't be equal.");
    error(map != value && "Parameters can't be equal.");
    error(value != key && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // resize (expand) if map can't contain new element
    if (map->length == map->capacity) {
        _ihash_table_resize(map, map->capacity + IHASH_MAP_CHUNK);
    }

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->capacity;

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

void remove_ihash_map(ihash_map_s * const restrict map, void const * const restrict key, void * const restrict key_buffer, void * const restrict value_buffer) {
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

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->capacity;

    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        const char * current_key = map->keys + (n * map->key_size);
        if (hash != map->hashes[n] || map->compare_key(key, current_key)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(key_buffer, current_key, map->key_size);
        memcpy(value_buffer, map->values + (n * map->value_size), map->value_size);
        map->length--;

        _ihash_map_fill_hole(map, n);

        // resize (expand) if map can contain a smaller capacity of elements
        if (map->capacity - IHASH_MAP_CHUNK == map->length) {
            _ihash_table_resize(map, map->capacity - IHASH_MAP_CHUNK);
        }

        return; // return to avoid errorion and termination at the end of function if element wasn't found
    }

    error(false && "Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_ihash_map(ihash_map_s const * const restrict map, void const * const restrict key) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(map != key && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // early return to avoid 'x mod 0' by capacity
    if (!map->capacity) { return false; }

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->capacity;

    // for each node at index check if element is contained and return true or false
    for (size_t n = map->head[index]; NIL != n; n = map->next[n]) {
        void const * const current_key = map->keys + (n * map->key_size);
        if (hash == map->hashes[n] && !map->compare_key(key, current_key)) {
            return true;
        }
    }

    return false;
}

void get_value_ihash_map(ihash_map_s const * const restrict map, void const * const restrict key, void * const restrict value_buffer) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value_buffer && "Parameter can't be NULL.");
    error(key != value_buffer && "Parameters can't be the same.");
    error(map->length && "Structure is empty.");
    error(map != key && "Parameters can't be equal.");
    error(map != value_buffer && "Parameters can't be equal.");
    error(value_buffer != key && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->capacity;

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

void set_value_ihash_map(ihash_map_s const * const restrict map, void const * const restrict key, void const * const restrict value, void * const restrict value_buffer) {
    error(map && "Parameter can't be NULL.");
    error(key && "Parameter can't be NULL.");
    error(value_buffer && "Parameter can't be NULL.");
    error(key != value_buffer && "Parameters can't be the same.");
    error(value != value_buffer && "Parameters can't be the same.");
    error(key != value && "Parameters can't be the same.");
    error(map->length && "Structure is empty.");

    error(map != key && "Parameters can't be equal.");
    error(map != value && "Parameters can't be equal.");
    error(map != value_buffer && "Parameters can't be equal.");
    error(key != value && "Parameters can't be equal.");
    error(key != value_buffer && "Parameters can't be equal.");
    error(value != value_buffer && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = map->hash_key(key);
    size_t const index = hash % map->capacity;

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

void each_key_ihash_map(ihash_map_s const * const restrict map, handle_fn const handle, void * const restrict arguments) {
    error(map && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(map != arguments && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // iterate over each valid key in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->keys + (i * map->key_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void each_value_ihash_map(ihash_map_s const * const restrict map, handle_fn const handle, void * const restrict arguments) {
    error(map && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(map != arguments && "Parameters can't be equal.");

    valid(map->hash_key && "Hash function can't be NULL.");
    valid(map->key_size && "Key size can't be zero.");
    valid(map->value_size && "Value size can't be zero.");
    valid(map->length <= map->capacity && "Lenght can't be larger than capacity.");
    valid(map->allocator && "Allocator can't be NULL.");

    // iterate over each valid value in lists (since they're sequentially we only need to iterate through values array)
    for (size_t i = 0; i < map->length; ++i) {
        if (!handle(map->values + (i * map->value_size), arguments)) {
            break; // return since we need to break-off of two loops
        }
    }
}

void _ihash_table_resize(ihash_map_s * const table, size_t const size) {
    // set table to new resized parameters
    table->capacity = size;

    table->head = table->allocator->realloc(table->head, size * sizeof(size_t), table->allocator->arguments);
    error((!table->capacity || table->head) && "Memory allocation failed.");

    table->next = table->allocator->realloc(table->next, size * sizeof(size_t), table->allocator->arguments);
    error((!table->capacity || table->next) && "Memory allocation failed.");

    table->prev = table->allocator->realloc(table->prev, size * sizeof(size_t), table->allocator->arguments);
    error((!table->capacity || table->prev) && "Memory allocation failed.");

    table->keys = table->allocator->realloc(table->keys, size * table->key_size, table->allocator->arguments);
    error((!table->capacity || table->keys) && "Memory allocation failed.");

    table->values = table->allocator->realloc(table->values, size * table->value_size, table->allocator->arguments);
    error((!table->capacity || table->values) && "Memory allocation failed.");

    table->hashes = table->allocator->realloc(table->hashes, size * sizeof(size_t), table->allocator->arguments);
    error((!table->capacity || table->hashes) && "Memory allocation failed.");

    for (size_t i = 0; i < table->capacity; ++i) {
        table->head[i] = NIL;
    }

    // reset lists by pushing hashes to their valid list
    for (size_t i = 0; i < table->length; ++i) {
        const size_t index = table->hashes[i] % table->capacity;

        size_t const head = table->head[index];
        if (NIL != head) {
            table->prev[head] = i;
        }

        // node index redirection
        table->prev[i] = NIL;
        table->next[i] = head;
        table->head[index] = i;
    }
}

void _ihash_map_fill_hole(ihash_map_s const * const map, size_t const hole) {
    if (NIL == map->prev[map->length]) {
        size_t const index = map->hashes[map->length] % map->capacity;
        map->head[index] = hole;
    }

    if (NIL == map->prev[hole]) {
        size_t const index = map->hashes[hole] % map->capacity;
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
