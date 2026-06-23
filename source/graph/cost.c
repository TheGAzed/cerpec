#include <graph/cost.h>

iam_cost_s compose_iam_cost(size_t const size, compare_fn const compare, void * const ac, copy_fn const juggle, void * const aj, copy_fn const sum, void * const as, void * const zero, void * const infinite) {
    error(size && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");
    error(juggle && "Parameter can't be NULL.");
    error(sum && "Parameter can't be NULL.");
    error(zero && "Parameter can't be NULL.");
    error(infinite && "Parameter can't be NULL.");
    error(compare(infinite, zero, ac) > 0 && "Infinite must be greater than zero.");
    error(compare(zero, infinite, ac) < 0 && "Zero must be smaller than infinity.");

    return (iam_cost_s) {
        .compare = compare, .size = size, .zero = zero, .infinite = infinite, .sum = sum, .juggle = juggle,
        .as = as, .aj = aj, .ac = ac,
    };
}
