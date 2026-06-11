#ifndef ISC_HASH_MAP_H
#define ISC_HASH_MAP_H

#include <cerpec.h>

#if !defined(ISC_HASH_MAP_CHUNK)
#   define ISC_HASH_MAP_CHUNK CERPEC_CHUNK
#elif ISC_HASH_MAP_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (ISC_HASH_MAP_CHUNK & (ISC_HASH_MAP_CHUNK - 1))
#   error "Chunk size must be a power of 2."
#endif

#ifndef ISC_HASH_MAP_LOAD
#   define ISC_HASH_MAP_LOAD 0.75
#elif ISC_HASH_MAP_LOAD < 0.50
#   error "Load factor can't be less than 0.50."
#elif ISC_HASH_MAP_LOAD > 1.00
#   error "Load factor can't be greater than 1.00."
#endif

/// @brief Infinite hash map structure.
typedef struct infinite_separate_chaining_hash_map {
    hash_fn hash_key;
    compare_fn compare_key;
    char * keys, * values;
    size_t * next, * prev, * head, * hashes;
    size_t key_size, value_size, length, capacity;
    memory_s const * allocator;
} isc_hash_map_s;

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param hash_key Function pointer to hash element into value.
/// @param compare_key Function pointer to compare keys.
/// @return Map structure.
isc_hash_map_s create_isc_hash_map(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key);

/// @brief Creates an empty structure.
/// @param key_size Size of a single key.
/// @param value_size Size of a single value.
/// @param hash_key Function pointer to hash element into value.
/// @param allocator Custom allocator structure.
/// @param compare_key Function pointer to compare keys.
/// @return Map structure.
isc_hash_map_s make_isc_hash_map(size_t const key_size, size_t const value_size, hash_fn const hash_key, compare_fn const compare_key, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param map Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param adk Arguments for key destroy function pointer.
/// @param destroy_value Function pointer to destroy a single value.
/// @param adv Arguments for value destroy function pointer.
void destroy_isc_hash_map(isc_hash_map_s * const map, set_fn const destroy_key, void * const adk, set_fn const destroy_value, void * const adv);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param map Structure to destroy.
/// @param destroy_key Function pointer to destroy a single key.
/// @param adk Arguments for key destroy function pointer.
/// @param destroy_value Function pointer to destroy a single value.
/// @param adv Arguments for value destroy function pointer.
void clear_isc_hash_map(isc_hash_map_s * const map, set_fn const destroy_key, void * const adk, set_fn const destroy_value, void * const adv);

/// @brief Creates a copy of a structure and all its elements.
/// @param map Structure to copy.
/// @param copy_key Function pointer to create a deep/shallow copy of a single key.
/// @param ack Arguments for copy key function pointer.
/// @param copy_value Function pointer to create a deep/shallow copy of a single value.
/// @param acv Arguments for copy value function pointer.
/// @return Map structure.
isc_hash_map_s copy_isc_hash_map(isc_hash_map_s const * const map, copy_fn const copy_key, void * const ack, copy_fn const copy_value, void * const acv);

/// @brief Checks if structure is empty.
/// @param map Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_isc_hash_map(isc_hash_map_s const * const map);

/// @brief Inserts unique key/value pair into structure.
/// @param map Structure to insert element into.
/// @param key Key to insert.
/// @param value Value to insert.
void insert_isc_hash_map(isc_hash_map_s * const map, void const * const key, void const * const value);

/// @brief Removes unique key/value pair from structure.
/// @param map Structure to remove element into.
/// @param key Key to remove.
/// @param key_buffer Key buffer to save removed key.
/// @param value_buffer Value buffer to save removed value.
void remove_isc_hash_map(isc_hash_map_s * const map, void const * const key, void * const key_buffer, void * const value_buffer);

/// @brief Checks if structure contains key.
/// @param map Structure to check.
/// @param key Key to check.
/// @return 'true' if contained, 'false' otherwise.
bool contains_key_isc_hash_map(isc_hash_map_s const * const map, void const * const key);

/// @brief Gets value based on key.
/// @param map Structure to set value.
/// @param key Key to value.
/// @param value_buffer Value buffer to save retrieved value.
void get_value_isc_hash_map(isc_hash_map_s const * const map, void const * const key, void * const value_buffer);

/// @brief Sets value based on key.
/// @param map Structure to get value.
/// @param key Key to value.
/// @param value New value to insert.
/// @param value_buffer Old value buffer to save replaced value.
/// @note When key doesn't exist it performs an insert, otherwise the old value is replace with the new one.
void set_isc_hash_map(isc_hash_map_s * const map, void const * const key, void const * const value, void * const value_buffer);

/// @brief Iterates over each key in structure.
/// @param map Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void each_key_isc_hash_map(isc_hash_map_s const * const map, handle_fn const handle, void * const ah);

/// @brief Iterates over each value in structure.
/// @param map Structure to iterate over.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Generic arguments to use in function pointer.
void each_value_isc_hash_map(isc_hash_map_s const * const map, handle_fn const handle, void * const ah);

#endif // ISC_HASH_MAP_H
