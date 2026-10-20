#include <graph/table.h>

table_s compose_table(size_t const size, copy_fn const convert, void * const ac, void * const zero, void * const infinite) {
    error(size && "Parameter can't be zero.");
    error(zero && "Parameter can't be NULL.");
    error(infinite && "Parameter can't be NULL.");

    return (table_s) { .size = size, .zero = zero, .infinite = infinite, .convert = convert, .ac = ac, };
}
