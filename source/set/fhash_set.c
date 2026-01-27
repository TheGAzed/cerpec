#include <set/fhash_set.h>

#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

/// @brief Fills hole produced by element removal in arrays of elements.
/// @param set Structure to remove hole from.
/// @param hole Index of hole in arrays.
void _fhash_set_fill_hole(fhash_set_s const * const set, size_t const hole);

/// @brief Insert logic wrapper mainly to remove repeated code snippeds.
/// @param set Structure to call insert logic on.
/// @param hash Hash of element to be inserted.
/// @param index Head list index.
/// @note Elements aren't added as shallow (memcpy) or deep (custom copy function) copying may occur.
void _insert_wrapper_fhash_set(fhash_set_s const * const set, size_t const hash, size_t const index);

/// @brief Contains logic wrapper mainly to remove repeated code snippeds.
/// @param set Structure to call contains logic on.
/// @param element Element to check if it's contained.
/// @param hash Hash of element to be contained.
/// @param index Head list index.
/// @return 'true' if element is contained, 'false' otherwise.
bool _contains_wrapper_fhash_set(fhash_set_s const * const set, void const * const element, size_t const hash, size_t const index);

fhash_set_s create_fhash_set(size_t const size, size_t const max, hash_fn const hash, compare_fn const compare) {
    error(hash && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");

    fhash_set_s const set = {
        .size = size, .hash = hash, .allocator = &standard, .max = max, .compare = compare,

        .elements = standard.alloc(max * size, standard.arguments),
        .hashes = standard.alloc(max * sizeof(size_t), standard.arguments),

        .head = standard.alloc(max * sizeof(size_t), standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
        .prev = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    error(set.elements && "Memory allocation failed.");
    error(set.hashes && "Memory allocation failed.");

    error(set.head && "Memory allocation failed.");
    error(set.next && "Memory allocation failed.");
    error(set.prev && "Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        set.head[i] = NIL;
    }

    return set;
}

fhash_set_s make_fhash_set(size_t const size, size_t const max, hash_fn const hash, compare_fn const compare, memory_s const * const allocator) {
    error(hash && "Parameter can't be NULL.");
    error(compare && "Parameter can't be NULL.");
    error(size && "Parameter can't be zero.");
    error(max && "Parameter can't be zero.");

    fhash_set_s const set = {
        .size = size, .hash = hash, .allocator = allocator, .max = max, .compare = compare,

        .elements = allocator->alloc(max * size, allocator->arguments),
        .hashes = allocator->alloc(max * sizeof(size_t), allocator->arguments),

        .head = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .prev = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    error(set.elements && "Memory allocation failed.");
    error(set.hashes && "Memory allocation failed.");
    error(set.head && "Memory allocation failed.");
    error(set.next && "Memory allocation failed.");
    error(set.prev && "Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        set.head[i] = NIL;
    }

    return set;
}

void destroy_fhash_set(fhash_set_s * const set, set_fn const destroy) {
    error(set && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

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
    memset(set, 0, sizeof(fhash_set_s));
}

void clear_fhash_set(fhash_set_s * const set, set_fn const destroy) {
    error(set && "Parameter can't be NULL.");
    error(destroy && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->length; ++i) {
        destroy(set->elements + (i * set->size));
    }

    for (size_t i = 0; i < set->max; ++i) {
        set->head[i] = NIL;
    }

    // only clear set (keep the set usable)
    set->length = 0;
}

fhash_set_s copy_fhash_set(fhash_set_s const * const set, copy_fn const copy) {
    error(set && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    fhash_set_s const replica = {
        .max = set->max, .hash = set->hash, .length = set->length, .size = set->size,
        .allocator = set->allocator, .compare = set->compare,

        .elements = set->allocator->alloc(set->max * set->size, set->allocator->arguments),
        .hashes = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),

        .head = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),
        .next = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),
        .prev = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),
    };
    error(replica.elements && "Memory allocation failed.");
    error(replica.hashes && "Memory allocation failed.");

    error(replica.head && "Memory allocation failed.");
    error(replica.next && "Memory allocation failed.");
    error(replica.prev && "Memory allocation failed.");

    for (size_t i = 0; i < set->max; ++i) {
        replica.head[i] = set->head[i]; // initially set replica heads to invalid
    }

    memcpy(replica.head, set->head, set->max * sizeof(size_t)); // special case - heads index based on maximum length

    memcpy(replica.next, set->next, set->length * sizeof(size_t));
    memcpy(replica.prev, set->prev, set->length * sizeof(size_t));
    memcpy(replica.hashes, set->hashes, set->length * sizeof(size_t));

    // for each element continuusly in array call copy function
    for (size_t i = 0; i < set->length; ++i) {
        copy(replica.elements + (i * replica.size), set->elements + (i * set->size));
    }

    return replica;
}

bool is_empty_fhash_set(fhash_set_s const * const set) {
    error(set && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    return !(set->length); // if 0 return 'true'
}

bool is_full_fhash_set(fhash_set_s const * const set) {
    error(set && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    return (set->length == set->max);
}

void insert_fhash_set(fhash_set_s * const set, void const * const element) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    // check if element is in set or not
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        void const * const current = set->elements + (n * set->size);
        error((hash != set->hashes[n] || set->compare(element, current)) && "Element already in map.");
    }

    _insert_wrapper_fhash_set(set, hash, index);
    memcpy(set->elements + (set->length * set->size), element, set->size);
    set->length++;
}

void remove_fhash_set(fhash_set_s * const set, void const * const element, void * const buffer) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");
    error(buffer && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    // calculate hash values and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        const char * current = set->elements + (n * set->size);
        if (hash != set->hashes[n] || set->compare(element, current)) { // if not equal contionue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(buffer, current, set->size);
        set->length--;

        _fhash_set_fill_hole(set, n);

        return; // return to avoid errorion and termination at the end of function if element wasn't found
    }

    error(false && "Structure does not contain element.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_fhash_set(fhash_set_s const * const set, void const * const element) {
    error(set && "Parameter can't be NULL.");
    error(element && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    return _contains_wrapper_fhash_set(set, element, hash, index);
}

fhash_set_s union_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->size && "Size can't be zero.");
    valid(set_one->length <= set_one->max && "Lenght can't be larger than maximum.");
    valid(set_one->elements && "Elements array can't be NULL.");
    valid(set_one->head && "Heads array can't be NULL.");
    valid(set_one->next && "Nexts array can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");

    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->size && "Size can't be zero.");
    valid(set_two->length <= set_two->max && "Lenght can't be larger than maximum.");
    valid(set_two->elements && "Elements array can't be NULL.");
    valid(set_two->head && "Heads array can't be NULL.");
    valid(set_two->next && "Nexts array can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements to maximum's replica
    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    // copy maximum set into set union
    fhash_set_s set_union = copy_fhash_set(maximum, copy);
    for (size_t m = 0; m < minimum->length; ++m) {
        char const * const element = minimum->elements + (m * minimum->size);

        size_t const min_hash = minimum->hashes[m];
        size_t const union_idx = min_hash % set_union.max;


        // if element is not in the set push it to set union
        bool const contains = _contains_wrapper_fhash_set(&set_union, element, min_hash, union_idx);
        if (!contains) {
            error(set_union.length <= set_union.max && "Length can't exceeds maximum.");
            _insert_wrapper_fhash_set(&set_union, min_hash, union_idx);

            copy(set_union.elements + (set_union.length * set_union.size), element);
            set_union.length++;
        }
    }

    return set_union;
}

fhash_set_s intersect_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->size && "Size can't be zero.");
    valid(set_one->length <= set_one->max && "Lenght can't be larger than maximum.");
    valid(set_one->elements && "Elements array can't be NULL.");
    valid(set_one->head && "Heads array can't be NULL.");
    valid(set_one->next && "Nexts array can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");

    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->size && "Size can't be zero.");
    valid(set_two->length <= set_two->max && "Lenght can't be larger than maximum.");
    valid(set_two->elements && "Elements array can't be NULL.");
    valid(set_two->head && "Heads array can't be NULL.");
    valid(set_two->next && "Nexts array can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements
    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    fhash_set_s const * const smallest = set_one->max < set_two->max ? set_one : set_two;

    fhash_set_s set_intersect = make_fhash_set(smallest->size, smallest->max, smallest->hash, smallest->compare, smallest->allocator);
    for (size_t min = 0; min < minimum->length; ++min) {
        char const * const element = minimum->elements + (min * minimum->size);

        size_t const min_hash = minimum->hashes[min];
        size_t const max_idx = min_hash % maximum->max;


        // if element is in set push it into intersect
        bool const contains = _contains_wrapper_fhash_set(maximum, element, min_hash, max_idx);
        if (contains) {
            size_t const intersect_index = min_hash % set_intersect.max;
            _insert_wrapper_fhash_set(&set_intersect, min_hash, intersect_index);

            copy(set_intersect.elements + (set_intersect.length * set_intersect.size), element);
            set_intersect.length++;
        }
    }

    return set_intersect;
}

fhash_set_s subtract_fhash_set(fhash_set_s const * const minuend, fhash_set_s const * const subtrahend, copy_fn const copy) {
    error(minuend && "Parameter can't be NULL.");
    error(subtrahend && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(minuend->hash == subtrahend->hash && "Function pointers must be the same.");
    error(minuend->size == subtrahend->size && "Sizes must be the same.");

    valid(minuend->hash && "Hash function can't be NULL.");
    valid(minuend->size && "Size can't be zero.");
    valid(minuend->length <= minuend->max && "Lenght can't be larger than maximum.");
    valid(minuend->elements && "Elements array can't be NULL.");
    valid(minuend->head && "Heads array can't be NULL.");
    valid(minuend->next && "Nexts array can't be NULL.");
    valid(minuend->allocator && "Allocator can't be NULL.");

    valid(subtrahend->hash && "Hash function can't be NULL.");
    valid(subtrahend->size && "Size can't be zero.");
    valid(subtrahend->length <= subtrahend->max && "Lenght can't be larger than maximum.");
    valid(subtrahend->elements && "Elements array can't be NULL.");
    valid(subtrahend->head && "Heads array can't be NULL.");
    valid(subtrahend->next && "Nexts array can't be NULL.");
    valid(subtrahend->allocator && "Allocator can't be NULL.");

    fhash_set_s set_subtract = make_fhash_set(minuend->size, minuend->max, minuend->hash, minuend->compare, minuend->allocator);
    for (int min = 0; min < minuend->length; ++min) {
        // get element and set its found flag to false
        char const * const element = minuend->elements + (min * minuend->size);

        size_t const hash = minuend->hashes[min];
        size_t const subtrahend_idx = hash % subtrahend->max;

        // if minuend element is not in subtrahend set push it to new set
        bool const contains = _contains_wrapper_fhash_set(subtrahend, element, hash, subtrahend_idx);
        if (!contains) {
            size_t const subtract_index = hash % set_subtract.max;
            _insert_wrapper_fhash_set(&set_subtract, hash, subtract_index);

            copy(set_subtract.elements + (set_subtract.length * set_subtract.size), element);
            set_subtract.length++;
        }
    }

    return set_subtract;
}

fhash_set_s exclude_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(copy && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->size && "Size can't be zero.");
    valid(set_one->length <= set_one->max && "Lenght can't be larger than maximum.");
    valid(set_one->elements && "Elements array can't be NULL.");
    valid(set_one->head && "Heads array can't be NULL.");
    valid(set_one->next && "Nexts array can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");

    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->size && "Size can't be zero.");
    valid(set_two->length <= set_two->max && "Lenght can't be larger than maximum.");
    valid(set_two->elements && "Elements array can't be NULL.");
    valid(set_two->head && "Heads array can't be NULL.");
    valid(set_two->next && "Nexts array can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");

    fhash_set_s const * const biggest = set_one->max >= set_two->max ? set_one : set_two;

    fhash_set_s set_exclude = make_fhash_set(biggest->size, biggest->max, biggest->hash, biggest->compare, biggest->allocator);

    for (size_t one = 0; one < set_one->length; ++one) {
        // get element and set its found flag to false
        char const * const element = set_one->elements + (one * set_one->size);

        size_t const hash = set_one->hashes[one];
        size_t const two_idx = hash % set_two->max;

        bool const contains = _contains_wrapper_fhash_set(set_two, element, hash, two_idx);
        if (!contains) {
            error(set_exclude.length <= set_exclude.max && "Length can't exceeds maximum.");

            size_t const exclude_index = hash % set_exclude.max;
            _insert_wrapper_fhash_set(&set_exclude, hash, exclude_index);

            copy(set_exclude.elements + (set_exclude.length * set_exclude.size), element);
            set_exclude.length++;
        }
    }

    for (size_t two = 0; two < set_two->length; ++two) {
        // get element and set its found flag to false
        char const * const element = set_two->elements + (two * set_two->size);

        size_t const hash = set_two->hashes[two];
        size_t const one_idx = hash % set_one->max;

        bool const contains = _contains_wrapper_fhash_set(set_one, element, hash, one_idx);
        if (!contains) {
            error(set_exclude.length <= set_exclude.max && "Length can't exceeds maximum.");

            size_t const exclude_index = hash % set_exclude.max;
            _insert_wrapper_fhash_set(&set_exclude, hash, exclude_index);

            copy(set_exclude.elements + (set_exclude.length * set_exclude.size), element);
            set_exclude.length++;
        }
    }

    return set_exclude;
}

bool is_subset_fhash_set(fhash_set_s const * const superset, fhash_set_s const * const subset) {
    error(superset && "Parameter can't be NULL.");
    error(subset && "Parameter can't be NULL.");
    error(superset->hash == subset->hash && "Function pointers must be the same.");
    error(superset->size == subset->size && "Sizes must be the same.");

    valid(superset->hash && "Hash function can't be NULL.");
    valid(superset->size && "Size can't be zero.");
    valid(superset->length <= superset->max && "Lenght can't be larger than maximum.");
    valid(superset->elements && "Elements array can't be NULL.");
    valid(superset->head && "Heads array can't be NULL.");
    valid(superset->next && "Nexts array can't be NULL.");
    valid(superset->allocator && "Allocator can't be NULL.");

    valid(subset->hash && "Hash function can't be NULL.");
    valid(subset->size && "Size can't be zero.");
    valid(subset->length <= subset->max && "Lenght can't be larger than maximum.");
    valid(subset->elements && "Elements array can't be NULL.");
    valid(subset->head && "Heads array can't be NULL.");
    valid(subset->next && "Nexts array can't be NULL.");
    valid(subset->allocator && "Allocator can't be NULL.");

    for (size_t sub = 0; sub < subset->length; ++sub) {
        // get element and set its found flag to false
        char const * const element = subset->elements + (sub * subset->size);

        size_t const sub_hash = subset->hashes[sub];
        size_t const super_idx = sub_hash % superset->max;

        bool const contains = _contains_wrapper_fhash_set(superset, element, sub_hash, super_idx);
        if (!contains) { return false; }
    }

    return true;
}

bool is_proper_subset_fhash_set(fhash_set_s const * const superset, fhash_set_s const * const subset) {
    error(superset && "Parameter can't be NULL.");
    error(subset && "Parameter can't be NULL.");
    error(superset->hash == subset->hash && "Function pointers must be the same.");
    error(superset->size == subset->size && "Sizes must be the same.");

    valid(superset->hash && "Hash function can't be NULL.");
    valid(superset->size && "Size can't be zero.");
    valid(superset->length <= superset->max && "Lenght can't be larger than maximum.");
    valid(superset->elements && "Elements array can't be NULL.");
    valid(superset->head && "Heads array can't be NULL.");
    valid(superset->next && "Nexts array can't be NULL.");
    valid(superset->allocator && "Allocator can't be NULL.");

    valid(subset->hash && "Hash function can't be NULL.");
    valid(subset->size && "Size can't be zero.");
    valid(subset->length <= subset->max && "Lenght can't be larger than maximum.");
    valid(subset->elements && "Elements array can't be NULL.");
    valid(subset->head && "Heads array can't be NULL.");
    valid(subset->next && "Nexts array can't be NULL.");
    valid(subset->allocator && "Allocator can't be NULL.");

    for (size_t sub = 0; sub < subset->length; ++sub) {
        // get element and set its found flag to false
        char const * const element = subset->elements + (sub * subset->size);

        size_t const sub_hash = subset->hashes[sub];
        size_t const super_idx = sub_hash % superset->max;

        bool const contains = _contains_wrapper_fhash_set(superset, element, sub_hash, super_idx);
        if (!contains) { return false; }
    }

    return (subset->length != superset->length);
}

bool is_disjoint_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two) {
    error(set_one && "Parameter can't be NULL.");
    error(set_two && "Parameter can't be NULL.");
    error(set_one->hash == set_two->hash && "Function pointers must be the same.");
    error(set_one->size == set_two->size && "Sizes must be the same.");

    valid(set_one->hash && "Hash function can't be NULL.");
    valid(set_one->size && "Size can't be zero.");
    valid(set_one->length <= set_one->max && "Lenght can't be larger than maximum.");
    valid(set_one->elements && "Elements array can't be NULL.");
    valid(set_one->head && "Heads array can't be NULL.");
    valid(set_one->next && "Nexts array can't be NULL.");
    valid(set_one->allocator && "Allocator can't be NULL.");

    valid(set_two->hash && "Hash function can't be NULL.");
    valid(set_two->size && "Size can't be zero.");
    valid(set_two->length <= set_two->max && "Lenght can't be larger than maximum.");
    valid(set_two->elements && "Elements array can't be NULL.");
    valid(set_two->head && "Heads array can't be NULL.");
    valid(set_two->next && "Nexts array can't be NULL.");
    valid(set_two->allocator && "Allocator can't be NULL.");

    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    for (size_t min = 0; min < minimum->length; ++min) {
        // get element and set its found flag to false
        char const * const element = minimum->elements + (min * minimum->size);

        size_t const min_hash = minimum->hash(element);
        size_t const max_idx = min_hash % maximum->max;

        bool const contains = _contains_wrapper_fhash_set(maximum, element, min_hash, max_idx);
        if (contains) { return false; }
    }

    return true;
}

void each_fhash_set(fhash_set_s const * const set, handle_fn const handle, void * const arguments) {
    error(set && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(set->hash && "Hash function can't be NULL.");
    valid(set->size && "Size can't be zero.");
    valid(set->length <= set->max && "Lenght can't be larger than maximum.");
    valid(set->elements && "Elements array can't be NULL.");
    valid(set->head && "Heads array can't be NULL.");
    valid(set->next && "Nexts array can't be NULL.");
    valid(set->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < set->length; ++i) {
        if (!handle(set->elements + (i * set->size), arguments)) { break; }
    }
}

void _fhash_set_fill_hole(fhash_set_s const * const set, size_t const hole) {
    if (NIL == set->prev[set->length]) {
        size_t const index = set->hashes[set->length] % set->max;
        set->head[index] = hole;
    }

    if (NIL == set->prev[hole]) {
        size_t const index = set->hashes[hole] % set->max;
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

void _insert_wrapper_fhash_set(fhash_set_s const * const set, size_t const hash, size_t const index) {
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

bool _contains_wrapper_fhash_set(fhash_set_s const * const set, void const * const element, size_t const hash, size_t const index) {
    // for each node at index check if element is contained and return true or false
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        void const * const current = set->elements + (n * set->size);
        if (hash == set->hashes[n] && !set->compare(element, current)) {
            return true;
        }
    }

    return false;
}
