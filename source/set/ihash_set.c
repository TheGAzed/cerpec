#include <set/ihash_set.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NIL (size_t)(-1)

void _ihash_set_resize(ihash_set_s * set, const size_t capacity);

ihash_set_s create_ihash_set(const size_t size, const hash_fn hash) {
    assert(hash && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    return (ihash_set_s) { .size = size, .hash = hash, .empty = NIL, }; // set empty list to NIL
}

void destroy_ihash_set(ihash_set_s * set, const destroy_fn destroy) {
    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->capacity; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            destroy(set->elements + (n * set->size));
        }
    }

    // free arrays
    free(set->head);
    free(set->elements);
    free(set->next);

    memset(set, 0, sizeof(ihash_set_s)); // set everything to zero/false
}

void clear_ihash_set(ihash_set_s * set, const destroy_fn destroy) {
    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // for each index, if each index is valid node then call destroy function on its element
    for (size_t i = 0; i < set->capacity; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            destroy(set->elements + (n * set->size));
        }
    }

    // free arrays
    free(set->head);
    free(set->elements);
    free(set->next);

    // only clear set (keep the set usable)
    set->empty = NIL;
    set->capacity = set->length = 0;
    set->head = NULL;
    set->next = NULL;
    set->elements = NULL;
}

ihash_set_s copy_ihash_set(const ihash_set_s set, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(set.hash && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // create replica with allocated memory based on capacity, and empty/hole list becomes NIL
    const ihash_set_s replica = {
        .capacity = set.capacity, .empty = NIL, .hash = set.hash, .length = set.length, .size = set.size,
        .elements = malloc(set.capacity * set.size),
        .head = malloc(set.capacity * sizeof(size_t)),
        .next = malloc(set.capacity * sizeof(size_t)),
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.head) && "[ERROR] Memory allocation failed.");
    assert((!replica.capacity || replica.next) && "[ERROR] Memory allocation failed.");

    // for each index, if each index is valid node then push it into replica
    for (size_t i = 0, hole = 0; i < set.capacity; ++i) {
        replica.head[i] = NIL; // initially set replica heads to invalid

        // if set has elements in head then push them into replica heads (like a stack)
        for (size_t n = set.head[i]; NIL != n; n = set.next[n], hole++) {
            copy(replica.elements + (hole * replica.size), set.elements + (n * set.size));

            // node index redirection
            replica.next[hole] = replica.head[i];
            replica.head[i] = hole;
        }
    }

    return replica;
}

bool is_empty_ihash_set(const ihash_set_s set) {
    assert(set.hash && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !(set.length); // if 0 return 'true'
}

void insert_ihash_set(ihash_set_s * set, const void * element) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // resize (expand) if set can't contain new element
    if (set->length == set->capacity) {
        _ihash_set_resize(set, set->capacity + IHASH_SET_CHUNK);
    }

    // calculate hash value and index in array
    const size_t hash = set->hash(element);
    const size_t index = hash % set->capacity;

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

void remove_ihash_set(ihash_set_s * set, const void * element, void * buffer) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(set->hash && "[INVALID] Parameter can't be NULL.");
    assert(set->size && "[INVALID] Parameter can't be zero.");
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash value and index in array
    const size_t hash = set->hash(element);
    const size_t index = hash % set->capacity;

    // for each node at index check if element is contained
    for (size_t * n = set->head + index; NIL != (*n); n = set->next + (*n)) {
        const char * current = set->elements + ((*n) + set->size);
        if (hash != set->hash(current)) { // if not equal contionue
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
        if (set->capacity - IHASH_SET_CHUNK == set->length) {
            _ihash_set_resize(set, set->capacity - IHASH_SET_CHUNK);
        }

        return; // return to avoid assertion and termination at the end of function if element wasn't found
    }

    assert(false && "[ERROR] Structure does not contain element.");
    exit(EXIT_FAILURE); // terminate on error
}

bool contains_ihash_set(const ihash_set_s set, const void * element) {
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(set.hash && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // calculate hash value and index in array
    const size_t hash = set.hash(element);
    const size_t index = hash % set.capacity;

    // for each node at index check if element is contained and return true or false
    for (size_t n = set.head[index]; NIL != n; n = set.next[n]) {
        if (hash == set.hash(set.elements + (n * set.size))) {
            return true;
        }
    }

    return false;
}

ihash_set_s union_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.hash == set_two.hash && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements to maximum's replica
    const ihash_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const ihash_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;

    // copy maximum set into set union
    ihash_set_s set_union = copy_ihash_set(maximum, copy);

    // copy minimum set's non-contained elements into set union
    for (size_t i = 0, hole = set_union.length; i < minimum.capacity; ++i) {
        for (size_t m = minimum.head[i]; NIL != m; m = minimum.next[m]) {
            // get element and set its found flag to false
            const char * element = minimum.elements + (m * minimum.size);
            bool contains = false;

            // search for element in union set
            const size_t min_hash = minimum.hash(element);
            const size_t union_mod = min_hash % set_union.capacity;
            for (size_t u = set_union.head[union_mod]; NIL != u; u = set_union.next[u]) {
                const size_t union_hash = set_union.hash(set_union.elements + (u * set_union.size));
                if (min_hash == union_hash) { // if element was found then set flag to true and break loop
                    contains = true;
                    break;
                }
            }

            // if element is not in the set push it to set union
            if (!contains) {
                // expand union set if necessary
                if (set_union.length == set_union.capacity) {
                    _ihash_set_resize(&set_union, set_union.capacity + IHASH_SET_CHUNK);
                }

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

ihash_set_s intersect_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.hash == set_two.hash && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // get minimum and maximum sets to avoid pointless resizing via only pushing minimum set's elements
    const ihash_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const ihash_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;

    ihash_set_s set_intersect = { .size = set_one.size, .hash = set_one.hash, .empty = NIL, };
    for (size_t i = 0, hole = set_intersect.length; i < minimum.capacity; ++i) {
        for (size_t min = minimum.head[i]; NIL != min; min = minimum.next[min]) {
            // get element and set its found flag to false
            const char * element = minimum.elements + (min * minimum.size);
            bool contains = false;

            const size_t min_hash = minimum.hash(element);
            const size_t max_mod = min_hash % maximum.capacity;

            for (size_t max = maximum.head[max_mod]; NIL != max; max = maximum.next[max]) {
                const size_t max_hash = minimum.hash(maximum.elements + (max * maximum.size));
                if (min_hash == max_hash) {
                    contains = true;
                    break;
                }
            }

            if (contains) {
                // expand intersect set if necessary
                if (set_intersect.length == set_intersect.capacity) {
                    _ihash_set_resize(&set_intersect, set_intersect.capacity + IHASH_SET_CHUNK);
                }

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

ihash_set_s subtract_ihash_set(const ihash_set_s minuend, const ihash_set_s subtrahend, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(minuend.hash == subtrahend.hash && "[ERROR] Function pointers must be the same.");
    assert(minuend.size == subtrahend.size && "[ERROR] Sizes must be the same.");

    assert(minuend.hash && "[INVALID] Parameter can't be NULL.");
    assert(minuend.size && "[INVALID] Parameter can't be zero.");
    assert(minuend.length <= minuend.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(subtrahend.hash && "[INVALID] Parameter can't be NULL.");
    assert(subtrahend.size && "[INVALID] Parameter can't be zero.");
    assert(subtrahend.length <= subtrahend.capacity && "[INVALID] Lenght can't be larger than capacity.");

    ihash_set_s set_subtract = { .size = minuend.size, .hash = minuend.hash, .empty = NIL, };
    for (size_t i = 0; i < minuend.capacity; ++i) {
        for (size_t min = minuend.head[i], hole = set_subtract.length; NIL != min; min = minuend.next[min]) {
            // get element and set its found flag to false
            const char * element = minuend.elements + (min * minuend.size);
            bool contains = false;

            const size_t min_hash = minuend.hash(element);
            const size_t sub_mod = min_hash % subtrahend.capacity;

            for (size_t sub = subtrahend.head[sub_mod]; NIL != sub; sub = subtrahend.next[sub]) {
                const size_t sub_hash = subtrahend.hash(subtrahend.elements + (sub * subtrahend.size));
                if (sub_hash == min_hash) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                // expand subtract set if necessary
                if (set_subtract.length == set_subtract.capacity) {
                    _ihash_set_resize(&set_subtract, set_subtract.capacity + IHASH_SET_CHUNK);
                }

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

ihash_set_s exclude_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two, const copy_fn copy) {
    assert(copy && "[ERROR] Parameter can't be NULL.");
    assert(set_one.hash == set_two.hash && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    ihash_set_s set_exclude = { .size = set_one.size, .hash = set_one.hash, .empty = NIL, };
    for (size_t i = 0, hole = set_exclude.length; i < set_one.capacity; ++i) {
        for (size_t one = set_one.head[i]; NIL != one; one = set_one.next[one]) {
            // get element and set its found flag to false
            const char * element = set_one.elements + (one * set_one.size);
            bool contains = false;

            const size_t one_hash = set_one.hash(element);
            const size_t two_mod = one_hash % set_two.capacity;

            for (size_t two = set_two.head[two_mod]; NIL != two; two = set_two.next[two]) {
                const size_t two_hash = set_two.hash(set_two.elements + (two * set_two.size));
                if (one_hash == two_hash) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                // expand intersect set if necessary
                if (set_exclude.length == set_exclude.capacity) {
                    _ihash_set_resize(&set_exclude, set_exclude.capacity + IHASH_SET_CHUNK);
                }

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

    for (size_t i = 0, hole = set_exclude.length; i < set_one.capacity; ++i) {
        for (size_t two = set_two.head[i]; NIL != two; two = set_two.next[two]) {
            // get element and set its found flag to false
            const char * element = set_two.elements + (two * set_two.size);
            bool contains = false;

            const size_t two_hash = set_two.hash(element);
            const size_t one_mod = two_hash % set_one.capacity;

            for (size_t one = set_one.head[one_mod]; NIL != one; one = set_one.next[one]) {
                const size_t max_hash = set_one.hash(set_one.elements + (one * set_one.size));
                if (two_hash == max_hash) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                // expand intersect set if necessary
                if (set_exclude.length == set_exclude.capacity) {
                    _ihash_set_resize(&set_exclude, set_exclude.capacity + IHASH_SET_CHUNK);
                }

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

bool is_subset_ihash_set(const ihash_set_s super, const ihash_set_s sub) {
    assert(super.hash == sub.hash && "[ERROR] Function pointers must be the same.");
    assert(super.size == sub.size && "[ERROR] Sizes must be the same.");

    assert(super.hash && "[INVALID] Parameter can't be NULL.");
    assert(super.size && "[INVALID] Parameter can't be zero.");
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(sub.hash && "[INVALID] Parameter can't be NULL.");
    assert(sub.size && "[INVALID] Parameter can't be zero.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < sub.capacity; ++i) {
        for (size_t s = sub.head[i]; NIL != s; s = sub.next[s]) {
            // get element and set its found flag to false
            const char * element = sub.elements + (s * sub.size);
            bool contains = false;

            const size_t sub_hash = sub.hash(element);
            const size_t super_mod = sub_hash % super.capacity;

            for (size_t n = super.head[super_mod]; NIL != n; n = super.next[n]) {
                const size_t super_hash = super.hash(super.elements + (n * super.size));
                if (sub_hash == super_hash) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                return false;
            }
        }
    }

    return true;
}

bool is_proper_subset_ihash_set(const ihash_set_s super, const ihash_set_s sub) {
    assert(super.hash == sub.hash && "[ERROR] Function pointers must be the same.");
    assert(super.size == sub.size && "[ERROR] Sizes must be the same.");

    assert(super.hash && "[INVALID] Parameter can't be NULL.");
    assert(super.size && "[INVALID] Parameter can't be zero.");
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(sub.hash && "[INVALID] Parameter can't be NULL.");
    assert(sub.size && "[INVALID] Parameter can't be zero.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < sub.capacity; ++i) {
        for (size_t s = sub.head[i]; NIL != s; s = sub.next[s]) {
            // get element and set its found flag to false
            const char * element = sub.elements + (s * sub.size);
            bool contains = false;

            const size_t sub_hash = sub.hash(element);
            const size_t super_mod = sub_hash % super.capacity;

            for (size_t n = super.head[super_mod]; NIL != n; n = super.next[n]) {
                const size_t super_hash = super.hash(super.elements + (n * super.size));
                if (sub_hash == super_hash) {
                    contains = true;
                    break;
                }
            }

            if (!contains) {
                return false;
            }
        }
    }

    return (sub.length != super.length);
}

bool is_disjoint_ihash_set(const ihash_set_s set_one, const ihash_set_s set_two) {
    assert(set_one.hash == set_two.hash && "[ERROR] Function pointers must be the same.");
    assert(set_one.size == set_two.size && "[ERROR] Sizes must be the same.");

    assert(set_one.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_one.size && "[INVALID] Parameter can't be zero.");
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");

    assert(set_two.hash && "[INVALID] Parameter can't be NULL.");
    assert(set_two.size && "[INVALID] Parameter can't be zero.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const ihash_set_s minimum = set_one.length < set_two.length ? set_one : set_two;
    const ihash_set_s maximum = set_one.length >= set_two.length ? set_one : set_two;
    for (size_t i = 0; i < minimum.capacity; ++i) {
        for (size_t s = minimum.head[i]; NIL != s; s = minimum.next[s]) {
            // get element and set its found flag to false
            const char * element = minimum.elements + (s * minimum.size);
            bool contains = false;

            const size_t min_hash = minimum.hash(element);
            const size_t max_mod = min_hash % maximum.capacity;

            for (size_t n = maximum.head[max_mod]; NIL != n; n = maximum.next[n]) {
                const size_t max_hash = maximum.hash(maximum.elements + (n * maximum.size));
                if (min_hash == max_hash) {
                    contains = true;
                    break;
                }
            }

            if (contains) {
                return false;
            }
        }
    }

    return true;
}

void foreach_ihash_set(const ihash_set_s set, const operate_fn operate, void * arguments) {
    assert(operate && "[ERROR] Parameter can't be NULL.");

    assert(set.hash && "[INVALID] Parameter can't be NULL.");
    assert(set.size && "[INVALID] Parameter can't be zero.");
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < set.capacity; ++i) {
        for (size_t n = set.head[i]; NIL != n; n = set.next[n]) {
            if (!operate(set.elements + (n * set.size), arguments)) {
                return;
            }
        }
    }
}

void _ihash_set_resize(ihash_set_s * set, const size_t capacity) {
    char * elements = malloc(set->length * set->size);
    assert((!set->length || elements) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < set->capacity; ++i) {
        for (size_t n = set->head[i]; NIL != n; n = set->next[n]) {
            memcpy(elements, set->elements + (n * set->size), set->size);
        }
    }

    set->capacity = capacity;
    set->elements = realloc(set->elements, set->capacity * set->size);
    set->head     = realloc(set->head, set->capacity * sizeof(size_t));
    set->next     = realloc(set->next, set->capacity * sizeof(size_t));

    assert((!set->capacity || set->elements) && "[ERROR] Memory allocation failed.");
    assert((!set->capacity || set->head) && "[ERROR] Memory allocation failed.");
    assert((!set->capacity || set->next) && "[ERROR] Memory allocation failed.");

    set->empty = NIL;
    for (size_t i = 0; i < set->capacity; ++i) {
        set->head[i] = NIL;
    }

    for (size_t i = 0, hole = 0; i < set->length; ++i, ++hole) {
        const size_t hash = set->hash(elements + (i * set->size));
        const size_t mod = hash % set->capacity;

        set->next[hole] = set->head[mod];
        set->head[mod] = hole;
    }
}
