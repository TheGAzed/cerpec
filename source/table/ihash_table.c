#include <table/ihash_table.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL (size_t)(-1)

/// Resizes (reallocates) table parameter arrays based on changed capacity.
/// @param table Structure to resize.
/// @param size New size.
void _ihash_table_resize(ihash_table_s * table, const size_t size);

ihash_table_s create_ihash_table(const size_t key_size, const size_t value_size, const hash_fn hash)  {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(key_size && "[ERROR] Parameter can't be zero.");
    assert(value_size && "[ERROR] Parameter can't be zero.");

    return (ihash_table_s) { .key_size = key_size, .value_size = value_size, .hash = hash, .empty = NIL, }; // table empty list to NIL
}

void destroy_ihash_table(ihash_table_s * table, const destroy_fn destroy_key, const destroy_fn destroy_value)  {
    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    // free arrays
    free(table->head);
    free(table->keys);
    free(table->values);
    free(table->next);

    memset(table, 0, sizeof(ihash_table_s)); // table everything to zero/false
}

void clear_ihash_table(ihash_table_s * table, const destroy_fn destroy_key, const destroy_fn destroy_value)  {
    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < table->capacity; ++i) {
        for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
            destroy_key(table->keys + (n * table->key_size));
            destroy_value(table->values + (n * table->value_size));
        }
    }

    // free arrays
    free(table->head);
    free(table->keys);
    free(table->values);
    free(table->next);

    // only clear table (keep the table usable)
    table->empty = NIL;
    table->capacity = table->length = 0;
    table->head = NULL;
    table->next = NULL;
    table->keys = table->values = NULL;
}

ihash_table_s copy_ihash_table(const ihash_table_s table, const copy_fn copy_key, const copy_fn copy_value) {
    assert(copy_key && "[ERROR] Parameter can't be NULL.");
    assert(copy_value && "[ERROR] Parameter can't be NULL.");

    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    const ihash_table_s replica = {
        .capacity = table.capacity, .empty = NIL, .hash = table.hash, .length = table.length,
        .key_size = table.key_size, .value_size = table.value_size,

        .keys = malloc(table.capacity * table.key_size),
        .values = malloc(table.capacity * table.value_size),

        .head = malloc(table.capacity * sizeof(size_t)),
        .next = malloc(table.capacity * sizeof(size_t)),
    };
    assert((!replica.capacity || replica.keys) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.values) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.head) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // for each index, if each index is valid node then push it into replica
    for (size_t i = 0, hole = 0; i < table.capacity; ++i) {
        replica.head[i] = NIL; // initially table replica heads to invalid

        // if table has elements in head then push them into replica heads (like a stack)
        for (size_t n = table.head[i]; NIL != n; n = table.next[n], hole++) {
            copy_key(replica.keys + (hole * replica.key_size), table.keys + (n * table.key_size));
            copy_value(replica.values + (hole * replica.value_size), table.values + (n * table.value_size));

            // node index redirection
            replica.next[hole] = replica.head[i];
            replica.head[i] = hole;
        }
    }

    return replica;
}

bool is_empty_ihash_table(const ihash_table_s table) {
    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !(table.length); // if 0 return 'true'
}

void insert_ihash_table(ihash_table_s * table, const void * key, const void * value) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // resize (expand) if table can't contain new element
    if (table->length == table->capacity) {
        _ihash_table_resize(table, table->capacity + IHASH_TABLE_CHUNK);
    }

    // calculate hash values and index in array
    const size_t hash = table->hash(key);
    const size_t index = hash % table->capacity;

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

void remove_ihash_table(ihash_table_s * table, const void * key, void * key_buffer, void * value_buffer) {
    assert(table && "[ERROR] Parameter can't be NULL.");
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(key_buffer && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");

    assert(table->hash && "[INVALID] Parameter can't be NULL.");
    assert(table->key_size && "[INVALID] Parameter can't be zero.");
    assert(table->value_size && "[INVALID] Parameter can't be zero.");
    assert(table->length <= table->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash values and index in array
    const size_t hash = table->hash(key);
    const size_t index = hash % table->capacity;

    // for each node at index check if element is contained
    for (size_t * n = table->head + index; NIL != (*n); n = table->next + (*n)) {
        const char * current = table->keys + ((*n) + table->key_size);
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

        // resize (expand) if table can contain a smaller capacity of elements
        if (table->capacity - IHASH_TABLE_CHUNK == table->length) {
            _ihash_table_resize(table, table->capacity - IHASH_TABLE_CHUNK);
        }

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_key_ihash_table(const ihash_table_s table, const void * key) {
    assert(key && "[ERROR] Parameter can't be NULL.");

    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash values and index in array
    const size_t hash = table.hash(key);
    const size_t index = hash % table.capacity;

    // for each node at index check if element is contained and return true or false
    for (size_t n = table.head[index]; NIL != n; n = table.next[n]) {
        if (hash == table.hash(table.keys + (n * table.key_size))) {
            return true;
        }
    }

    return false;
}

void get_value_ihash_table(const ihash_table_s table, const void * key, void * value_buffer) {
    assert(key && "[ERROR] Parameter can't be NULL.");
    assert(value_buffer && "[ERROR] Parameter can't be NULL.");

    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash values and index in array
    const size_t hash = table.hash(key);
    const size_t index = hash % table.capacity;

    // for each node at index check if element is contained
    for (size_t n = table.head[index]; NIL != n; n = table.next[n]) {
        const char * current = table.keys + (n + table.key_size);
        if (hash == table.hash(current)) {
            memcpy(value_buffer, current, table.value_size); // copy retrieved element into buffer
            return; // return to avoid assertion and termination at the end of function if key wasn't found
        }
    }

    assert(false && "[ERROR] Structure does not contain key.");
    exit(EXIT_FAILURE); // terminate on error
}

void foreach_key_ihash_table(const ihash_table_s table, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < table.capacity; ++i) {
        for (size_t n = table.head[i]; NIL != n; n = table.next[n]) {
            if (!operate(table.keys + (n * table.key_size), arguments)) {
                return;
            }
        }
    }
}

void foreach_value_ihash_table(const ihash_table_s table, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(table.hash && "[INVALID] Parameter can't be NULL.");
    assert(table.key_size && "[INVALID] Parameter can't be zero.");
    assert(table.value_size && "[INVALID] Parameter can't be zero.");
    assert(table.length <= table.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < table.capacity; ++i) {
        for (size_t n = table.head[i]; NIL != n; n = table.next[n]) {
            if (!operate(table.values + (n * table.value_size), arguments)) {
                return;
            }
        }
    }
}

void _ihash_table_resize(ihash_table_s * table, const size_t size) {
    char * keys = NULL;
    char * values = NULL;

    if (size != table->length) {
        keys = realloc(table->keys, table->capacity * table->key_size);
        values = realloc(table->values, table->capacity * table->value_size);

        assert(keys && "[ERROR] Memory allocation failed.");
        assert(values && "[ERROR] Memory allocation failed.");
    } else {
        keys = malloc(size * table->key_size);
        values = malloc(size * table->value_size);

        for (size_t i = 0; i < table->capacity; ++i) {
            for (size_t n = table->head[i]; NIL != n; n = table->next[n]) {
                memcpy(keys, table->keys + (n * table->key_size), table->key_size);
                memcpy(values, table->values + (n * table->value_size), table->value_size);
            }
        }
        assert((!table->capacity || table->keys) && "[ERROR] Memory allocation failed.");

        free(table->keys);
    }

    table->capacity = size;
    table->head     = realloc(table->head, table->capacity * sizeof(size_t));
    table->next     = realloc(table->next, table->capacity * sizeof(size_t));
    table->keys     = keys;
    table->keys     = values;

    assert((!table->capacity || table->head) && "[ERROR] Memory allocation failed.");
    assert((!table->capacity || table->next) && "[ERROR] Memory allocation failed.");

    table->empty = NIL;
    for (size_t i = 0; i < table->capacity; ++i) {
        table->head[i] = NIL;
    }

    for (size_t i = 0, hole = 0; i < table->length; ++i, ++hole) {
        const size_t hash = table->hash(keys + (i * table->key_size));
        const size_t mod = hash % table->capacity;

        table->next[hole] = table->head[mod];
        table->head[mod] = hole;
    }
}
