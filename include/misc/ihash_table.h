#ifndef IHASH_TABLE_H
#define IHASH_TABLE_H

#include <cerpec.h>

#if !defined(IHASH_TABLE_CHUNK)
#   define IHASH_TABLE_CHUNK CERPEC_CHUNK
#endif

/// @brief Stack data structure.
typedef struct infinite_hash_table {
    hash_fn hash;
    char * keys, * values;
    size_t * next, * head;
    size_t key_size, value_size, empty, length, capacity;
} ihash_table_s;

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param hash Function pointer to hash element into value.
/// @return Table structure.
ihash_table_s create_ihash_table(const size_t key_size, const size_t value_size, const hash_fn hash);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param table Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param destroy_value Function pointer to destroy a single value.
void destroy_ihash_table(ihash_table_s * table, const destroy_fn destroy_key, const destroy_fn destroy_value);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param table Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param destroy_value Function pointer to destroy a single value.
void clear_ihash_table(ihash_table_s * table, const destroy_fn destroy_key, const destroy_fn destroy_value);

/// @brief Creates a copy of a structure and all its elements.
/// @param table Structure to copy.
/// @param copy_key Function pointer to create a deep/shallow copy of a single key.
/// @param copy_value Function pointer to create a deep/shallow copy of a single value.
/// @return Stack structure.
ihash_table_s copy_ihash_table(const ihash_table_s * table, const copy_fn copy_key, const copy_fn copy_value);

/// @brief Checks if structure is empty.
/// @param table Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_ihash_table(const ihash_table_s * table);

/// @brief Inserts unique key/value pair into structure.
/// @param table Structure to insert element into.
/// @param key Key to insert.
/// @param value Value to insert.
void insert_ihash_table(ihash_table_s * table, const void * key, const void * value);

/// @brief Removes unique key/value pair from structure.
/// @param table Structure to remove element into.
/// @param key Key to remove.
/// @param key_buffer Key buffer to save removed key.
/// @param value_buffer Value buffer to save removed value.
void remove_ihash_table(ihash_table_s * table, const void * key, void * key_buffer, void * value_buffer);

/// @brief Checks if structure contains key.
/// @param table Structure to check.
/// @param key Key to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_key_ihash_table(const ihash_table_s * table, const void * key);

/// @brief Gets value based on key.
/// @param table Structure to get value.
/// @param key Key to value.
/// @param value_buffer Value buffer to save retrieved value.
void get_value_ihash_table(const ihash_table_s * table, const void * key, void * value_buffer);

/// @brief Iterates over each key in structure.
/// @param table Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_key_ihash_table(const ihash_table_s * table, const handle_fn handle, void * arguments);

/// @brief Iterates over each value in structure.
/// @param table Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_value_ihash_table(const ihash_table_s * table, const handle_fn handle, void * arguments);

#endif // IHASH_TABLE_H
