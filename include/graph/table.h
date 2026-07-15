#ifndef TABLE_H
#define TABLE_H

#include <cerpec.h>

/// @brief Cost value of size that is comparable and is bound to <0, inf>
typedef struct graph_table {
    compare_fn compare;
    copy_fn sum, juggle;
    set_fn destroy;
    size_t size;
    void * infinite, * zero, * aj, * as, * ac, * ad;
    size_t * previous;
    char * costs;
    void const * graph;
} table_s;

/// @brief Composes a structure via its parametric properties.
/// @param size Size of cost.
/// @param compare Compare function pointer to do cost comparison.
/// @param ac Arguments for compare function pointer.
/// @param juggle Juggle (convert) function pointer to convert edge weight into a cost.
/// @param aj Arguments for juggle (convert) function pointer.
/// @param sum Add function pointer to sum two costs.
/// @param as Arguments for sum function pointer.
/// @param destroy
/// @param ad
/// @param zero A cost of zero representing an instantly reachable cost.
/// @param infinite An infinte cost representing an impossiblely reachable cost.
/// @return Cost structure.
table_s compose_table(
    size_t const size, compare_fn const compare, void * const ac, copy_fn const juggle, void * const aj,
    copy_fn const sum, void * const as, set_fn const destroy, void * const ad, void * const zero, void * const infinite);

#endif // TABLE_H
