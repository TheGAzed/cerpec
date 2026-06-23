#ifndef COST_H
#define COST_H

#include <cerpec.h>

/// @brief Cost value of size that is comparable and is bound to <0, inf>
typedef struct infinite_adjacency_matrix_graph_cost {
    compare_fn compare;
    copy_fn sum, juggle;
    size_t size;
    void * infinite, * zero, * aj, * as, * ac;
} iam_cost_s;

/// @brief Composes a structure via its parametric properties.
/// @param size Size of cost.
/// @param compare Compare function pointer to do costs comparison.
/// @param ac Arguments for compare function pointer.
/// @param juggle Convert function pointer to convert an edge (weight) into a cost.
/// @param aj Arguments for juggle convert function pointer.
/// @param sum Add function pointer to sum two cost.
/// @param as Arguments for sum function pointer.
/// @param zero A cost of zero representing an instantly reachable cost.
/// @param infinite An infinte cost representing an impossiblely reachable cost.
/// @return Cost structure.
iam_cost_s compose_iam_cost(size_t const size, compare_fn const compare, void * ac, copy_fn const juggle, void * const aj, copy_fn const sum, void * as, void * const zero, void * const infinite);

#endif //COST_H
