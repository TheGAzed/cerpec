#include <set/fbitwise_set.h>

#include <assert.h>
#include <string.h>
#include <limits.h>

#define BIT_COUNT (sizeof(unsigned) * CHAR_BIT)

/// @brief Calculates the number of set (one) bits in unsigned integer.
/// @param bits Unsigned integer to count set bits.
/// @return Number of set bits.
int _fbitwise_set_popcount(unsigned const bits);

fbitwise_set_s create_fbitwise_set(size_t const max) {
    assert(max && "[ERROR] Parameter can't be zero.");

    // ceil to closest multiple of CHAR_BIT (usually 8)
    size_t const mod = max % CHAR_BIT;
    size_t const ceil = mod ? max - mod + CHAR_BIT : max;
    size_t const size = ceil / CHAR_BIT;

    // create structure with allocated memory based on bits
    fbitwise_set_s const set = {
        .allocator = &standard, .max = max,
        .bits = standard.alloc(size, standard.arguments),
    };
    assert(set.bits && "[ERROR] Memory allocation failed.");

    memset(set.bits, 0, size);

    return set;
}

fbitwise_set_s make_fbitwise_set(size_t const max, memory_s const * const allocator) {
    assert(max && "[ERROR] Parameter can't be zero.");
    assert(allocator && "[ERROR] Parameter can't be NULL.");

    // ceil to closest multiple of BIT_COUNT (usually 4 * 8 = 32)
    size_t const mod = max % BIT_COUNT;
    size_t const ceil = mod ? max - mod + BIT_COUNT : max;
    size_t const size = ceil / CHAR_BIT;

    // create structure with allocated memory based on bits
    fbitwise_set_s const set = {
        .allocator = allocator, .max = max,
        .bits = allocator->alloc(size, allocator->arguments),
    };
    assert(set.bits && "[ERROR] Memory allocation failed.");

    memset(set.bits, 0, size);

    return set;
}

void destroy_fbitwise_set(fbitwise_set_s * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // free bits array since it only stores indexes
    set->allocator->free(set->bits, set->allocator->arguments);

    set->max = set->length = 0;
    set->allocator = NULL;
    set->bits = NULL;
}

void clear_fbitwise_set(fbitwise_set_s * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // ceil to closest multiple of BIT_COUNT (usually 4 * 8 = 32)
    size_t const mod = set->max % BIT_COUNT;
    size_t const ceil = mod ? set->max - mod + BIT_COUNT : set->max;

    memset(set->bits, 0, ceil / CHAR_BIT);
    set->length = 0;
}

fbitwise_set_s copy_fbitwise_set(fbitwise_set_s const * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // ceil to closest multiple of BIT_COUNT (usually 4 * 8 = 32)
    size_t const mod = set->max % BIT_COUNT;
    size_t const ceil = mod ? set->max - mod + BIT_COUNT : set->max;
    size_t const size = ceil / CHAR_BIT;

    // create replica structure
    const fbitwise_set_s replica = {
        .max = set->max, .length = set->length, .allocator = set->allocator,
        .bits = set->allocator->alloc(size, set->allocator->arguments),
    };
    assert(replica.bits && "[ERROR] Memory allocation failed.");

    // copy initialize bits
    memcpy(replica.bits, set->bits, size);

    return replica;
}

bool is_empty_fbitwise_set(fbitwise_set_s const * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    return !(set->length);
}

bool is_full_fbitwise_set(fbitwise_set_s const * const set) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    return (set->length == set->max);
}

void insert_fbitwise_set(fbitwise_set_s * const set, size_t const index) {
    assert(index < set->max && "[ERROR] Parameter can't exceed maximum.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // calculate bits array index and bit at parameter index
    size_t const idx = index / BIT_COUNT;
    unsigned const relative = (unsigned)(index % BIT_COUNT);
    unsigned const bit = 1U << (BIT_COUNT - relative - 1);

    // assert that bit index is unique (set doesn't contain index)
    assert(!(set->bits[idx] & bit) && "[ERROR] Structure already contains element.");

    // set bit in bits array by ORing
    set->bits[idx] |= bit;
    set->length++;
}

void remove_fbitwise_set(fbitwise_set_s * const set, size_t const index) {
    assert(index < set->max && "[ERROR] Parameter can't exceed maximum.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // calculate bits array index and bit at parameter index
    size_t const idx = index / BIT_COUNT;
    unsigned const relative = (unsigned)(index % BIT_COUNT);
    unsigned const bit = 1U << (BIT_COUNT - relative - 1);

    assert((set->bits[idx] & bit) && "[ERROR] Strucutre already contains element.");

    // remove bit in bits array by XORing
    set->bits[idx] ^= bit;
    set->length--;
}

bool contains_fbitwise_set(fbitwise_set_s const * const set, size_t const index) {
    assert(set && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    // calculate bits array index and bit at parameter index
    size_t const idx = index / BIT_COUNT;
    unsigned const relative = (unsigned)(index % BIT_COUNT);
    unsigned const bit = 1U << (BIT_COUNT - relative - 1);

    return (index < set->max) && (bool)(set->bits[idx] & bit); // return ANDed bit as a boolean
}

fbitwise_set_s union_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");

    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_one->max && "[INVALID] Parameter can't be zero.");

    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_two->max && "[INVALID] Parameter can't be zero.");

    // get smallest and biggest set by capacity
    fbitwise_set_s const * const minimum = set_one->max < set_two->max ? set_one : set_two;
    fbitwise_set_s const * const maximum = set_one->max >= set_two->max ? set_one : set_two;

    // make union as a copy of biggest set and set its length to zero for later popcount
    fbitwise_set_s set_union = copy_fbitwise_set(maximum);
    set_union.length = 0;

    // for each bits in minimum set copy minimum's (OR) into union and calculate new union length
    for (size_t i = 0; i < minimum->max / BIT_COUNT; ++i) {
        set_union.bits[i] |= minimum->bits[i];
        set_union.length += (size_t)_fbitwise_set_popcount(set_union.bits[i]);
    }

    return set_union;
}

fbitwise_set_s intersect_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");

    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_one->max && "[INVALID] Parameter can't be zero.");

    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_two->max && "[INVALID] Parameter can't be zero.");

    fbitwise_set_s const * const minimum = set_one->max < set_two->max ? set_one : set_two;
    fbitwise_set_s const * const maximum = set_one->max >= set_two->max ? set_one : set_two;

    fbitwise_set_s set_intersect = copy_fbitwise_set(minimum);
    set_intersect.length = 0;
    for (size_t i = 0; i < minimum->max / BIT_COUNT; ++i) {
        set_intersect.bits[i] &= maximum->bits[i];
        set_intersect.length += (size_t)_fbitwise_set_popcount(set_intersect.bits[i]);
    }

    return set_intersect;
}

fbitwise_set_s subtract_fbitwise_set(fbitwise_set_s const * const minuend, fbitwise_set_s const * const subtrahend) {
    assert(minuend && "[ERROR] Parameter can't be NULL.");
    assert(subtrahend && "[ERROR] Parameter can't be NULL.");

    assert(minuend->length <= minuend->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(minuend->max && "[INVALID] Parameter can't be zero.");

    assert(subtrahend->length <= subtrahend->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(subtrahend->max && "[INVALID] Parameter can't be zero.");

    // get smallest set based on capacity to minimize operations and invalid array access
    fbitwise_set_s const * const minimum = minuend->max < subtrahend->max ? minuend : subtrahend;

    fbitwise_set_s set_subtract = copy_fbitwise_set(minuend);
    set_subtract.length = 0;
    for (size_t i = 0; i < minimum->max / BIT_COUNT; ++i) {
        set_subtract.bits[i] &= ~(subtrahend->bits[i]);
        set_subtract.length += (size_t)_fbitwise_set_popcount(set_subtract.bits[i]);
    }

    return set_subtract;
}

fbitwise_set_s exclude_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");

    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_one->max && "[INVALID] Parameter can't be zero.");

    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_two->max && "[INVALID] Parameter can't be zero.");

    fbitwise_set_s const * const minimum = set_one->max < set_two->max ? set_one : set_two;
    fbitwise_set_s const * const maximum = set_one->max >= set_two->max ? set_one : set_two;

    fbitwise_set_s set_exclude = copy_fbitwise_set(maximum);
    set_exclude.length = 0;
    for (size_t i = 0; i < minimum->max / BIT_COUNT; ++i) {
        set_exclude.bits[i] ^= minimum->bits[i];
        set_exclude.length += (size_t)_fbitwise_set_popcount(set_exclude.bits[i]);
    }

    return set_exclude;
}

bool is_subset_fbitwise_set(fbitwise_set_s const * const superset, fbitwise_set_s const * const subset) {
    assert(superset && "[ERROR] Parameter can't be NULL.");
    assert(subset && "[ERROR] Parameter can't be NULL.");

    assert(superset->length <= superset->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(superset->max && "[INVALID] Parameter can't be zero.");

    assert(subset->length <= subset->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(subset->max && "[INVALID] Parameter can't be zero.");

    // if ANDed sub- and super-set doesn't equals sub's bits then it isn't a subset
    size_t i = 0;
    for (; i < subset->max / BIT_COUNT && i < superset->max / BIT_COUNT; ++i) {
        if ((subset->bits[i] & superset->bits[i]) != subset->bits[i]) {
            return false;
        }
    }

    // if subset has bits set beyond superset then it also isn't a subset
    for (; i < subset->max / BIT_COUNT; ++i) {
        if (subset->bits[i]) {
            return false;
        }
    }

    return true;
}

bool is_proper_subset_fbitwise_set(fbitwise_set_s const * const superset, fbitwise_set_s const * const subset) {
    assert(superset && "[ERROR] Parameter can't be NULL.");
    assert(subset && "[ERROR] Parameter can't be NULL.");

    assert(superset->length <= superset->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(superset->max && "[INVALID] Parameter can't be zero.");

    assert(subset->length <= subset->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(subset->max && "[INVALID] Parameter can't be zero.");

    // if ANDed sub- and super-set doesn't equals sub's bits then it isn't a subset
    size_t i = 0;
    for (; i < subset->max / BIT_COUNT && i < superset->max / BIT_COUNT; ++i) {
        if ((subset->bits[i] & superset->bits[i]) != subset->bits[i]) {
            return false;
        }
    }

    // if subset has bits set beyond superset then it also isn't a subset
    for (; i < subset->max / BIT_COUNT; ++i) {
        if (subset->bits[i]) {
            return false;
        }
    }

    return (subset->length != superset->length);
}

bool is_disjoint_fbitwise_set(fbitwise_set_s const * const set_one, fbitwise_set_s const * const set_two) {
    assert(set_one && "[ERROR] Parameter can't be NULL.");
    assert(set_two && "[ERROR] Parameter can't be NULL.");

    assert(set_one->length <= set_one->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_one->max && "[INVALID] Parameter can't be zero.");

    assert(set_two->length <= set_two->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set_two->max && "[INVALID] Parameter can't be zero.");

    for (size_t i = 0; i < set_two->max / BIT_COUNT && i < set_one->max / BIT_COUNT; ++i) {
        if (set_two->bits[i] & set_one->bits[i]) {
            return false;
        }
    }

    return true;
}

void map_index_fbitwise_set(fbitwise_set_s const * const set, handle_fn const handle, void * const arguments) {
    assert(set && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(set->length <= set->max && "[INVALID] Lenght can't be larger than maximum.");
    assert(set->max && "[INVALID] Parameter can't be zero.");

    for (size_t i = 0; i < set->max; ++i) {
        size_t temp = i;

        // calculate bits' array index and bit to check
        const size_t idx = i / BIT_COUNT;
        const unsigned relative = ((unsigned)(i) % BIT_COUNT);
        const unsigned bit = 1U << (BIT_COUNT - relative - 1);

        // if bit isn't set continue, and if handle function returns false break the loop and quit main function
        if ((set->bits[idx] & bit) && !handle(&temp, arguments)) {
            break;
        }
    }
}

int _fbitwise_set_popcount(unsigned const bits) {
#if defined(__GNUC__) || defined(__clang__) // if either gcc or clang is used, '__builtin_popcount*' is supported
    return __builtin_popcount(bits);
#else // else use custom popcount implementation
    int count = 0;

    // while bits is not 0 increment count and remove its rightmost bit
    for (unsigned b = bits; b; b ^= b & (~b + 1)) {
        count++;
    }

    return count;
#endif
}
