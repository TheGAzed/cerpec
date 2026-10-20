#ifndef TABLE_H
#define TABLE_H

#include <cerpec.h>

/// @brief Cost value of size that is comparable and is bound to <0, inf>
typedef struct graph_table {
    copy_fn convert;
    size_t size;
    void * infinite, * zero, * ac;
    size_t * previous;
    char * costs;
    void const * graph;
} table_s;

/// @brief Composes a structure via its parametric properties.
/// @param size Size of cost.
/// @param convert Convert function pointer to convert edge weight into a cost.
/// @param ac Arguments for juggle (convert) function pointer.
/// @param zero A cost of zero representing an instantly reachable cost.
/// @param infinite An infinte cost representing an impossiblely reachable cost.
/// @return Cost structure.
table_s compose_table(size_t const size, copy_fn const convert, void * const ac, void * const zero, void * const infinite);

#endif // TABLE_H
