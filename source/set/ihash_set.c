#include <set/ihash_set.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

/// @brief Fills hole produced by element removal in arrays of elements.
/// @param set Structure to remove hole from.
/// @param hole Index of hole in arrays.
void _ihash_set_fill_hole(ihash_set_s const * const set, size_t const hole);

/// @brief Resizes (reallocates) set parameter arrays based on changed capacity.
/// @param set Structure to resize.
/// @param size New size.
void _ihash_set_resize(ihash_set_s * const set, size_t const size);

/// @brief Make logic wrapper mainly to repeated assertion for specific structure operations.
/// @param size Size of single element.
/// @param hash Hash function to generate hash values from elements.
/// @param compare Compare function to compare elements.
/// @param allocator Custom allocator function.
/// @return Set structure.
ihash_set_s _make_wrapper_ihash_set(size_t const size, hash_fn const hash, compare_fn const compare, memory_s const * const allocator);

/// @brief Copy logic wrapper mainly to repeated assertion for specific structure operations.
/// @param set Structure to copy.
/// @param copy Function pointer to create shallow/deep copy of single element
/// @return Set structure.
ihash_set_s _copy_wrapper_ihash_set(ihash_set_s const * const set, copy_fn const copy);

/// @brief Insert logic wrapper mainly to remove repeated code snippeds.
/// @param set Structure to call insert logic on.
/// @param hash Hash of element to be inserted.
/// @param index Head list index.
/// @note Elements aren't added as shallow (memcpy) or deep (custom copy function) copying may occur.
void _insert_wrapper_ihash_set(ihash_set_s const * const set, size_t const hash, size_t const index);

/// @brief Contains logic wrapper mainly to remove repeated code snippeds.
/// @param set Structure to call contains logic on.
/// @param element Element to check if it's contained.
/// @param hash Hash of element to be contained.
/// @param index Head list index.
/// @return 'true' if element is contained, 'false' otherwise.
bool _contains_wrapper_ihash_set(ihash_set_s const * const set, void const * const element, size_t const hash, size_t const index);

ihash_set_s create_ihash_set(size_t const size, hash_fn const hash, compare_fn const compare) {
    error(hash && "Parameter can't be NULL.");
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");

    return (ihash_set_s) { .size = size, .hash = hash, .compare = compare, .allocator = &standard, };
}

ihash_set_s make_ihash_set(size_t const size, hash_fn const hash, compare_fn const compare, memory_s const * const allocator) {
    error(hash && "Parameter can't be NULL.");
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");
    error(allocator && "Parameter can't be NULL.");

    return _make_wrapper_ihash_set(size, hash, compare, allocator);
}

void destroy_ihash_set(ihash_set_s * const set, set_fn const destroy) {
    error(set && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->length; ++i) {
        destroy(set->elements + (i * set->size));
    }

    // free arrays
    set->allocator->free(set->elements, set->allocator->arguments);
    set->allocator->free(set->hashes, set->allocator->arguments);

    set->allocator->free(set->head, set->allocator->arguments);
    set->allocator->free(set->next, set->allocator->arguments);
    set->allocator->free(set->prev, set->allocator->arguments);

    // set everything to zero/false
    memset(set, 0, sizeof(ihash_set_s));
}

void clear_ihash_set(ihash_set_s * const set, set_fn const destroy) {
    error(set && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->length; ++i) {
        destroy(set->elements + (i * set->size));
    }

    // free arrays
    set->allocator->free(set->elements, set->allocator->arguments);
    set->allocator->free(set->hashes, set->allocator->arguments);

    set->allocator->free(set->head, set->allocator->arguments);
    set->allocator->free(set->next, set->allocator->arguments);
    set->allocator->free(set->prev, set->allocator->arguments);

    // only clear set (keep the set usable)
    set->capacity = set->length = 0;

    set->elements = NULL;
    set->hashes = NULL;

    set->head = NULL;
    set->next = NULL;
    set->prev = NULL;
}

ihash_set_s copy_ihash_set(ihash_set_s const * const set, copy_fn const copy) {
    error(set && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    return _copy_wrapper_ihash_set(set, copy);
}

bool is_empty_ihash_set(ihash_set_s const * const set) {
    error(set && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    return !(set->length); // if 0 return 'true'
}

void insert_ihash_set(ihash_set_s * const set, void const * const element) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    // resize (expand) if set can't contain new element
    if (set->length == set->capacity) {
        _ihash_set_resize(set, set->capacity + IHASH_SET_CHUNK);
    }

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->capacity;

    // check if element is in set or not
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        void const * const current = set->elements + (n * set->size);
        error((hash != set->hashes[n] || set->compare(element, current)) && "Element already in map.");
    }

    _insert_wrapper_ihash_set(set, hash, index);
    memcpy(set->elements + (set->length * set->size), element, set->size);
    set->length++;
}

void remove_ihash_set(ihash_set_s * const set, void const * const element, void * const buffer) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->capacity;

    // for each node at index check if element is contained
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        const char * current = set->elements + (n * set->size);
        if (hash != set->hashes[n] || set->compare(element, current)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(buffer, current, set->size);
        set->length--;

        _ihash_set_fill_hole(set, n);

        // resize (expand) if set can contain a smaller capacity of elements
        if (set->capacity - IHASH_SET_CHUNK == set->length) {
            _ihash_set_resize(set, set->length);
        }

        return; // return to avoid errorion and termination at the end of function if element wasn't found
    }

    error(false && "Structure does not contain element.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_ihash_set(ihash_set_s const * const set, void const * const element) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    // early return to avoid 'x mod 0' by capacity
    if (!set->capacity) { return false; }

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->capacity;

    return _contains_wrapper_ihash_set(set, element, hash, index);
}

ihash_set_s union_ihash_set(ihash_set_s const * const set_one, ihash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->size && "Size can't be zero.");
    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->compare && "Compare function can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");
    valid(set_one->length <= set_one->capacity && "Lenght can't be larger than capacity.");

    valid(set_two->size && "Size can't be zero.");
    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->compare && "Compare function can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");
    valid(set_two->length <= set_two->capacity && "Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements to maximum's replica
    ihash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    ihash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    // copy maximum set into set union
    ihash_set_s set_union = _copy_wrapper_ihash_set(maximum, copy);
    for (size_t m = 0; m < minimum->length; ++m) {
        char const * const element = minimum->elements + (m * minimum->size);

        size_t const min_hash = minimum->hashes[m];
        size_t const union_idx = min_hash % set_union.capacity;

        // if element is not in the set push it to set union
        bool const contains = _contains_wrapper_ihash_set(&set_union, element, min_hash, union_idx);
        if (!contains) {
            // expand union set if necessary
            if (set_union.length == set_union.capacity) {
                _ihash_set_resize(&set_union, set_union.capacity + IHASH_SET_CHUNK);
            }

            // index may change due to increase in capacity
            size_t const new_union_idx = min_hash % set_union.capacity;
            _insert_wrapper_ihash_set(&set_union, min_hash, new_union_idx);

            copy(set_union.elements + (set_union.length * set_union.size), element);
            set_union.length++;
        }
    }

    return set_union;
}

ihash_set_s intersect_ihash_set(ihash_set_s const * const set_one, ihash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->size && "Size can't be zero.");
    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->compare && "Compare function can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");
    valid(set_one->length <= set_one->capacity && "Lenght can't be larger than capacity.");

    valid(set_two->size && "Size can't be zero.");
    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->compare && "Compare function can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");
    valid(set_two->length <= set_two->capacity && "Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements
    ihash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    ihash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    ihash_set_s set_intersect = _make_wrapper_ihash_set(set_one->size, set_one->hash, set_one->compare, set_one->allocator);
    for (size_t min = 0; min < minimum->length; ++min) {
        char const * const element = minimum->elements + (min * minimum->size);

        size_t const min_hash = minimum->hashes[min];
        size_t const max_idx = min_hash % maximum->capacity;

        // if element is in set push it into intersect
        bool const contains = _contains_wrapper_ihash_set(maximum, element, min_hash, max_idx);
        if (contains) {
            // expand intersect set if necessary
            if (set_intersect.length == set_intersect.capacity) {
                _ihash_set_resize(&set_intersect, set_intersect.capacity + IHASH_SET_CHUNK);
            }

            size_t const intersect_index = min_hash % set_intersect.capacity;
            _insert_wrapper_ihash_set(&set_intersect, min_hash, intersect_index);

            copy(set_intersect.elements + (set_intersect.length * set_intersect.size), element);
            set_intersect.length++;
        }
    }

    return set_intersect;
}

ihash_set_s subtract_ihash_set(ihash_set_s const * const minuend, ihash_set_s const * const subtrahend, copy_fn const copy) {
    error(minuend && "Parameter can't be NULL.");
    error(subtrahend && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(minuend->hash == subtrahend->hash && "Function pointers must be the same.");
    error(minuend->size == subtrahend->size && "Sizes must be the same.");

    valid(minuend->size && "Size can't be zero.");
    valid(minuend->hash && "Hash function can't be NULL.");
    valid(minuend->compare && "Compare function can't be NULL.");
    valid(minuend->allocator && "Allocator can't be NULL.");
    valid(minuend->length <= minuend->capacity && "Lenght can't be larger than capacity.");

    valid(subtrahend->size && "Size can't be zero.");
    valid(subtrahend->hash && "Hash function can't be NULL.");
    valid(subtrahend->compare && "Compare function can't be NULL.");
    valid(subtrahend->allocator && "Allocator can't be NULL.");
    valid(subtrahend->length <= subtrahend->capacity && "Lenght can't be larger than capacity.");

    ihash_set_s set_subtract = _make_wrapper_ihash_set(minuend->size, minuend->hash, minuend->compare, minuend->allocator);
    for (size_t min = 0; min < minuend->length; ++min) {
        // get element and set its found flag to false
        char const * const element = minuend->elements + (min * minuend->size);

        size_t const hash = minuend->hashes[min];
        size_t const subtrahend_idx = hash % subtrahend->capacity;

        // if minuend element is not in subtrahend set push it to new set
        bool const contains = _contains_wrapper_ihash_set(subtrahend, element, hash, subtrahend_idx);
        if (!contains) {
            // expand subtract set if necessary
            if (set_subtract.length == set_subtract.capacity) {
                _ihash_set_resize(&set_subtract, set_subtract.capacity + IHASH_SET_CHUNK);
            }

            size_t const subtract_index = hash % set_subtract.capacity;
            _insert_wrapper_ihash_set(&set_subtract, hash, subtract_index);

            copy(set_subtract.elements + (set_subtract.length * set_subtract.size), element);
            set_subtract.length++;
        }
    }

    return set_subtract;
}

ihash_set_s exclude_ihash_set(ihash_set_s const * const set_one, ihash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->size && "Size can't be zero.");
    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->compare && "Compare function can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");
    valid(set_one->length <= set_one->capacity && "Lenght can't be larger than capacity.");

    valid(set_two->size && "Size can't be zero.");
    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->compare && "Compare function can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");
    valid(set_two->length <= set_two->capacity && "Lenght can't be larger than capacity.");

    ihash_set_s set_exclude = _make_wrapper_ihash_set(set_one->size, set_one->hash, set_one->compare, set_one->allocator);

    for (size_t one = 0; one < set_one->length; ++one) {
        // get element and set its found flag to false
        char const * const element = set_one->elements + (one * set_one->size);

        size_t const hash = set_one->hashes[one];
        size_t const two_idx = hash % set_two->capacity;

        bool const contains = _contains_wrapper_ihash_set(set_two, element, hash, two_idx);
        if (!contains) {
            // expand exclude set if necessary
            if (set_exclude.length == set_exclude.capacity) {
                _ihash_set_resize(&set_exclude, set_exclude.capacity + IHASH_SET_CHUNK);
            }

            size_t const exclude_index = hash % set_exclude.capacity;
            _insert_wrapper_ihash_set(&set_exclude, hash, exclude_index);

            copy(set_exclude.elements + (set_exclude.length * set_exclude.size), element);
            set_exclude.length++;
        }
    }

    for (size_t two = 0; two < set_two->length; ++two) {
        // get element and set its found flag to false
        char const * const element = set_two->elements + (two * set_two->size);

        size_t const hash = set_two->hashes[two];
        size_t const one_idx = hash % set_one->capacity;

        bool const contains = _contains_wrapper_ihash_set(set_one, element, hash, one_idx);
        if (!contains) {
            // expand exclude set if necessary
            if (set_exclude.length == set_exclude.capacity) {
                _ihash_set_resize(&set_exclude, set_exclude.capacity + IHASH_SET_CHUNK);
            }

            size_t const exclude_index = hash % set_exclude.capacity;
            _insert_wrapper_ihash_set(&set_exclude, hash, exclude_index);

            copy(set_exclude.elements + (set_exclude.length * set_exclude.size), element);
            set_exclude.length++;
        }
    }

    return set_exclude;
}

bool is_subset_ihash_set(ihash_set_s const * const superset, ihash_set_s const * const subset) {
    error(superset && "Parameter can't be NULL.");
    error(subset && "Parameter can't be NULL.");
    error(superset->hash == subset->hash && "Function pointers must be the same.");
    error(superset->size == subset->size && "Sizes must be the same.");

    valid(superset->size && "Size can't be zero.");
    valid(superset->hash && "Hash function can't be NULL.");
    valid(superset->compare && "Compare function can't be NULL.");
    valid(superset->allocator && "Allocator can't be NULL.");
    valid(superset->length <= superset->capacity && "Lenght can't be larger than capacity.");

    valid(subset->size && "Size can't be zero.");
    valid(subset->hash && "Hash function can't be NULL.");
    valid(subset->compare && "Compare function can't be NULL.");
    valid(subset->allocator && "Allocator can't be NULL.");
    valid(subset->length <= subset->capacity && "Lenght can't be larger than capacity.");

    for (size_t sub = 0; sub < subset->length; ++sub) {
        // get element and set its found flag to false
        char const * const element = subset->elements + (sub * subset->size);

        size_t const sub_hash = subset->hashes[sub];
        size_t const super_idx = sub_hash % superset->capacity;

        bool const contains = _contains_wrapper_ihash_set(superset, element, sub_hash, super_idx);
        if (!contains) { return false; }
    }

    return true;
}

bool is_proper_subset_ihash_set(ihash_set_s const * const superset, ihash_set_s const * const subset) {
    error(superset && "Parameter can't be NULL.");
    error(subset && "Parameter can't be NULL.");
    error(superset->hash == subset->hash && "Function pointers must be the same.");
    error(superset->size == subset->size && "Sizes must be the same.");

    valid(superset->size && "Size can't be zero.");
    valid(superset->hash && "Hash function can't be NULL.");
    valid(superset->compare && "Compare function can't be NULL.");
    valid(superset->allocator && "Allocator can't be NULL.");
    valid(superset->length <= superset->capacity && "Lenght can't be larger than capacity.");

    valid(subset->size && "Size can't be zero.");
    valid(subset->hash && "Hash function can't be NULL.");
    valid(subset->compare && "Compare function can't be NULL.");
    valid(subset->allocator && "Allocator can't be NULL.");
    valid(subset->length <= subset->capacity && "Lenght can't be larger than capacity.");

    for (size_t sub = 0; sub < subset->length; ++sub) {
        // get element and set its found flag to false
        char const * const element = subset->elements + (sub * subset->size);

        size_t const sub_hash = subset->hashes[sub];
        size_t const super_idx = sub_hash % superset->capacity;

        bool const contains = _contains_wrapper_ihash_set(superset, element, sub_hash, super_idx);
        if (!contains) { return false; }
    }

    return (subset->length != superset->length);
}

bool is_disjoint_ihash_set(ihash_set_s const * const set_one, ihash_set_s const * const set_two) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->size && "Size can't be zero.");
    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->compare && "Compare function can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");
    valid(set_one->length <= set_one->capacity && "Lenght can't be larger than capacity.");

    valid(set_two->size && "Size can't be zero.");
    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->compare && "Compare function can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");
    valid(set_two->length <= set_two->capacity && "Lenght can't be larger than capacity.");

    ihash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    ihash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    for (size_t min = 0; min < minimum->length; ++min) {
        // get element and set its found flag to false
        char const * const element = minimum->elements + (min * minimum->size);

        size_t const min_hash = minimum->hash(element);
        size_t const max_idx = min_hash % maximum->capacity;

        bool const contains = _contains_wrapper_ihash_set(maximum, element, min_hash, max_idx);
        if (contains) { return false; }
    }

    return true;
}

void each_ihash_set(ihash_set_s const * const set, handle_fn const handle, void * const arguments) {
    error(set && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(set->size && "Size can't be zero.");
    valid(set->hash && "Hash function can't be NULL.");
    valid(set->compare && "Compare function can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");
    valid(set->length <= set->capacity && "Lenght can't be larger than capacity.");

    for (size_t i = 0; i < set->length; ++i) {
        if (!handle(set->elements + (i * set->size), arguments)) { break; }
    }
}

void _ihash_set_resize(ihash_set_s * const set, size_t const size) {
    // set table to new resized parameters
    set->capacity = size;

    set->head = set->allocator->realloc(set->head, size * sizeof(size_t), set->allocator->arguments);
    error((!set->capacity || set->head) && "Memory allocation failed.");

    set->next = set->allocator->realloc(set->next, size * sizeof(size_t), set->allocator->arguments);
    error((!set->capacity || set->next) && "Memory allocation failed.");

    set->prev = set->allocator->realloc(set->prev, size * sizeof(size_t), set->allocator->arguments);
    error((!set->capacity || set->prev) && "Memory allocation failed.");

    set->elements = set->allocator->realloc(set->elements, size * set->size, set->allocator->arguments);
    error((!set->capacity || set->elements) && "Memory allocation failed.");

    set->hashes = set->allocator->realloc(set->hashes, size * sizeof(size_t), set->allocator->arguments);
    error((!set->capacity || set->hashes) && "Memory allocation failed.");

    for (size_t i = 0; i < set->capacity; ++i) {
        set->head[i] = NIL;
    }

    // reset lists by pushing hashes to their valid list
    for (size_t i = 0; i < set->length; ++i) {
        const size_t index = set->hashes[i] % set->capacity;

        size_t const head = set->head[index];
        if (NIL != head) {
            set->prev[head] = i;
        }

        // node index redirection
        set->prev[i] = NIL;
        set->next[i] = head;
        set->head[index] = i;
    }
}

ihash_set_s _make_wrapper_ihash_set(size_t const size, hash_fn const hash, compare_fn const compare, memory_s const * const allocator) {
    return (ihash_set_s) { .size = size, .hash = hash, .compare = compare, .allocator = allocator, };
}

ihash_set_s _copy_wrapper_ihash_set(ihash_set_s const * const set, copy_fn const copy) {
    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    ihash_set_s const replica = {
        .capacity = set->capacity, .hash = set->hash, .length = set->length, .size = set->size,
        .allocator = set->allocator, .compare = set->compare,

        .elements = set->allocator->alloc(set->capacity * set->size, set->allocator->arguments),
        .hashes = set->allocator->alloc(set->capacity * sizeof(size_t), set->allocator->arguments),

        .head = set->allocator->alloc(set->capacity * sizeof(size_t), set->allocator->arguments),
        .next = set->allocator->alloc(set->capacity * sizeof(size_t), set->allocator->arguments),
        .prev = set->allocator->alloc(set->capacity * sizeof(size_t), set->allocator->arguments),
    };
    error((!replica.capacity || replica.elements) && "Memory allocation failed.");
    error((!replica.capacity || replica.hashes) && "Memory allocation failed.");

    error((!replica.capacity || replica.head) && "Memory allocation failed.");
    error((!replica.capacity || replica.next) && "Memory allocation failed.");
    error((!replica.capacity || replica.prev) && "Memory allocation failed.");

    memcpy(replica.head, set->head, set->capacity * sizeof(size_t)); // special case - heads index based on maximum length

    memcpy(replica.next, set->next, set->length * sizeof(size_t));
    memcpy(replica.prev, set->prev, set->length * sizeof(size_t));
    memcpy(replica.hashes, set->hashes, set->length * sizeof(size_t));

    // for each element continuusly in array call copy function
    for (size_t i = 0; i < set->length; ++i) {
        copy(replica.elements + (i * replica.size), set->elements + (i * set->size));
    }

    return replica;
}

void _insert_wrapper_ihash_set(ihash_set_s const * const set, size_t const hash, size_t const index) {
    size_t const current = set->length;

    // if head has an element then redirect its prev to current
    size_t const head = set->head[index];
    if (NIL != head) {
        set->prev[head] = current;
    }

    // node index redirection
    set->prev[current] = NIL;
    set->next[current] = head;
    set->head[index] = current;

    // copy element into elements array
    set->hashes[current] = hash;
}

bool _contains_wrapper_ihash_set(ihash_set_s const * const set, void const * const element, size_t const hash, size_t const index) {
    // for each node at index check if element is contained and return true or false
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        void const * const current = set->elements + (n * set->size);
        if (hash == set->hashes[n] && !set->compare(element, current)) {
            return true;
        }
    }

    return false;
}

void _ihash_set_fill_hole(ihash_set_s const * const set, size_t const hole) {
    if (NIL == set->prev[set->length]) {
        size_t const index = set->hashes[set->length] % set->capacity;
        set->head[index] = hole;
    }

    if (NIL == set->prev[hole]) {
        size_t const index = set->hashes[hole] % set->capacity;
        set->head[index] = set->next[hole];
    }

    // cut current removed node's siblings from itself
    if (NIL != set->prev[hole]) { set->next[set->prev[hole]] = set->next[hole]; }
    if (NIL != set->next[hole]) { set->prev[set->next[hole]] = set->prev[hole]; }

    // cut current removed node from its siblings
    set->next[hole] = set->prev[hole] = hole;

    // replace element at current index with popped last element like in a stack
    memmove(set->elements + (hole * set->size), set->elements + (set->length * set->size), set->size);
    set->hashes[hole] = set->hashes[set->length];
    set->next[hole] = set->next[set->length];
    set->prev[hole] = set->prev[set->length];

    // redirect array's last swapped node's siblings to hole
    if (NIL != set->next[set->length]) { set->prev[set->next[set->length]] = hole; }
    if (NIL != set->prev[set->length]) { set->next[set->prev[set->length]] = hole; }
}
