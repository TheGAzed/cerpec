#include <set/ibitwise_set.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BIT_COUNT (sizeof(unsigned) * CHAR_BIT)

/// @brief Calculates the number of set (one) bits in unsigned integer.
/// @param bits Unsigned integer to count set bits.
/// @return Number of set bits.
size_t _ibitwise_set_popcount(const unsigned bits);

/// @brief Resizes (reallocates) structure parameter arrays based on changed capacity.
/// @param set Structure to resize.
/// @param size New size.
void _ibitwise_set_resize(ibitwise_set_s * set, const size_t size);

ibitwise_set_s create_ibitwise_set(void) {
    return (ibitwise_set_s) { 0 };
}

void destroy_ibitwise_set(ibitwise_set_s * set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    free(set->bits);

    memset(set, 0, sizeof(ibitwise_set_s));
}

void clear_ibitwise_set(ibitwise_set_s * set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    free(set->bits);

    memset(set, 0, sizeof(ibitwise_set_s));
}

ibitwise_set_s copy_ibitwise_set(const ibitwise_set_s set) {
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const ibitwise_set_s replica = {
        .capacity = set.capacity, .length = set.length,
        .bits = malloc(set.capacity * sizeof(unsigned)),
    };
    assert((!replica.capacity || replica.bits) && "[ERROR] Memory allocation failed.");

    memcpy(replica.bits, set.bits, replica.capacity / CHAR_BIT);

    return replica;
}

bool is_empty_ibitwise_set(const ibitwise_set_s set) {
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    return !(set.length);
}

void insert_ibitwise_set(ibitwise_set_s * set, const size_t index) {
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    const size_t idx = index / BIT_COUNT;
    const unsigned relative = (unsigned)(index % BIT_COUNT);
    const unsigned bit = 1U << (BIT_COUNT - relative - 1);

    if (idx >= set->capacity) {
        const size_t capacity = idx - (size_t)relative + IBITWISE_SET_CHUNK;
        _ibitwise_set_resize(set, capacity);
    }

    assert(!(set->bits[idx] & bit) && "[ERROR] Structure already contains element.");

    set->bits[idx] |= bit;
    set->length++;
}

void remove_ibitwise_set(ibitwise_set_s * set, const size_t index) {
    assert(set->length <= set->capacity && "[INVALID] Lenght can't be larger than capacity.");

    const size_t idx = index / BIT_COUNT;
    const unsigned relative = (unsigned)(index % BIT_COUNT);
    const unsigned bit = 1U << (BIT_COUNT - relative - 1);

    assert((set->bits[idx] & bit) && "[ERROR] Strucutre already contains element.");

    set->bits[idx] ^= bit;
    set->length--;

    // if new set has no leftmost chunk bits set then shrink bits array until it has
    size_t capacity = set->capacity;
    for (size_t i = set->capacity / IBITWISE_SET_CHUNK; i; --i) {
        for (size_t j = 0; j < IBITWISE_SET_CHUNK / BIT_COUNT; ++j) {
            if (set->bits[(set->capacity / BIT_COUNT) - j - 1]) {
                goto EXIT_LOOP;
            }
        }
        capacity -= IBITWISE_SET_CHUNK;
    }

EXIT_LOOP:
    // if capacity has shrunk then shrink new set as well
    if (set->capacity != capacity) {
        _ibitwise_set_resize(set, capacity);
    }
}

bool contains_ibitwise_set(const ibitwise_set_s set, const size_t index) {
    assert(set.length <= set.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const size_t idx = index / BIT_COUNT;
    const unsigned relative = (unsigned)(index % BIT_COUNT);
    const unsigned bit = 1U << (BIT_COUNT - relative - 1);

    return (bool)(set.bits[idx] & bit);
}

ibitwise_set_s union_ibitwise_set(const ibitwise_set_s set_one, const ibitwise_set_s set_two) {
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const ibitwise_set_s minimum = set_one.capacity < set_two.capacity ? set_one : set_two;
    const ibitwise_set_s maximum = set_one.capacity >= set_two.capacity ? set_one : set_two;

    ibitwise_set_s set_union = copy_ibitwise_set(maximum);
    set_union.length = 0;
    for (size_t i = 0; i < minimum.capacity / BIT_COUNT; ++i) {
        set_union.bits[i] |= minimum.bits[i];
        set_union.length += _ibitwise_set_popcount(set_union.bits[i]);
    }

    return set_union;
}

ibitwise_set_s intersect_ibitwise_set(const ibitwise_set_s set_one, const ibitwise_set_s set_two) {
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const ibitwise_set_s minimum = set_one.capacity < set_two.capacity ? set_one : set_two;
    const ibitwise_set_s maximum = set_one.capacity >= set_two.capacity ? set_one : set_two;

    ibitwise_set_s set_intersect = copy_ibitwise_set(minimum);
    set_intersect.length = 0;
    for (size_t i = 0; i < minimum.capacity / BIT_COUNT; ++i) {
        set_intersect.bits[i] |= maximum.bits[i];
        set_intersect.length += _ibitwise_set_popcount(set_intersect.bits[i]);
    }

    // if new set has no leftmost chunk bits set then shrink bits array until it has
    size_t capacity = set_intersect.capacity;
    for (size_t i = set_intersect.capacity / IBITWISE_SET_CHUNK; i; --i) {
        for (size_t j = 0; j < IBITWISE_SET_CHUNK / BIT_COUNT; ++j) {
            if (set_intersect.bits[(set_intersect.capacity / BIT_COUNT) - j - 1]) {
                goto EXIT_LOOP;
            }
        }
        capacity -= IBITWISE_SET_CHUNK;
    }

EXIT_LOOP:
    // if capacity has shrunk then shrink new set as well
    if (set_intersect.capacity != capacity) {
        _ibitwise_set_resize(&set_intersect, capacity);
    }

    return set_intersect;
}

ibitwise_set_s subtract_ibitwise_set(const ibitwise_set_s minuend, const ibitwise_set_s subtrahend) {
    assert(minuend.length <= minuend.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(subtrahend.length <= subtrahend.capacity && "[INVALID] Lenght can't be larger than capacity.");

    // get smallest set based on capacity to minimize operations and invalid array access
    const ibitwise_set_s minimum = minuend.capacity < subtrahend.capacity ? minuend : subtrahend;

    ibitwise_set_s set_subtract = copy_ibitwise_set(minuend);
    set_subtract.length = 0;
    for (size_t i = 0; i < minimum.capacity / BIT_COUNT; ++i) {
        set_subtract.bits[i] &= ~(subtrahend.bits[i]);
        set_subtract.length += _ibitwise_set_popcount(set_subtract.bits[i]);
    }

    // if new set has no leftmost chunk bits set then shrink bits array until it has
    size_t capacity = set_subtract.capacity;
    for (size_t i = set_subtract.capacity / IBITWISE_SET_CHUNK; i; --i) {
        for (size_t j = 0; j < IBITWISE_SET_CHUNK / BIT_COUNT; ++j) {
            if (set_subtract.bits[(set_subtract.capacity / BIT_COUNT) - j - 1]) {
                goto EXIT_LOOP;
            }
        }
        capacity -= IBITWISE_SET_CHUNK;
    }

EXIT_LOOP:
    // if capacity has shrunk then shrink new set as well
    if (set_subtract.capacity != capacity) {
        _ibitwise_set_resize(&set_subtract, capacity);
    }

    return set_subtract;
}

ibitwise_set_s exclude_ibitwise_set(const ibitwise_set_s set_one, const ibitwise_set_s set_two) {
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    const ibitwise_set_s minimum = set_one.capacity < set_two.capacity ? set_one : set_two;
    const ibitwise_set_s maximum = set_one.capacity >= set_two.capacity ? set_one : set_two;

    ibitwise_set_s set_exclude = copy_ibitwise_set(minimum);
    set_exclude.length = 0;
    for (size_t i = 0; i < minimum.capacity / BIT_COUNT; ++i) {
        set_exclude.bits[i] ^= maximum.bits[i];
        set_exclude.length += _ibitwise_set_popcount(set_exclude.bits[i]);
    }

    // if new set has no leftmost chunk bits set then shrink bits array until it has
    size_t capacity = set_exclude.capacity;
    for (size_t i = set_exclude.capacity / IBITWISE_SET_CHUNK; i; --i) {
        for (size_t j = 0; j < IBITWISE_SET_CHUNK / BIT_COUNT; ++j) {
            if (set_exclude.bits[(set_exclude.capacity / BIT_COUNT) - j - 1]) {
                goto EXIT_LOOP;
            }
        }
        capacity -= IBITWISE_SET_CHUNK;
    }

EXIT_LOOP:
    // if capacity has shrunk then shrink new set as well
    if (set_exclude.capacity != capacity) {
        _ibitwise_set_resize(&set_exclude, capacity);
    }

    return set_exclude;
}

bool is_subset_ibitwise_set(const ibitwise_set_s super, const ibitwise_set_s sub) {
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < sub.capacity / BIT_COUNT && i < super.capacity / BIT_COUNT; ++i) {
        if (sub.bits[i] & super.bits[i] != sub.bits[i]) {
            return false;
        }
    }

    return (sub.capacity <= super.capacity);
}

bool is_proper_subset_ibitwise_set(const ibitwise_set_s super, const ibitwise_set_s sub) {
    assert(super.length <= super.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(sub.length <= sub.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < sub.capacity / BIT_COUNT && i < super.capacity / BIT_COUNT; ++i) {
        if (sub.bits[i] & super.bits[i] != sub.bits[i]) {
            return false;
        }
    }

    return (sub.capacity <= super.capacity) && (sub.length == super.length);
}

bool is_disjoint_ibitwise_set(const ibitwise_set_s set_one, const ibitwise_set_s set_two) {
    assert(set_one.length <= set_one.capacity && "[INVALID] Lenght can't be larger than capacity.");
    assert(set_two.length <= set_two.capacity && "[INVALID] Lenght can't be larger than capacity.");

    for (size_t i = 0; i < set_two.capacity / BIT_COUNT && i < set_one.capacity / BIT_COUNT; ++i) {
        if (set_two.bits[i] & set_one.bits[i]) {
            return false;
        }
    }

    return true;
}

void foreach_index_ibitwise_set(const ibitwise_set_s set, const operate_fn operate, void * arguments) {
    for (size_t i = 0; i < set.capacity; ++i) {
        size_t temp = i;

        // calculate bits' array index and bit to check
        const size_t idx = i / BIT_COUNT;
        const unsigned relative = ((unsigned)(i) % BIT_COUNT);
        const unsigned bit = 1U << (BIT_COUNT - relative - 1);

        // if bit isn't set continue, and if operate function returns false break the loop and quit main function
        if ((set.bits[idx] & bit) && !operate(&temp, arguments)) {
            break;
        }
    }
}

size_t _ibitwise_set_popcount(const unsigned bits) {
#if defined(__GNUC__) || defined(__clang__) // if either gcc or clang is used, '__builtin_*' support exists
    return (size_t)__builtin_popcount(bits);
#else // else use custom popcount implementation
    size_t count = 0;
    unsigned current = bits;
    while (current) { // while current is not 0 increment count and remove rightmost bit from current
        count++;
        current ^= current & (~current + 1); // extract and remove rightmost bit
    }

    return count;
#endif
}

void _ibitwise_set_resize(ibitwise_set_s * set, const size_t size) {
    set->capacity = size;

    set->bits = realloc(set->bits, set->capacity / CHAR_BIT);
    assert((!set->capacity || set->bits) && "[ERROR] Memory allocation failed.");
}
