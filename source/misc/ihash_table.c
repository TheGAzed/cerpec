#include <misc/ihash_table.h>

#include <assert.h>
#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

/// Resizes (reallocates) table parameter arrays based on changed capacity.
/// @param table Structure to resize.
/// @param size New size.
void _ihash_table_resize(ihash_table_s * const table, size_t const size);

ihash_table_s create_ihash_table(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key)  {
    assert(hash_key && "[ERROR] Parameter can't be NULL.");
    assert(compare_key && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");

    return (ihash_table_s) {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .empty = NIL, .compare_key = compare_key,
        .allocator = &standard,
    };
}

ihash_table_s make_ihash_table(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key , memory_s const * const allocator) {
    assert(hash_key && "[ERROR] Parameter can't be NULL.");
    assert(compare_key && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");
    assert(allocator && "[ERROR] Parameter can't be NULL.");

    return (ihash_table_s) {
        .key_size = key_size, .value_size = value_size, .hash_key = hash_key, .empty = NIL, .compare_key = compare_key,
        .allocator = allocator,
    };
}

void destroy_ihash_table(ihash_table_s * const table, set_fn const destroy_key, set_fn const destroy_value)  {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    // free arrays
    table->allocator->free(table->head, table->allocator->arguments);
    table->allocator->free(table->next, table->allocator->arguments);
    table->allocator->free(table->hashes, table->allocator->arguments);
    table->allocator->free(table->keys, table->allocator->arguments);
    table->allocator->free(table->values, table->allocator->arguments);

    // table everything to zero/false
    table->capacity = table->empty = table->key_size = table->length = table->value_size = 0;
    table->hash_key = NULL;
    table->compare_key = NULL;
    table->keys = table->values = NULL;
    table->head = table->next = table->hashes = NULL;
    table->allocator = NULL;
}

void clear_ihash_table(ihash_table_s * const table, set_fn const destroy_key, set_fn const destroy_value)  {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    // free arrays
    table->allocator->free(table->keys, table->allocator->arguments);
    table->allocator->free(table->values, table->allocator->arguments);
    table->allocator->free(table->hashes, table->allocator->arguments);
    table->allocator->free(table->head, table->allocator->arguments);
    table->allocator->free(table->next, table->allocator->arguments);

    // only clear table (keep the table usable)
    table->empty = NIL;
    table->capacity = table->length = 0;
    table->head = table->next = table->hashes = NULL;
    table->keys = table->values = NULL;
}

ihash_table_s copy_ihash_table(ihash_table_s const * const table, copy_fn const copy_key, copy_fn const copy_value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(copy_key && "[ERROR] Parameter can't be NULL.");
    assert(copy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    ihash_table_s const replica = {
        .capacity = table->capacity, .empty = NIL, .hash_key = table->hash_key, .length = table->length,
        .key_size = table->key_size, .value_size = table->value_size, .compare_key = table->compare_key,

        .keys = table->allocator->alloc(table->capacity * table->key_size, table->allocator->arguments),
        .values = table->allocator->alloc(table->capacity * table->value_size, table->allocator->arguments),
        .hashes = table->allocator->alloc(table->capacity * sizeof(size_t), table->allocator->arguments),

        .head = table->allocator->alloc(table->capacity * sizeof(size_t), table->allocator->arguments),
        .next = table->allocator->alloc(table->capacity * sizeof(size_t), table->allocator->arguments),

        .allocator = table->allocator,
    };
    assert((!replica.capacity || replica.keys) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.values) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.hashes) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.head) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.next) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < table->capacity; ++i) {
        replica.head[i] = replica.next[i] = NIL; // initially table replica heads to invalid
    }

    // for each index, if each index is valid node then push it into replica
    for (size_t i = 0, hole = 0; i < table->capacity; ++i) {
        // if table has elements in head then push them into replica heads (like a stack)
        for (size_t n = table->head[i]; NIL != n; n = table->next[n], hole++) {
            copy_key(replica.keys + (hole * replica.key_size), table->keys + (n * table->key_size));
            copy_value(replica.values + (hole * replica.value_size), table->values + (n * table->value_size));
            replica.hashes[hole] = table->hashes[n];

            // node index redirection
            replica.next[hole] = replica.head[i];
            replica.head[i] = hole;
        }
    }

    return replica;
}

bool is_empty_ihash_table(ihash_table_s const * const table) {
    assert(table && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    return !(table->length); // if 0 return 'true'
}

void insert_ihash_table(ihash_table_s * const table, void const * const key, void const * const value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // resize (expand) if table can't contain new element
    if (table->length == table->capacity) {
        _ihash_table_resize(table, table->capacity + IHASH_TABLE_CHUNK);
    }

    // calculate hash values and index in array
    size_t const hash = table->hash_key(key);
    size_t const index = hash % table->capacity;

    // check if element is in table or not
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        void const * const current_key = table->keys + (n * table->key_size);
        assert((hash != table->hashes[n] || table->compare_key(key, current_key)) && "[ERROR] Element already in table.");
    }

    // get hole/empty index
    size_t hole = table->length;
    if (NIL != table->empty) { // if empty has a valid index then pop it and table it as hole
        hole = table->empty;
        table->empty = table->next[table->empty];
    }

    // node index redirection
    table->next[hole] = table->head[index];
    table->head[index] = hole;

    // copy element into elements array
    table->hashes[hole] = hash;
    memcpy(table->keys + (hole * table->key_size), key, table->key_size);
    memcpy(table->values + (hole * table->value_size), value, table->value_size);

    table->length++;
}

void remove_ihash_table(ihash_table_s * const restrict table, void const * const restrict key, void * const restrict key_buffer, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(key_buffer && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != key_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key_buffer != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = table->hash_key(key);
    size_t const index = hash % table->capacity;

    // for each node at index check if element is contained
    for (size_t * n = table->head + index; NIL != (*n); n = table->next + (*n)) {
        const char * current_key = table->keys + ((*n) * table->key_size);
        if (hash != table->hashes[(*n)] || table->compare_key(key, current_key)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(key_buffer, current_key, table->key_size);
        memcpy(value_buffer, table->values + ((*n) * table->value_size), table->value_size);
        table->length--;

        // redirect node using pointer magic
        const size_t hole = (*n);
        (*n) = table->next[(*n)];

        // if hole isn't last index in array append it into empty stack
        if (hole != table->length) {
            table->next[hole] = table->empty;
            table->empty = hole;
        }

        // resize (expand) if table can contain a smaller capacity of elements
        if (table->capacity - IHASH_TABLE_CHUNK == table->length) {
            _ihash_table_resize(table, table->capacity - IHASH_TABLE_CHUNK);
        }

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_ihash_table(ihash_table_s const * const table, void const * const key) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // early return to avoid 'x mod 0' by capacity
    if (!table->capacity) {
        return false;
    }

    // calculate hash values and index in array
    size_t const hash = table->hash_key(key);
    size_t const index = hash % table->capacity;

    // for each node at index check if element is contained and return true or false
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        void const * const current_key = table->keys + (n * table->key_size);
        if (hash == table->hashes[n] && !table->compare_key(key, current_key)) {
            return true;
        }
    }

    return false;
}

void get_value_ihash_table(ihash_table_s const * const table, void const * const restrict key, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash values and index in array
    size_t const hash = table->hash_key(key);
    size_t const index = hash % table->capacity;

    // for each node at index check if element is contained
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        const char * current_key = table->keys + (n * table->key_size);
        if (hash == table->hashes[n] && !table->compare_key(key, current_key)) {
            memcpy(value_buffer, table->values + (n * table->value_size), table->value_size); // copy retrieved element into buffer
            return; // return to avoid assertion and termination at the end of function if key wasn't found
        }
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void set_value_ihash_table(ihash_table_s const * const table, void const * const restrict key, void const * const restrict value, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(value != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash values and index in array
    size_t const hash = table->hash_key(key);
    size_t const index = hash % table->capacity;

    // for each node at index check if element is contained
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        const char * current_key = table->keys + (n * table->key_size);

        if (hash == table->hashes[n] && !table->compare_key(key, current_key)) {
            void * current_value = table->values + (n * table->value_size);
            memcpy(value_buffer, current_value, table->value_size); // copy retrieved element into buffer
            memcpy(current_value, value, table->value_size);
            return; // return to avoid assertion and termination at the end of function if key wasn't found
        }
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void map_key_ihash_table(ihash_table_s const * const table, handle_fn const handle, void * const arguments) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid key in lists
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            if (!handle(table->keys + (n * table->key_size), arguments)) {
                return; // return since we need to break-off of two loops
            }
        }
    }
}

void map_value_ihash_table(ihash_table_s const * const table, handle_fn const handle, void * const arguments) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(table->hash_key && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid value in lists
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            if (!handle(table->values + (n * table->value_size), arguments)) {
                return; // return since we need to break-off of two loops
            }
        }
    }
}

void _ihash_table_resize(ihash_table_s * const table, size_t const size) {
    // allocate new array for keys, values and hashes based on new capacity
    char * keys = table->allocator->alloc(size * table->key_size, table->allocator->arguments);
    assert((!size || keys) && "[ERROR] Memory allocation failed.");

    char * values = table->allocator->alloc(size * table->value_size, table->allocator->arguments);
    assert((!size || values) && "[ERROR] Memory allocation failed.");

    size_t * hashes = table->allocator->alloc(size * sizeof(size_t), table->allocator->arguments);
    assert((!size || hashes) && "[ERROR] Memory allocation failed.");

    // for each valid key-value copy it straight into new arrays
    for (size_t i = 0, index = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            hashes[index] = table->hashes[n];
            memcpy(keys + (index * table->key_size), table->keys + (n * table->key_size), table->key_size);
            memcpy(values + (index * table->value_size), table->values + (n * table->value_size), table->value_size);

            index++;
        }
    }

    // free up previous keys and values array
    table->allocator->free(table->keys, table->allocator->arguments);
    table->allocator->free(table->values, table->allocator->arguments);
    table->allocator->free(table->hashes, table->allocator->arguments);

    // set table to new resized parameters
    table->capacity = size;
    table->head     = table->allocator->realloc(table->head, size * sizeof(size_t), table->allocator->arguments);
    table->next     = table->allocator->realloc(table->next, size * sizeof(size_t), table->allocator->arguments);
    table->keys     = keys;
    table->values   = values;
    table->hashes   = hashes;

    // set empty stack index and heads to NIL (make them invalid)
    table->empty = NIL;
    for (size_t i = 0; i < table->capacity; ++i) {
        table->head[i] = table->next[i] = NIL;
    }

    // reset lists by pushing hashes to their valid list
    for (size_t i = 0; i < table->length; ++i) {
        const size_t index = table->hashes[i] % table->capacity;

        table->next[i] = table->head[index];
        table->head[index] = i;
    }
}
