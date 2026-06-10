#ifndef FSC_HASH_MAP_H
#define FSC_HASH_MAP_H

#include <cerpec.h>

/// @brief Finite hash map structure.
typedef struct finite_separate_chaining_hash_map {
    hash_fn hash_key;
    compare_fn compare_key;
    char * keys, * values;
    size_t * next, * prev, * head, * hashes;
    size_t key_size, value_size, length, max;
    memory_s const * allocator;
} fsc_hash_map_s;

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param max Maximum length of structure.
/// @param compare_key Function pointer to compare keys.
/// @param hash_key Function pointer to hash element into value.
/// @return Map structure.
fsc_hash_map_s create_fsc_hash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key);

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param max Maximum length of structure.
/// @param hash_key Function pointer to hash element into value.
/// @param compare_key Function pointer to compare keys.
/// @param allocator Custom allocator structure.
/// @return Map structure.
fsc_hash_map_s make_fsc_hash_map(size_t const key_size, size_t const value_size, size_t const max, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param map Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param argdk Arguments for key destroy function pointer.
/// @param destroy_value Function pointer to destroy a single value.
/// @param argdv Arguments for value destroy function pointer.
void destroy_fsc_hash_map(fsc_hash_map_s * const map, set_fn const destroy_key, void * const argdk, set_fn const destroy_value, void * const argdv);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param map Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param argdk Arguments for key destroy function pointer.
/// @param destroy_value Function pointer to destroy a single value.
/// @param argdv Arguments for value destroy function pointer.
void clear_fsc_hash_map(fsc_hash_map_s * const map, set_fn const destroy_key, void * const argdk, set_fn const destroy_value, void * const argdv);

/// @brief Creates a copy of a structure and all its elements.
/// @param map Structure to copy.
/// @param copy_key Function pointer to create a deep/shallow copy of a single key.
/// @param copy_value Function pointer to create a deep/shallow copy of a single value.
/// @return Map structure.
fsc_hash_map_s copy_fsc_hash_map(fsc_hash_map_s const * const map, copy_fn const copy_key, copy_fn const copy_value);

/// @brief Checks if structure is empty.
/// @param map Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_fsc_hash_map(fsc_hash_map_s const * const map);

/// @brief Checks if structure is full.
/// @param map Structure to check.
/// @return 'true' if full, 'false' if not.
bool is_full_fsc_hash_map(fsc_hash_map_s const * const map);

/// @brief Inserts unique key/value pair into structure.
/// @param map Structure to insert element into.
/// @param key Key to insert.
/// @param value Value to insert.
void insert_fsc_hash_map(fsc_hash_map_s * const map, void const * const key, void const * const value);

/// @brief Removes unique key/value pair from structure.
/// @param map Structure to remove element into.
/// @param key Key to remove.
/// @param key_buffer Key buffer to save removed key.
/// @param value_buffer Value buffer to save removed value.
void remove_fsc_hash_map(fsc_hash_map_s * const map, void const * const key, void * const key_buffer, void * const value_buffer);

/// @brief Checks if structure contains key.
/// @param map Structure to check.
/// @param key Key to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_key_fsc_hash_map(fsc_hash_map_s const * const map, void const * const key);

/// @brief Gets value based on key.
/// @param map Structure to get value.
/// @param key Key to value.
/// @param value_buffer Value buffer to save retrieved value.
void get_value_fsc_hash_map(fsc_hash_map_s const * const map, void const * const key, void * const value_buffer);

/// @brief Sets value based on key.
/// @param map Structure to set value.
/// @param key Key to value.
/// @param value New value to insert.
/// @param value_buffer Old value buffer to save replaced value.
void set_fsc_hash_map(fsc_hash_map_s * const map, void const * const key, void const * const value, void * const value_buffer);

/// @brief Iterates over each key in structure.
/// @param map Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param argh Generic arguments to use in function pointer.
void each_key_fsc_hash_map(fsc_hash_map_s const * const map, handle_fn const handle, void * const argh);

/// @brief Iterates over each value in structure.
/// @param map Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param argh Generic arguments to use in function pointer.
void each_value_fsc_hash_map(fsc_hash_map_s const * const map, handle_fn const handle, void * const argh);

#endif // FSC_HASH_MAP_H
