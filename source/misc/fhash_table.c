#include <misc/fhash_table.h>

#include <assert.h>
#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

fhash_table_s create_fhash_table(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash) {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");

    fhash_table_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash = hash, .empty = NIL, .max = max,
        .allocator = &standard,
        .head = standard.alloc(max * sizeof(size_t), standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
        .keys = standard.alloc(max * key_size, standard.arguments),
        .values = standard.alloc(max * value_size, standard.arguments),
    };
    assert(table.head && "[ERROR] Memory allocation failed.");
    assert(table.next && "[ERROR] Memory allocation failed.");
    assert(table.keys && "[ERROR] Memory allocation failed.");
    assert(table.values && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = table.next[i] = NIL;
    }

    return table;
}

fhash_table_s make_fhash_table(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash, memory_s const * const allocator) {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");
    assert(allocator && "[ERROR] Parameter can't be NULL.");

    fhash_table_s const table =  {
        .key_size = key_size, .value_size = value_size, .hash = hash, .empty = NIL, .max = max,
        .allocator = allocator,
        .head = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .keys = allocator->alloc(max * key_size, allocator->arguments),
        .values = allocator->alloc(max * value_size, allocator->arguments),
    };
    assert(table.head && "[ERROR] Memory allocation failed.");
    assert(table.next && "[ERROR] Memory allocation failed.");
    assert(table.keys && "[ERROR] Memory allocation failed.");
    assert(table.values && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        table.head[i] = table.next[i] = NIL;
    }

    return table;
}

void destroy_fhash_table(fhash_table_s * const table, set_fn const destroy_key, set_fn const destroy_value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->max; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    // free arrays
    table->allocator->free(table->head, table->allocator->arguments);
    table->allocator->free(table->keys, table->allocator->arguments);
    table->allocator->free(table->values, table->allocator->arguments);
    table->allocator->free(table->next, table->allocator->arguments);

    // table everything to zero/false
    table->max = table->empty = table->key_size = table->length = table->value_size = 0;
    table->hash = NULL;
    table->keys = table->values = NULL;
    table->next = NULL;
    table->allocator = NULL;
}

void clear_fhash_table(fhash_table_s * table, const set_fn destroy_key, const set_fn destroy_value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(destroy_key && "[ERROR] Parameter can't be NULL.");
    assert(destroy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->max; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    for (size_t i = 0; i < table->max; ++i) {
        table->head[i] = table->next[i] = NIL;
    }

    // only clear table (keep the table usable)
    table->empty = NIL;
    table->length = 0;
}

fhash_table_s copy_fhash_table(fhash_table_s const * const table, copy_fn const copy_key, copy_fn const copy_value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(copy_key && "[ERROR] Parameter can't be NULL.");
    assert(copy_value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    fhash_table_s const replica = {
        .max = table->max, .empty = NIL, .hash = table->hash, .length = table->length,
        .key_size = table->key_size, .value_size = table->value_size,

        .keys = table->allocator->alloc(table->max * table->key_size, table->allocator->arguments),
        .values = table->allocator->alloc(table->max * table->value_size, table->allocator->arguments),

        .head = table->allocator->alloc(table->max * sizeof(size_t), table->allocator->arguments),
        .next = table->allocator->alloc(table->max * sizeof(size_t), table->allocator->arguments),

        .allocator = table->allocator,
    };
    assert(replica.keys && "[ERROR] Memory allocation failed.");
    assert(replica.values && "[ERROR] Memory allocation failed.");
    assert(replica.head && "[ERROR] Memory allocation failed.");
    assert(replica.next && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < table->max; ++i) {
        replica.head[i] = replica.next[i] = NIL; // initially table replica heads to invalid
    }

    // for each index, if each index is valid node then push it into replica
    for (size_t i = 0, hole = 0; i < table->max; ++i) {
        // if table has elements in head then push them into replica heads (like a stack)
        for (size_t n = table->head[i]; NIL != n; n = table->next[n], hole++) {
            copy_key(replica.keys + (hole * replica.key_size), table->keys + (n * table->key_size));
            copy_value(replica.values + (hole * replica.value_size), table->values + (n * table->value_size));

            // node index redirection
            replica.next[hole] = replica.head[i];
            replica.head[i] = hole;
        }
    }

    return replica;
}

bool is_empty_fhash_table(fhash_table_s const * const table) {
    assert(table && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    return !(table->length); // if 0 return 'true'
}

bool is_full_fhash_table(fhash_table_s const * const table) {
    assert(table && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    return (table->length == table->max); // if 0 return 'true'
}

void insert_fhash_table(fhash_table_s * const table, void const * const restrict key, void const * const restrict value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = table->hash(key);
    size_t const index = hash % table->max;

    // check if element is in table or not
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        assert(hash != table->hash(table->keys + (n * table->key_size)) && "[ERROR] Element already in table.");
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
    memcpy(table->keys + (hole * table->key_size), key, table->key_size);
    memcpy(table->values + (hole * table->value_size), value, table->value_size);
    table->length++;
}

void remove_fhash_table(fhash_table_s * const table, void const * const restrict key, void * const restrict key_buffer, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(key_buffer && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != key_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key_buffer != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = table->hash(key);
    size_t const index = hash % table->max;

    // for each node at index check if element is contained
    for (size_t * n = table->head + index; NIL != (*n); n = table->next + (*n)) {
        const char * current = table->keys + ((*n) * table->key_size);
        if (hash != table->hash(current)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(key_buffer, current, table->key_size);
        memcpy(value_buffer, current, table->value_size);
        table->length--;

        // redirect node using pointer magic
        const size_t hole = (*n);
        (*n) = table->next[(*n)];

        // if hole isn't last index in array append it into empty stack
        if (hole != table->length) {
            table->next[hole] = table->empty;
            table->empty = hole;
        }

        if (!table->length) {
            table->empty = NIL;
        }

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_fhash_table(fhash_table_s const * const table, void const * const key) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = table->hash(key);
    size_t const index = hash % table->max;

    // for each node at index check if element is contained and return true or false
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        if (hash == table->hash(table->keys + (n * table->key_size))) {
            return true;
        }
    }

    return false;
}

void get_value_fhash_table(fhash_table_s const * const table, void const * const restrict key, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash values and index in array
    size_t const hash = table->hash(key);
    size_t const index = hash % table->max;

    // for each node at index check if element is contained
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        const char * current_key = table->keys + (n * table->key_size);
        if (hash == table->hash(current_key)) {
            memcpy(value_buffer, table->values + (n * table->value_size), table->value_size); // copy retrieved element into buffer
            return; // return to avoid assertion and termination at the end of function if key wasn't found
        }
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void set_value_fhash_table(fhash_table_s const * const table, void const * const restrict key, void const * const restrict value, void * const restrict value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");
    assert(key != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(key != value && "[ERROR] Parameters can't be the same.");
    assert(value != value_buffer && "[ERROR] Parameters can't be the same.");
    assert(table->length && "[ERROR] Structure is empty.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash values and index in array
    size_t const hash = table->hash(key);
    size_t const index = hash % table->max;

    // for each node at index check if element is contained
    for (size_t n = table->head[index]; NIL != n; n = table->next[n]) {
        const char * current_key = table->keys + (n * table->key_size);
        if (hash == table->hash(current_key)) {
            void * current_value = table->values + (n * table->value_size);
            memcpy(value_buffer, current_value, table->value_size); // copy retrieved element into buffer
            memcpy(current_value, value, table->value_size);
            return; // return to avoid assertion and termination at the end of function if key wasn't found
        }
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void map_key_fhash_table(fhash_table_s const * const table, handle_fn const handle, void * const arguments) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid key in lists
    for (size_t i = 0; i < table->max; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            if (!handle(table->keys + (n * table->key_size), arguments)) {
                return; // return since we need to break-off of two loops
            }
        }
    }
}

void map_value_fhash_table(fhash_table_s const * const table, handle_fn const handle, void * const arguments) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(table->allocator && "[INVALID] Paremeter can't be NULL.");

    // iterate over each valid value in lists
    for (size_t i = 0; i < table->max; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            if (!handle(table->values + (n * table->value_size), arguments)) {
                return; // return since we need to break-off of two loops
            }
        }
    }
}
