#ifndef IHASH_TABLE_H
#define IHASH_TABLE_H

#include <cerpec.h>

#if !defined(IHASH_TABLE_CHUNK)
#   define IHASH_TABLE_CHUNK CERPEC_CHUNK
#elif IHASH_TABLE_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#endif

/// @brief Infinite hash table data structure.
typedef struct infinite_hash_table {
    hash_fn hash_key;
    compare_fn compare_key;
    char * keys, * values;
    size_t * next, * head, * hashes;
    size_t key_size, value_size, empty, length, capacity;
    memory_s const * allocator;
} ihash_table_s;

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param hash_key Function pointer to hash element into value.
/// @param compare_key Function pointer to compare keys.
/// @return Table structure.
ihash_table_s create_ihash_table(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key);

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param hash_key Function pointer to hash element into value.
/// @param allocator Custom allocator structure.
/// @param compare_key Function pointer to compare keys.
/// @return Table structure.
ihash_table_s make_ihash_table(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param table Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param destroy_value Function pointer to destroy a single value.
void destroy_ihash_table(ihash_table_s * const table, set_fn const destroy_key, set_fn const destroy_value);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param table Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param destroy_value Function pointer to destroy a single value.
void clear_ihash_table(ihash_table_s * table, const set_fn destroy_key, const set_fn destroy_value);

/// @brief Creates a copy of a structure and all its elements.
/// @param table Structure to copy.
/// @param copy_key Function pointer to create a deep/shallow copy of a single key.
/// @param copy_value Function pointer to create a deep/shallow copy of a single value.
/// @return Stack structure.
ihash_table_s copy_ihash_table(ihash_table_s const * const table, copy_fn const copy_key, copy_fn const copy_value);

/// @brief Checks if structure is empty.
/// @param table Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_ihash_table(ihash_table_s const * const table);

/// @brief Inserts unique key/value pair into structure.
/// @param table Structure to insert element into.
/// @param key Key to insert.
/// @param value Value to insert.
void insert_ihash_table(ihash_table_s * const table, void const * const key, void const * const value);

/// @brief Removes unique key/value pair from structure.
/// @param table Structure to remove element into.
/// @param key Key to remove.
/// @param key_buffer Key buffer to save removed key.
/// @param value_buffer Value buffer to save removed value.
void remove_ihash_table(ihash_table_s * const table, void const * const key, void * const key_buffer, void * const value_buffer);

/// @brief Checks if structure contains key.
/// @param table Structure to check.
/// @param key Key to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_key_ihash_table(ihash_table_s const * const table, void const * const key);

/// @brief Gets value based on key.
/// @param table Structure to set value.
/// @param key Key to value.
/// @param value_buffer Value buffer to save retrieved value.
void get_value_ihash_table(ihash_table_s const * const table, void const * const key, void * const value_buffer);

/// @brief Sets value based on key.
/// @param table Structure to get value.
/// @param key Key to value.
/// @param value New value to insert.
/// @param value_buffer Old value buffer to save replaced value.
void set_value_ihash_table(ihash_table_s const * const table, void const * const key, void const * const value, void * const value_buffer);

/// @brief Iterates over each key in structure.
/// @param table Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_key_ihash_table(ihash_table_s const * const table, handle_fn const handle, void * const arguments);

/// @brief Iterates over each value in structure.
/// @param table Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Generic arguments to use in function pointer.
void map_value_ihash_table(ihash_table_s const * const table, handle_fn const handle, void * const arguments);

#endif // IHASH_TABLE_H
