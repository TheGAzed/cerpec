#include <set/fhash_set.h>

#include <assert.h>
#include <stdlib.h> // imports exit()
#include <string.h>

#define NIL (size_t)(-1)

fhash_set_s create_fhash_set(size_t const size, size_t const max, hash_fn const hash) {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");

    fhash_set_s const set = {
        .size = size, .hash = hash, .empty = NIL, .allocator = &standard, .max = max,
        .elements = standard.alloc(max * size, standard.arguments),
        .head = standard.alloc(max * sizeof(size_t), standard.arguments),
        .next = standard.alloc(max * sizeof(size_t), standard.arguments),
    };
    assert(set.elements && "[ERROR] Memory allocation failed.");
    assert(set.head && "[ERROR] Memory allocation failed.");
    assert(set.next && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        set.head[i] = set.next[i] = NIL;
    }

    return set;
}

fhash_set_s make_fhash_set(size_t const size, size_t const max, hash_fn const hash, memory_s const * const allocator) {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");
    assert(max && "[ERROR] Parameter can't be zero.");

    fhash_set_s const set = {
        .size = size, .hash = hash, .empty = NIL, .allocator = allocator, .max = max,
        .elements = allocator->alloc(max * size, allocator->arguments),
        .head = allocator->alloc(max * sizeof(size_t), allocator->arguments),
        .next = allocator->alloc(max * sizeof(size_t), allocator->arguments),
    };
    assert(set.elements && "[ERROR] Memory allocation failed.");
    assert(set.head && "[ERROR] Memory allocation failed.");
    assert(set.next && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < max; ++i) {
        set.head[i] = set.next[i] = NIL;
    }

    return set;
}

void destroy_fhash_set(fhash_set_s * const set, set_fn const destroy) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->max; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            destroy(set->elements + (n * set->size));
        }
    }

    // free arrays
    set->allocator->free(set->head, set->allocator->arguments);
    set->allocator->free(set->elements, set->allocator->arguments);
    set->allocator->free(set->next, set->allocator->arguments);

    // set everything to zero/false
    set->max = set->empty = set->length = set->size = 0;
    set->allocator = NULL;
    set->elements = NULL;
    set->hash = NULL;
    set->head = set->next = NULL;
}

void clear_fhash_set(fhash_set_s * const set, set_fn const destroy) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->max; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            destroy(set->elements + (n * set->size));
        }
    }

    // only clear set (keep the set usable)
    set->empty = NIL;
    set->length = 0;
}

fhash_set_s copy_fhash_set(fhash_set_s const * const set, copy_fn const copy) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    fhash_set_s const replica = {
        .max = set->max, .empty = NIL, .hash = set->hash, .length = set->length, .size = set->size,
        .allocator = set->allocator,
        .elements = set->allocator->alloc(set->max * set->size, set->allocator->arguments),
        .head = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),
        .next = set->allocator->alloc(set->max * sizeof(size_t), set->allocator->arguments),
    };
    assert(replica.elements && "[ERROR] Memory allocation failed.");
    assert(replica.head && "[ERROR] Memory allocation failed.");
    assert(replica.next && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < set->max; ++i) {
        replica.head[i] = replica.next[i] = NIL; // initially set replica heads to invalid
    }

    // for each index, if each index is valid node then push it into replica
    for (size_t i = 0, hole = 0; i < set->max; ++i) {
        // if set has elements in head then push them into replica heads (like a stack)
        for (size_t n = set->head[i]; NIL != n; n = set->next[n], hole++) {
            copy(replica.elements + (hole * replica.size), set->elements + (n * set->size));

            // node index redirection
            replica.next[hole] = replica.head[i];
            replica.head[i] = hole;
        }
    }

    return replica;
}

bool is_empty_fhash_set(fhash_set_s const * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    return !(set->length); // if 0 return 'true'
}

bool is_full_fhash_set(fhash_set_s const * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    return (set->length == set->max);
}

void insert_fhash_set(fhash_set_s * const set, void const * const element) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    // check if element is in set or not
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        assert(hash != set->hash(set->elements + (n * set->size)) && "[ERROR] Element already in set.");
    }

    // get hole/empty index
    size_t hole = set->length;
    if (NIL != set->empty) { // if empty has a valid index then pop it and set it as hole
        hole = set->empty;
        set->empty = set->next[set->empty];
    }

    // node index redirection
    set->next[hole] = set->head[index];
    set->head[index] = hole;

    // copy element into elements array
    memcpy(set->elements + (hole * set->size), element, set->size);
    set->length++;
}

void remove_fhash_set(fhash_set_s * const set, void const * const element, void * const buffer) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    // for each node at index check if element is contained
    for (size_t * n = set->head + index; NIL != (*n); n = set->next + (*n)) {
        char const * const current = set->elements + ((*n) * set->size);
        if (hash != set->hash(current)) { // if not equal continue
            continue;
        } // else remove found element and return

        // copy removed element into buffer
        memcpy(buffer, current, set->size);
        set->length--;

        // redirect node using pointer magic
        const size_t hole = (*n);
        (*n) = set->next[(*n)];

        // if hole isn't last index in array append it into empty stack
        if (hole != set->length) {
            set->next[hole] = set->empty;
            set->empty = hole;
        }

        // resize (expand) if set can contain a smaller capacity of elements
        if (!set->length) {
            set->empty = NIL;
        }

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    assert(false && "[ERROR] Structure does not contain element.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_fhash_set(fhash_set_s const * const set, void const * const element) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    // calculate hash value and index in array
    size_t const hash = set->hash(element);
    size_t const index = hash % set->max;

    // for each node at index check if element is contained and return true or false
    for (size_t n = set->head[index]; NIL != n; n = set->next[n]) {
        if (hash == set->hash(set->elements + (n * set->size))) {
            return true;
        }
    }

    return false;
}

fhash_set_s union_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one->hash == set_two->hash && "[ERROR] Function pointers must be the same.");
    assert(set_one->size == set_two->size && "[ERROR] Sizes must be the same.");

    assert(set_one->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one->size && "[INVALID] Parameter can't be zero.");
    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(set_two->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two->size && "[INVALID] Parameter can't be zero.");
    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements to maximum's replica
    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    // copy maximum set into set union
    fhash_set_s set_union = copy_fhash_set(maximum, copy);

    // copy minimum set's non-contained elements into set union
    for (size_t i = 0, hole = set_union.length; i < minimum->max; ++i) {
        for (size_t m = minimum->head[i]; NIL != m; m = minimum->next[m]) {
            // get element and set its found flag to false
            char const * const element = minimum->elements + (m * minimum->size);
            bool contains_flag = false;

            // search for element in union set
            size_t const min_hash = minimum->hash(element);
            size_t const union_mod = min_hash % set_union.max;
            for (size_t u = set_union.head[union_mod]; NIL != u; u = set_union.next[u]) {
                size_t const union_hash = set_union.hash(set_union.elements + (u * set_union.size));
                if (min_hash == union_hash) { // if element was found then set flag to true and break loop
                    contains_flag = true;
                    break;
                }
            }

            // if element is not in the set push it to set union
            if (!contains_flag) {
                assert(set_union.length <= set_union.max && "[ERROR] Length can't exceeds maximum.");

                // copy element into set union
                copy(set_union.elements + (hole * set_union.size), element);
                set_union.length++;

                // redirect/push new list node
                set_union.next[hole] = set_union.head[i];
                set_union.head[i] = hole;

                hole++; // go to next empty array node
            }
        }
    }

    return set_union;
}

fhash_set_s intersect_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one->hash == set_two->hash && "[ERROR] Function pointers must be the same.");
    assert(set_one->size == set_two->size && "[ERROR] Sizes must be the same.");

    assert(set_one->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one->size && "[INVALID] Parameter can't be zero.");
    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(set_two->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two->size && "[INVALID] Parameter can't be zero.");
    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements
    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;

    fhash_set_s const * const smallest = set_one->max < set_two->max ? set_one : set_two;

    fhash_set_s set_intersect = make_fhash_set(smallest->size, smallest->max, smallest->hash, smallest->allocator);
    for (size_t i = 0, hole = set_intersect.length; i < minimum->max; ++i) {
        for (size_t min = minimum->head[i]; NIL != min; min = minimum->next[min]) {
            // get element and set its found flag to false
            char const * const element = minimum->elements + (min * minimum->size);
            bool contains_flag = false;

            size_t const min_hash = minimum->hash(element);
            size_t const max_mod = min_hash % maximum->max;

            for (size_t max = maximum->head[max_mod]; NIL != max; max = maximum->next[max]) {
                size_t const max_hash = minimum->hash(maximum->elements + (max * maximum->size));
                if (min_hash == max_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (contains_flag) {
                // copy element into set intersect
                copy(set_intersect.elements + (hole * set_intersect.size), element);
                set_intersect.length++;

                // redirect/push new list node
                set_intersect.next[hole] = set_intersect.head[i];
                set_intersect.head[i] = hole;

                hole++; // go to next empty array node
            }
        }
    }

    return set_intersect;
}

fhash_set_s subtract_fhash_set(fhash_set_s const * const minuend, fhash_set_s const * const subtrahend, copy_fn const copy) {
    assert(minuend && "[ERROR] Parameter can't be NULL.");
    assert(subtrahend && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(minuend->hash == subtrahend->hash && "[ERROR] Function pointers must be the same.");
    assert(minuend->size == subtrahend->size && "[ERROR] Sizes must be the same.");

    assert(minuend->hash && "[INVALID] Parameter can't be NULL.");
    assert(minuend->size && "[INVALID] Parameter can't be zero.");
    assert(minuend->length <= minuend->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(subtrahend->hash && "[INVALID] Parameter can't be NULL.");
    assert(subtrahend->size && "[INVALID] Parameter can't be zero.");
    assert(subtrahend->length <= subtrahend->max && "[INVALID] Lenght can't be larger than maximum.");

    fhash_set_s set_subtract = make_fhash_set(minuend->size, minuend->max, minuend->hash, minuend->allocator);
    for (size_t i = 0; i < minuend->max; ++i) {
        for (size_t min = minuend->head[i], hole = set_subtract.length; NIL != min; min = minuend->next[min]) {
            // get element and set its found flag to false
            char const * const element = minuend->elements + (min * minuend->size);
            bool contains_flag = false;

            size_t const min_hash = minuend->hash(element);
            size_t const sub_mod = min_hash % subtrahend->max;

            for (size_t sub = subtrahend->head[sub_mod]; NIL != sub; sub = subtrahend->next[sub]) {
                size_t const sub_hash = subtrahend->hash(subtrahend->elements + (sub * subtrahend->size));
                if (sub_hash == min_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (!contains_flag) {
                // copy element into set subtract
                copy(set_subtract.elements + (hole * set_subtract.size), element);
                set_subtract.length++;

                // redirect/push new list node
                set_subtract.next[hole] = set_subtract.head[i];
                set_subtract.head[i] = hole;

                hole++; // go to next empty array node
            }
        }
    }

    return set_subtract;
}

fhash_set_s exclude_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two, copy_fn const copy) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one->hash == set_two->hash && "[ERROR] Function pointers must be the same.");
    assert(set_one->size == set_two->size && "[ERROR] Sizes must be the same.");

    assert(set_one->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one->size && "[INVALID] Parameter can't be zero.");
    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(set_two->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two->size && "[INVALID] Parameter can't be zero.");
    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");

    fhash_set_s const * const biggest = set_one->max >= set_two->max ? set_one : set_two;

    fhash_set_s set_exclude = make_fhash_set(biggest->size, biggest->max, biggest->hash, biggest->allocator);
    for (size_t i = 0, hole = set_exclude.length; i < set_one->max; ++i) {
        for (size_t one = set_one->head[i]; NIL != one; one = set_one->next[one]) {
            // get element and set its found flag to false
            char const * const element = set_one->elements + (one * set_one->size);
            bool contains_flag = false;

            size_t const one_hash = set_one->hash(element);
            size_t const two_mod = one_hash % set_two->max;

            for (size_t two = set_two->head[two_mod]; NIL != two; two = set_two->next[two]) {
                size_t const two_hash = set_two->hash(set_two->elements + (two * set_two->size));
                if (one_hash == two_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (!contains_flag) {
                assert(set_exclude.length <= set_exclude.max && "[ERROR] Length can't exceeds maximum.");

                // copy element into set intersect
                copy(set_exclude.elements + (hole * set_exclude.size), element);
                set_exclude.length++;

                // redirect/push new list node
                set_exclude.next[hole] = set_exclude.head[i];
                set_exclude.head[i] = hole;

                hole++; // go to next empty array node
            }
        }
    }

    for (size_t i = 0, hole = set_exclude.length; i < set_one->max; ++i) {
        for (size_t two = set_two->head[i]; NIL != two; two = set_two->next[two]) {
            // get element and set its found flag to false
            char const * const element = set_two->elements + (two * set_two->size);
            bool contains_flag = false;

            size_t const two_hash = set_two->hash(element);
            size_t const one_mod = two_hash % set_one->max;

            for (size_t one = set_one->head[one_mod]; NIL != one; one = set_one->next[one]) {
                size_t const max_hash = set_one->hash(set_one->elements + (one * set_one->size));
                if (two_hash == max_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (!contains_flag) {
                assert(set_exclude.length <= set_exclude.max && "[ERROR] Length can't exceeds maximum.");

                // copy element into set intersect
                copy(set_exclude.elements + (hole * set_exclude.size), element);
                set_exclude.length++;

                // redirect/push new list node
                set_exclude.next[hole] = set_exclude.head[i];
                set_exclude.head[i] = hole;

                hole++; // go to next empty array node
            }
        }
    }

    return set_exclude;
}

bool is_subset_fhash_set(fhash_set_s const * const superset, fhash_set_s const * const subset) {
    assert(superset && "[ERROR] Parameter can't be NULL.");
    assert(subset && "[ERROR] Parameter can't be NULL.");
    assert(superset->hash == subset->hash && "[ERROR] Function pointers must be the same.");
    assert(superset->size == subset->size && "[ERROR] Sizes must be the same.");

    assert(superset->hash && "[INVALID] Parameter can't be NULL.");
    assert(superset->size && "[INVALID] Parameter can't be zero.");
    assert(superset->length <= superset->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(subset->hash && "[INVALID] Parameter can't be NULL.");
    assert(subset->size && "[INVALID] Parameter can't be zero.");
    assert(subset->length <= subset->max && "[INVALID] Lenght can't be larger than maximum.");

    for (size_t i = 0; i < subset->max; ++i) {
        for (size_t s = subset->head[i]; NIL != s; s = subset->next[s]) {
            // get element and set its found flag to false
            char const * const element = subset->elements + (s * subset->size);
            bool contains_flag = false;

            size_t const sub_hash = subset->hash(element);
            size_t const super_mod = sub_hash % superset->max;

            for (size_t n = superset->head[super_mod]; NIL != n; n = superset->next[n]) {
                size_t const super_hash = superset->hash(superset->elements + (n * superset->size));
                if (sub_hash == super_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (!contains_flag) {
                return false;
            }
        }
    }

    return true;
}

bool is_proper_subset_fhash_set(fhash_set_s const * const superset, fhash_set_s const * const subset) {
    assert(superset && "[ERROR] Parameter can't be NULL.");
    assert(subset && "[ERROR] Parameter can't be NULL.");
    assert(superset->hash == subset->hash && "[ERROR] Function pointers must be the same.");
    assert(superset->size == subset->size && "[ERROR] Sizes must be the same.");

    assert(superset->hash && "[INVALID] Parameter can't be NULL.");
    assert(superset->size && "[INVALID] Parameter can't be zero.");
    assert(superset->length <= superset->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(subset->hash && "[INVALID] Parameter can't be NULL.");
    assert(subset->size && "[INVALID] Parameter can't be zero.");
    assert(subset->length <= subset->max && "[INVALID] Lenght can't be larger than maximum.");

    for (size_t i = 0; i < subset->max; ++i) {
        for (size_t s = subset->head[i]; NIL != s; s = subset->next[s]) {
            // get element and set its found flag to false
            char const * const element = subset->elements + (s * subset->size);
            bool contains_flag = false;

            size_t const sub_hash = subset->hash(element);
            size_t const super_mod = sub_hash % superset->max;

            for (size_t n = superset->head[super_mod]; NIL != n; n = superset->next[n]) {
                size_t const super_hash = superset->hash(superset->elements + (n * superset->size));
                if (sub_hash == super_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (!contains_flag) {
                return false;
            }
        }
    }

    return (subset->length != superset->length);
}

bool is_disjoint_fhash_set(fhash_set_s const * const set_one, fhash_set_s const * const set_two) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");
    assert(set_one->hash == set_two->hash && "[ERROR] Function pointers must be the same.");
    assert(set_one->size == set_two->size && "[ERROR] Sizes must be the same.");

    assert(set_one->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one->size && "[INVALID] Parameter can't be zero.");
    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");

    assert(set_two->hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two->size && "[INVALID] Parameter can't be zero.");
    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");

    fhash_set_s const * const minimum = set_one->length < set_two->length ? set_one : set_two;
    fhash_set_s const * const maximum = set_one->length >= set_two->length ? set_one : set_two;
    for (size_t i = 0; i < minimum->max; ++i) {
        for (size_t s = minimum->head[i]; NIL != s; s = minimum->next[s]) {
            // get element and set its found flag to false
            char const * const element = minimum->elements + (s * minimum->size);
            bool contains_flag = false;

            size_t const min_hash = minimum->hash(element);
            size_t const max_mod = min_hash % maximum->max;

            for (size_t n = maximum->head[max_mod]; NIL != n; n = maximum->next[n]) {
                size_t const max_hash = maximum->hash(maximum->elements + (n * maximum->size));
                if (min_hash == max_hash) {
                    contains_flag = true;
                    break;
                }
            }

            if (contains_flag) {
                return false;
            }
        }
    }

    return true;
}

void map_fhash_set(fhash_set_s const * const set, handle_fn const handle, void * const arguments) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");

    for (size_t i = 0; i < set->max; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            if (!handle(set->elements + (n * set->size), arguments)) {
                return;
            }
        }
    }
}
