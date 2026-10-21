#include <string.h>

#include <graph/iam_graph_new.h>

#include <sequence/fstack.h>
#include <set/fbitwise_set.h>
#include <dodac.h>
#include <sequence/fqueue.h>

struct visited_list_array {
    fbitwise_set_s set;
    size_t head, * next, * prev;
};

/// @brief Resize function for graph.
/// @param graph Graph to resize.
/// @param size New size to resize into.
void _iam_graph_resize(iam_graph_s * const graph, size_t const size);

void _initialize_table(table_s * const table, iam_graph_s const * const graph, void const * const infinite, bool const is_self);

/// @brief Find parent helper funtion for union set function used in Kruskal's MST algorithm.
/// @param table Kruskal table structure to for parent.
/// @param vertex Index of child vertex to search parent.
/// @return Parent vertex index.
/// @note  Also known as find-set in disjoint sets.
size_t _find_parent(table_s const * const table, size_t const vertex);

/// @brief Union set function to union two disjoint sets commonly used in Kruskal's MST algorithm.
/// @param table Table of vertices with costs/ranks.
/// @param source Source vertex index to union.
/// @param destination Destination vertex idnex to union.
/// @param increment Increment function pointer to increment source parent's rank.
/// @param ai Increment function pointer arguments.
/// @param compare
/// @param ac
void _union_set(table_s const * const table, size_t const source, size_t const destination, set_fn const increment, void * const ai, compare_fn
                const compare, void * const ac);

void _set_visited(struct visited_list_array * const visited, size_t const index);

iam_graph_s _create_iam_graph(size_t const vertex_size, size_t const edge_size) {
    error(vertex_size && "Parameter can't be zero.");
    error(edge_size && "Parameter can't be zero.");

    return (iam_graph_s) {
        .vertex_size = vertex_size, .edge_size = edge_size, .allocator = &standard,
    };
}

iam_graph_s _make_iam_graph(size_t const vertex_size, size_t const edge_size, memory_s const * const allocator) {
    error(vertex_size && "Parameter can't be zero.");
    error(edge_size && "Parameter can't be zero.");

    return (iam_graph_s) {
        .vertex_size = vertex_size, .edge_size = edge_size, .allocator = allocator,
    };
}

void _destroy_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, void * const adv,
    set_fn const destroy_edge, void * const ade) {
    error(graph && "Parameter can't be NULL.");
    error(destroy_vertex && "Parameter can't be NULL.");
    error(destroy_edge && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size), adv);
    }

    for (size_t i = 0; i < graph->edge_length; ++i) {
        destroy_edge(graph->edges + (i * graph->edge_size), ade);
    }

    graph->allocator->free(graph->vertices, graph->allocator->arg);
    graph->allocator->free(graph->edges, graph->allocator->arg);
    graph->allocator->free(graph->sparse, graph->allocator->arg);
    graph->allocator->free(graph->dense, graph->allocator->arg);
    graph->allocator->free(graph->degrees, graph->allocator->arg);

    memset(graph, 0, sizeof(iam_graph_s));
}

void _clear_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, void * const adv,
    set_fn const destroy_edge, void * const ade) {
    error(graph && "Parameter can't be NULL.");
    error(destroy_vertex && "Parameter can't be NULL.");
    error(destroy_edge && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size), adv);
    }

    for (size_t i = 0; i < graph->edge_length; ++i) {
        destroy_edge(graph->edges + (i * graph->edge_size), ade);
    }

    graph->allocator->free(graph->vertices, graph->allocator->arg);
    graph->allocator->free(graph->edges, graph->allocator->arg);
    graph->allocator->free(graph->sparse, graph->allocator->arg);
    graph->allocator->free(graph->dense, graph->allocator->arg);
    graph->allocator->free(graph->degrees, graph->allocator->arg);

    graph->capacity = graph->vertex_length = 0;
    graph->vertices = graph->edges = NULL;
}

iam_graph_s _copy_iam_graph(iam_graph_s const * const graph, copy_fn const copy_vertex, void * const acv, copy_fn const copy_edge, void * const ace) {
    error(graph && "Parameter can't be NULL.");
    error(copy_vertex && "Parameter can't be NULL.");
    error(copy_edge && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    iam_graph_s const replica = {
        .capacity = graph->capacity,
        .vertex_size = graph->vertex_size, .edge_size = graph->edge_size,
        .edge_length = graph->edge_length, .vertex_length = graph->vertex_length,

        .edges = graph->allocator->alloc(edge_capacity * graph->edge_size, graph->allocator->arg),
        .sparse = graph->allocator->alloc(edge_capacity * sizeof(size_t), graph->allocator->arg),
        .dense = graph->allocator->alloc(edge_capacity * sizeof(size_t), graph->allocator->arg),

        .vertices = graph->allocator->alloc(graph->capacity * graph->vertex_size, graph->allocator->arg),
        .degrees = graph->allocator->alloc(graph->capacity * sizeof(size_t), graph->allocator->arg),

        .allocator = graph->allocator,
    };
    error((!replica.capacity || replica.vertices) && "Memory allocation failed.");
    error((!replica.capacity || replica.dense) && "Memory allocation failed.");
    error((!edge_capacity || replica.edges) && "Memory allocation failed.");

    // memory copy degrees
    memcpy(replica.degrees, graph->degrees, replica.capacity * sizeof(size_t));
    // function copy each vertex
    for (size_t i = 0; i < graph->vertex_length; ++i) {
        copy_vertex(replica.vertices + (i * replica.vertex_size), graph->vertices + (i * graph->vertex_size), acv);
    }

    size_t const max_edge_length = (graph->vertex_length * (graph->vertex_length - 1)) / 2;
    memcpy(replica.sparse, graph->sparse, max_edge_length * sizeof(size_t));
    memcpy(replica.dense, graph->dense, replica.edge_length * sizeof(size_t));
    for (size_t i = 0; i < replica.edge_length; ++i) {
        void const * edge = graph->edges + (i * graph->edge_size);
        copy_vertex(replica.edges + (i * replica.edge_size), edge, ace);
    }

    return replica;
}

bool _is_empty_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    return !(graph->vertex_length);
}

bool _is_complete_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    return graph->edge_length == ((graph->vertex_length * (graph->vertex_length - 1)) / 2);
}

bool _is_connected_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // early return since an empty (null) graph is NOT connected
    // also avoid unnecessary allocation and invalid memory access
    if (!graph->vertex_length) { return false; }

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);
    fstack_s stack = create_fstack(sizeof(size_t), graph->vertex_length);

    size_t v = 0;
    push_fstack(&stack, &v);

    while (!is_empty_fstack(&stack)) {
        pop_fstack(&stack, &v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t i = 0; i < graph->vertex_length - 1; i++) {
            size_t const i_off = (i * (i + 1)) / 2;

            size_t const u = (i < v) ? i : i + 1;
            size_t const e = v <= i ? i_off + v : v_off + i;

            if (IAM_GRAPH_SPECIAL != graph->sparse[e] && !contains_fbitwise_set(&visited, u)) {
                insert_fbitwise_set(&visited, u);
                push_fstack(&stack, &u);
            }
        }
    }

    size_t const visited_count = visited.length;
    destroy_fbitwise_set(&visited);
    destroy_fstack(&stack, voiddst, NULL);

    return graph->vertex_length == visited_count;
}

bool _is_tree_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // early return since an empty graph is NOT a tree
    // if the edge count isn't equal to vertex count - 1 then it also isn't a tree
    // also avoid unnecessary allocation and invalid memory access
    if (!graph->vertex_length || graph->edge_length != graph->vertex_length - 1) {
        return false;
    }

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);

    struct family { size_t parent, child; } pair = { .parent = IAM_GRAPH_SPECIAL, .child = 0, };
    fstack_s stack = create_fstack(sizeof(struct family), graph->vertex_length);

    push_fstack(&stack, &pair);

    bool contains_cycle = false;
    while (!is_empty_fstack(&stack)) {
        pop_fstack(&stack, &pair);
        size_t const v = pair.child;

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t i = 0; i < graph->vertex_length - 1; i++) {
            size_t const i_off = (i * (i + 1)) / 2;

            size_t const u = (i < v) ? i : i + 1;
            size_t const e = v <= i ? i_off + v : v_off + i;

            if (IAM_GRAPH_SPECIAL == graph->sparse[e]) { continue; }

            // if we are revisiting a vertex and that vertex isn't the parent of
            if (!contains_fbitwise_set(&visited, u)) {
                insert_fbitwise_set(&visited, u);

                pair = (struct family) { .parent = v, .child = u };
                push_fstack(&stack, &pair);
            } else if (pair.parent != u) {
                contains_cycle = true;
                goto CLEANUP;
            }
        }
    }

CLEANUP:
    size_t const visited_count = visited.length;
    destroy_fbitwise_set(&visited);
    destroy_fstack(&stack, voiddst, NULL);

    // check if no cycles were detected and if all vertices were visited (graph is connected)
    return !contains_cycle && (graph->vertex_length == visited_count); // check with count to detect unvisited
}

bool _is_cyclic_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // early return, because a graph with 0/1/2 vertices can't have circles
    if (graph->vertex_length < 3) {
        return false;
    }

    // initialize visited list array set for best access, removal and unvisited non-connected graphs
    struct visited_list_array visited = {
        .set = create_fbitwise_set(graph->vertex_length),
        .next = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arg),
        .prev = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arg),
    };
    error(visited.next && "Memory allocation failed.");
    error(visited.prev && "Memory allocation failed.");

    // initialize visited list array to circualr doubly linked list
    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited.next[i] = i + 1;
        visited.prev[i] = i - 1;
    }
    // since vertex length is not zero (thanks to early return), no underflow array access should occur
    visited.next[graph->vertex_length - 1] = 0;
    visited.prev[0] = graph->vertex_length - 1;

    struct family { size_t parent, child; } pair = { .parent = IAM_GRAPH_SPECIAL, .child = 0, };
    fstack_s stack = create_fstack(sizeof(struct family), graph->vertex_length);

    bool contains_cycle = false;
    while (graph->vertex_length - visited.set.length) {
        // get unvisited head vertex in case of non-connected graph, set it to visited and add it to the stack
        size_t const current = visited.head;
        push_fstack(&stack, &pair);
        _set_visited(&visited, current);

        // loop until stack is empty
        // end program if cycle was found by goto jumping to cleanup
        while (stack.length) {
            // get visited vertex from child pair
            pop_fstack(&stack, &pair);
            size_t const v = pair.child;

            // while each neighbor of v is iterated
            size_t const v_off = (v * (v - 1)) / 2;
            for (size_t i = 0; i < graph->vertex_length - 1; i++) {
                size_t const i_off = (i * (i + 1)) / 2;

                // get neighbor index and edge weight array position
                size_t const u = (i < v) ? i : i + 1;
                size_t const e = v <= i ? i_off + v : v_off + i;

                // get edge weight and check if it is valid
                if (IAM_GRAPH_SPECIAL == graph->sparse[e]) { continue; }

                // if vertex u wasn't visited, then visit it and add pair to stack
                // else if neighbor isn't parent, then cycle was found (leave nested loops)
                if (!contains_fbitwise_set(&visited.set, u)) {
                    _set_visited(&visited, u);
                    pair = (struct family) { .parent = v, .child = u };
                    push_fstack(&stack, &pair);
                } else if (pair.parent != u) {
                    contains_cycle = true;
                    goto CLEANUP;
                }
            }
        }
    }

CLEANUP:
    destroy_fstack(&stack, voiddst, NULL);

    destroy_fbitwise_set(&visited.set);
    graph->allocator->free(visited.next, graph->allocator->arg);
    graph->allocator->free(visited.prev, graph->allocator->arg);

    return contains_cycle;
}

size_t _insert_vertex_iam_graph(iam_graph_s * const graph, void const * const vertex) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    if (graph->vertex_length == graph->capacity) {
        size_t const capacity = graph->vertex_length ? graph->vertex_length * CERPEC_FACTOR : IAM_GRAPH_CHUNK;
        _iam_graph_resize(graph, capacity);
    }

    graph->degrees[graph->vertex_length] = 0;
    memcpy(graph->vertices + (graph->vertex_length * graph->vertex_size), vertex, graph->vertex_size);

    return graph->vertex_length++;
}

size_t _remove_vertex_iam_graph(iam_graph_s * const graph, size_t const index, void * const buffer, set_fn const destroy_edge, void * const ade) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // last vertex's first matrix edge
    size_t const start_last = ((graph->vertex_length - 1) * (graph->vertex_length - 2)) / 2;

    // destroy removed vertex' row edges and replace them with last's edges
    size_t const start_row = (index * (index - 1)) / 2; // index vertex's first row edge
    for (size_t r = 0; r < index; r++) {
        // calculate removed and last vertex' edges
        // if an edge exists for removed vertex then destroy it
        size_t const dense_index = graph->sparse[start_row + r];
        if (IAM_GRAPH_SPECIAL != dense_index) {
            void * edge_index = graph->edges + (dense_index * graph->edge_size);
            destroy_edge(edge_index, ade);
            graph->degrees[r]--;
            graph->edge_length--;

            // sparse set remove edge
            graph->dense[dense_index] = graph->dense[graph->edge_length];
            graph->sparse[graph->dense[graph->edge_length]] = dense_index;
            graph->sparse[start_row + r] = IAM_GRAPH_SPECIAL;

            memmove(edge_index, graph->edges + (graph->edge_length * graph->edge_size), graph->edge_size);
        }
        // sparse set move edge to removed vertex proper edge position
        size_t const _dense_index = graph->sparse[start_row + r] = graph->sparse[start_last + r];
        if (IAM_GRAPH_SPECIAL != _dense_index) {
            graph->dense[_dense_index] = start_last + r;
        }
    }

    // destroy removed vertex' column edges and replace them with last's edges (except the one shared with last)
    for (size_t c = index, start_col = start_row + c; c < graph->vertex_length - 1; c++, start_col += c) {
        // calculate removed and last vertex' edges
        size_t const dense_index = graph->sparse[start_col + c];

        // if an edge exists for removed vertex then destroy it
        if (IAM_GRAPH_SPECIAL != dense_index) {
            void * edge_index = graph->edges + (dense_index * graph->edge_size);
            destroy_edge(edge_index, ade);
            graph->degrees[c]--;
            graph->edge_length--;

            graph->dense[dense_index] = graph->dense[graph->edge_length];
            graph->sparse[graph->dense[graph->edge_length]] = dense_index;
            graph->sparse[start_col + c] = IAM_GRAPH_SPECIAL;

            memmove(edge_index, graph->edges + (graph->edge_length * graph->edge_size), graph->edge_size);
        }
        // sparse set move edge to removed vertex proper edge position
        size_t const _dense_index = graph->sparse[start_col + c] = graph->sparse[start_last + c];
        if (IAM_GRAPH_SPECIAL != _dense_index) {
            graph->dense[_dense_index] = start_last + c;
        }
    }

    size_t const dense_index = graph->sparse[start_last + index];
    // remove edge shared with last vertex in array if removed vertex isn't last and an edge exists
    if (index != graph->vertex_length - 1 && IAM_GRAPH_SPECIAL != dense_index) {
        destroy_edge(graph->edges + (dense_index * graph->edge_size), ade);
    }

    // save removed index into buffer and move last vertex into removed's position
    graph->vertex_length--;
    void const * last = graph->vertices + (graph->vertex_length * graph->vertex_size);
    void * removed = graph->vertices + (index * graph->vertex_size);
    memcpy(buffer, removed, graph->vertex_size);
    graph->degrees[index] = graph->degrees[graph->vertex_length]; // move last vertex' degree to removed index
    memmove(removed, last, graph->vertex_size); // memmove since removed could be last

    // shrink graph if elements fit into smaller memory chunk
    if (graph->vertex_length <= graph->capacity / CERPEC_FACTOR &&
        (graph->vertex_length > IAM_GRAPH_CHUNK || !graph->vertex_length)) {
        _iam_graph_resize(graph, graph->vertex_length);
    }

    // if last index was removed return an invalid index, else return the last vertex' index before it was moved
    return graph->vertex_length;
}

void _get_vertex_iam_graph(iam_graph_s const * const graph, size_t const index, void * const buffer) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    memcpy(buffer, graph->vertices + (index * graph->vertex_size), graph->vertex_size);
}

void _insert_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void const * const weight) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // reflect edge insertion in vertex one's and two's degrees
    graph->degrees[index_one]++;
    graph->degrees[index_two]++;

    // get smallest and largest vertex index for edge insertion calculations
    size_t const minimum = index_one < index_two ? index_one : index_two;
    size_t const maximum = index_one >= index_two ? index_one : index_two;

    // calculate start row in flattened 2D array
    size_t const start_row = (maximum * (maximum - 1)) / 2;

    size_t const dense_index = graph->sparse[start_row + minimum];
    error(IAM_GRAPH_SPECIAL == dense_index);

    graph->sparse[start_row + minimum] = graph->edge_length;
    graph->dense[graph->edge_length] = start_row + minimum;
    memcpy(graph->edges + (graph->edge_length * graph->edge_size), weight, graph->edge_size);
    graph->edge_length++;
}

void _remove_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void * const buffer) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // reflect edge removal in vertex one's and two's degrees
    graph->degrees[index_one]--;
    graph->degrees[index_two]--;

    // get smallest and largest vertex index for edge removal calculations
    size_t const minimum = index_one < index_two ? index_one : index_two;
    size_t const maximum = index_one >= index_two ? index_one : index_two;

    // calculate start row in flattened 2D array
    size_t const start_row = (maximum * (maximum - 1)) / 2;

    size_t const dense_index = graph->sparse[start_row + minimum];
    error(IAM_GRAPH_SPECIAL != dense_index);
    memcpy(buffer, graph->edges + (dense_index * graph->edge_size), graph->edge_size);
    graph->edge_length--;

    graph->dense[dense_index] = graph->dense[graph->edge_length];
    graph->sparse[graph->dense[graph->edge_length]] = dense_index;
    graph->sparse[start_row + minimum] = IAM_GRAPH_SPECIAL;
}

bool _contains_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const minimum = index_one < index_two ? index_one : index_two;
    size_t const maximum = index_one >= index_two ? index_one : index_two;

    size_t const start_row = (maximum * (maximum - 1)) / 2;
    size_t const dense_index = graph->sparse[start_row + minimum];

    return (IAM_GRAPH_SPECIAL != dense_index);
}

size_t _get_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two, void * const buffer) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const minimum = index_one < index_two ? index_one : index_two;
    size_t const maximum = index_one >= index_two ? index_one : index_two;

    size_t const start_row = (maximum * (maximum - 1)) / 2;

    size_t const dense_index = graph->sparse[start_row + minimum];
    error(IAM_GRAPH_SPECIAL != dense_index);

    memcpy(buffer, graph->edges + (dense_index * graph->edge_size), graph->edge_size);
    return dense_index;
}

size_t _degree_iam_graph(iam_graph_s const * const graph, size_t const index) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    return graph->degrees[index];
}

void _bfs_iam_graph(iam_graph_s const * const graph, table_s * const table, size_t const start, size_t const end) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_GRAPH_SPECIAL) && "Parameter can't exceed length.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);
    fqueue_s queue = create_fqueue(sizeof(size_t), graph->vertex_length);

    _initialize_table(table, graph, table->infinite, false);

    size_t v = start;
    insert_fbitwise_set(&visited, v);
    enqueue_fqueue(&queue, &v);
    memcpy(table->costs + (table->size * v), table->zero, table->size);

    while (!is_empty_fqueue(&queue) && end != v) {
        dequeue_fqueue(&queue, &v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t i = 0; i < graph->vertex_length - 1; i++) {
            size_t const i_off = (i * (i + 1)) / 2;

            size_t const u = (i < v) ? i : i + 1;
            size_t const e = v <= i ? i_off + v : v_off + i;

            size_t const dense_index = graph->sparse[e];
            if (IAM_GRAPH_SPECIAL != dense_index && !contains_fbitwise_set(&visited, u)) {
                insert_fbitwise_set(&visited, u);
                enqueue_fqueue(&queue, &u);

                table->previous[u] = v;
                void const * edge = graph->edges + (dense_index * graph->edge_size);
                table->convert(table->costs + (u * table->size), edge, table->ac);
            }
        }
    }

    destroy_fbitwise_set(&visited);
    destroy_fqueue(&queue, voiddst, NULL);
}

void _dfs_iam_graph(iam_graph_s const * const graph, table_s * const table, size_t const start, size_t const end) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_GRAPH_SPECIAL) && "Parameter can't exceed length.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);
    fstack_s stack = create_fstack(sizeof(size_t), graph->vertex_length);

    _initialize_table(table, graph, table->infinite, false);

    size_t v = start;
    insert_fbitwise_set(&visited, v);
    push_fstack(&stack, &v);
    memcpy(table->costs + (table->size * v), table->zero, table->size);

    while (!is_empty_fstack(&stack) && end != v) {
        pop_fstack(&stack, &v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t i = 0; i < graph->vertex_length - 1; i++) {
            size_t const i_off = (i * (i + 1)) / 2;

            size_t const u = (i < v) ? i : i + 1;
            size_t const e = v <= i ? i_off + v : v_off + i;

            size_t const dense_index = graph->sparse[e];
            if (IAM_GRAPH_SPECIAL != dense_index && !contains_fbitwise_set(&visited, u)) {
                insert_fbitwise_set(&visited, u);
                push_fstack(&stack, &u);

                table->previous[u] = v;
                void const * edge = graph->edges + (dense_index * graph->edge_size);
                table->convert(table->costs + (u * table->size), edge, table->ac);
            }
        }
    }

    destroy_fbitwise_set(&visited);
    destroy_fstack(&stack, voiddst, NULL);
}

void _dijkstra_iam_graph(iam_graph_s const * const graph, table_s * const table, size_t const start, size_t const end, compare_fn const compare, void * const ac, copy_fn const sum, void * const as) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_GRAPH_SPECIAL) && "Parameter can't exceed length.");
    error(compare && "Parameter can't be NULL.");
    error(sum && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);

    _initialize_table(table, graph, table->infinite, false);
    memcpy(table->costs + (table->size * start), table->zero, table->size);

    // allocate for both minimum and sum
    char * buffer = graph->allocator->alloc(2 * table->size, graph->allocator->arg);
    error(buffer && "Memory allocation failed.");

    // initialize current minimum vertex
    struct iam_graph_smallest { size_t vertex; void * cost; } minimum = { .vertex = start, .cost = buffer, };
    memcpy(minimum.cost, table->zero, table->size);

    char * s_cost = buffer + table->size;
    for (size_t i = 0; i < graph->vertex_length - 1 && end != minimum.vertex && IAM_GRAPH_SPECIAL != minimum.vertex; ++i) {
        size_t const v = minimum.vertex;
        void const * v_cost = table->costs + (minimum.vertex * table->size);

        minimum.vertex = IAM_GRAPH_SPECIAL;
        memcpy(minimum.cost, table->infinite, table->size);

        insert_fbitwise_set(&visited, v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t j = 0; j < graph->vertex_length - 1; j++) {
            size_t const i_off = (j * (j + 1)) / 2;

            size_t const u = (j < v) ? j : j + 1;
            size_t const e = v <= j ? i_off + v : v_off + j;

            if (contains_fbitwise_set(&visited, u)) { continue; }

            // save edges for access
            void * g_cost = table->costs + (u * table->size);

            size_t const dense_index = graph->sparse[e];
            if (IAM_GRAPH_SPECIAL != dense_index) {
                void const * edge = graph->edges + (dense_index * graph->edge_size);
                sum(table->convert(s_cost, edge, ac), v_cost, as);
            } else {
                memcpy(s_cost, table->infinite, table->size);
            }

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (compare(s_cost, g_cost, ac) < 0) {
                table->previous[u] = v;
                memcpy(g_cost, s_cost, table->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (compare(minimum.cost, g_cost, ac) > 0) {
                memcpy(minimum.cost, g_cost, table->size);
                minimum.vertex = u;
            }
        }
    }

    destroy_fbitwise_set(&visited);
    graph->allocator->free(buffer, graph->allocator->arg);
}

void _a_star_iam_graph(iam_graph_s const * const graph, table_s * const table, size_t const start, size_t const end, compare_fn const compare, void * const ac, copy_fn const sum, void * const as, operate_fn const heuristic, void * const ah) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_GRAPH_SPECIAL) && "Parameter can't exceed length.");
    error(compare && "Parameter can't be NULL.");
    error(sum && "Parameter can't be NULL.");
    error(heuristic && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);

    _initialize_table(table, graph, table->infinite, false);
    memcpy(table->costs + (table->size * start), table->zero, table->size);

    // allocate for all costs to later free at once
    char * buffer = graph->allocator->alloc(4 * table->size, graph->allocator->arg);
    error(buffer && "Memory allocation failed.");

    // initialize current minimum vertex
    struct iam_graph_smallest { size_t vertex; void * cost; } minimum = { .vertex = start, .cost = buffer, };
    memcpy(minimum.cost, table->zero, table->size);

    char * s_cost = buffer + table->size;
    char * h_cost = buffer + (2 * table->size);
    char * f_cost = buffer + (3 * table->size);
    for (size_t i = 0; i < graph->vertex_length - 1 && end != minimum.vertex && IAM_GRAPH_SPECIAL != minimum.vertex; ++i) {
        size_t const v = minimum.vertex;
        void const * v_cost = table->costs + (minimum.vertex * table->size);

        minimum.vertex = IAM_GRAPH_SPECIAL;
        memcpy(minimum.cost, table->infinite, table->size);

        insert_fbitwise_set(&visited, v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t j = 0; j < graph->vertex_length - 1; j++) {
            size_t const i_off = (j * (j + 1)) / 2;

            size_t const u = (j < v) ? j : j + 1;
            size_t const e = v <= j ? i_off + v : v_off + j;

            if (contains_fbitwise_set(&visited, u)) { continue; }

            // save edges for access
            void * g_cost = table->costs + (u * table->size);

            size_t const dense_index = graph->sparse[e];
            if (IAM_GRAPH_SPECIAL != dense_index) {
                void const * edge = graph->edges + (dense_index * graph->edge_size);
                sum(table->convert(s_cost, edge, ac), v_cost, as);
            } else {
                memcpy(s_cost, table->infinite, table->size);
            }

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (compare(s_cost, g_cost, ac) < 0) {
                table->previous[u] = v;
                memcpy(g_cost, s_cost, table->size);
            }

            // after vertex cost was updated determine F(n) by adding updated G(n) {as vertex cost} and H(n) together
            heuristic(h_cost, graph->vertices + (v * graph->vertex_size), graph->vertices + (end * graph->vertex_size), ah);
            // neither g_cost nor h_cost is infinite (used to prevent potential overflow while summing)
            bool const can_sum = compare(g_cost, table->infinite, ac) && compare(h_cost, table->infinite, ac);
            memcpy(f_cost, can_sum ? sum(h_cost, g_cost, as) : table->infinite, table->size);

            // if F(n)'s cost is smaller than the current minimum cost then set minimum cost to F(n)'s
            if (compare(minimum.cost, f_cost, ac) > 0) {
                memcpy(minimum.cost, f_cost, table->size);
                minimum.vertex = u;
            }
        }
    }

    destroy_fbitwise_set(&visited);
    graph->allocator->free(buffer, graph->allocator->arg);
}

void _prim_iam_graph(iam_graph_s const * const graph, table_s * const table, size_t const start, compare_fn const compare, void * const ac) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error(compare && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    fbitwise_set_s visited = create_fbitwise_set(graph->vertex_length);

    _initialize_table(table, graph, table->infinite, false);
    memcpy(table->costs + (table->size * start), table->zero, table->size);

    // allocate for both minimum and sum
    char * buffer = graph->allocator->alloc(2 * table->size, graph->allocator->arg);
    error(buffer && "Memory allocation failed.");

    // initialize current minimum vertex
    struct iam_graph_smallest { size_t vertex; void * cost; } minimum = { .vertex = start, .cost = buffer, };
    memcpy(minimum.cost, table->zero, table->size);

    char * w_cost = buffer + table->size;
    for (size_t i = 0; i < graph->vertex_length - 1 && IAM_GRAPH_SPECIAL != minimum.vertex; ++i) {
        size_t const v = minimum.vertex;

        minimum.vertex = IAM_GRAPH_SPECIAL;
        memcpy(minimum.cost, table->infinite, table->size);

        insert_fbitwise_set(&visited, v);

        size_t const v_off = (v * (v - 1)) / 2;
        for (size_t j = 0; j < graph->vertex_length - 1; j++) {
            size_t const i_off = (j * (j + 1)) / 2;

            size_t const u = (j < v) ? j : j + 1;
            size_t const e = v <= j ? i_off + v : v_off + j;

            if (contains_fbitwise_set(&visited, u)) { continue; }

            // save edges for access
            // save edges for access
            void * g_cost = table->costs + (u * table->size);

            size_t const dense_index = graph->sparse[e];
            if (IAM_GRAPH_SPECIAL != dense_index) {
                void const * edge = graph->edges + (dense_index * graph->edge_size);
                table->convert(w_cost, edge, ac);
            } else {
                memcpy(w_cost, table->infinite, table->size);
            }

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (compare(w_cost, g_cost, ac) < 0) {
                table->previous[u] = v;
                memcpy(g_cost, w_cost, table->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (compare(minimum.cost, g_cost, ac) > 0) {
                memcpy(minimum.cost, g_cost, table->size);
                minimum.vertex = u;
            }
        }
    }

    destroy_fbitwise_set(&visited);
    graph->allocator->free(buffer, graph->allocator->arg);
}

void _kruskal_iam_graph(iam_graph_s const * const graph, table_s * const table, process_fn const sort, void * const as, set_fn const increment, void * const ai, compare_fn const compare, void * const ac) {
    error(graph && "Parameter can't be NULL.");
    error(table && "Parameter can't be NULL.");
    error(sort && "Parameter can't be NULL.");
    error(increment && "Parameter can't be NULL.");

    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    _initialize_table(table, graph, table->zero, true);

    // initialize edge array defined by the edge itself and vertices making it up
    iam_edge_s * kruskal_edges = graph->allocator->alloc(graph->edge_length * sizeof(iam_edge_s), graph->allocator->arg);
    error((!graph->edge_length || kruskal_edges) && "Memory allocation failed.");

    for (size_t i = 0, index = 0; i < graph->vertex_length; ++i) {
        for (size_t j = i + 1; j < graph->vertex_length; ++j) {
            size_t const e = ((j * (j - 1)) / 2) + i;
            size_t const dense_index = graph->sparse[e];

            if (IAM_GRAPH_SPECIAL != dense_index) {
                void * edge = graph->edges + (dense_index * graph->edge_size);
                kruskal_edges[index++] = (iam_edge_s) { .vertices = { i, j, }, .weight = edge, };
            }
        }
    }

    // sort edge array based on its edge (weight)
    sort(kruskal_edges, graph->edge_length, as);

    // Kruskal's algorithm with sorted edges
    for (int i = 0; i < graph->edge_length; ++i) {
        iam_edge_s const smallest = kruskal_edges[i]; // get the next smallest edge from sorted array of edges

        size_t const source      = _find_parent(table, smallest.vertices[0]);
        size_t const destination = _find_parent(table, smallest.vertices[1]);

        if (source != destination) {
            _union_set(table, source, destination, increment, ai, compare, ac);
        }
    }
}

void _destroy_iam_table(table_s * const table) {
    error(table && "Parameter can't be NULL.");

    iam_graph_s const * graph = table->graph;

    valid(graph && "Table graph can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    graph->allocator->free(table->costs, graph->allocator->arg);
    graph->allocator->free(table->previous, graph->allocator->arg);

    memset(table, 0, sizeof(table_s));
}

iam_graph_s _subgraph_iam_graph(table_s const * const table, copy_fn const copy_vertex, void * const acv, copy_fn const copy_edge, void * const ace, compare_fn const compare, void * const ac) {
    error(table && "Parameter can't be NULL.");
    error(copy_vertex && "Parameter can't be NULL.");
    error(copy_edge && "Parameter can't be NULL.");

    iam_graph_s const * graph = table->graph;

    valid(graph && "Table graph can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    iam_graph_s const subgraph = {
        .capacity = graph->capacity, .allocator = graph->allocator,
        .vertex_size = graph->vertex_size, .edge_size = graph->edge_size,
        .edge_length = graph->edge_length, .vertex_length = graph->vertex_length,

        .edges = graph->allocator->alloc(edge_capacity * graph->edge_size, graph->allocator->arg),
        .sparse = graph->allocator->alloc(edge_capacity * sizeof(size_t), graph->allocator->arg),
        .dense = graph->allocator->alloc(edge_capacity * sizeof(size_t), graph->allocator->arg),

        .vertices = graph->allocator->alloc(graph->capacity * graph->vertex_size, graph->allocator->arg),
        .degrees = graph->allocator->alloc(graph->capacity * sizeof(size_t), graph->allocator->arg),
    };
    error((!subgraph.capacity || subgraph.vertices) && "Memory allocation failed.");
    error((!subgraph.capacity || subgraph.dense) && "Memory allocation failed.");
    error((!edge_capacity || subgraph.edges) && "Memory allocation failed.");

    // create vertex copies and subgraph edge copies based on existing cost
    for (size_t i = 0; i < graph->vertex_length; ++i) {
        copy_vertex(subgraph.vertices + (i * subgraph.vertex_size), graph->vertices + (i * graph->vertex_size), acv);

        char const * cost = table->costs + (table->size * i);

        void const * zero = table->zero, * infinite = table->infinite;
        if (compare(cost, zero, ac) && compare(cost, infinite, ac)) {
            size_t const index_one = i;
            size_t const index_two = table->previous[i];

            size_t const minimum = index_one < index_two ? index_one : index_two;
            size_t const maximum = index_one >= index_two ? index_one : index_two;

            size_t const start_row = (maximum * (maximum - 1)) / 2;

            size_t const dense_index = graph->sparse[start_row + minimum];
            error(IAM_GRAPH_SPECIAL != dense_index);

            char * subgraph_edge = subgraph.edges + (dense_index * subgraph.edge_size);
            char const * graph_edge = graph->edges + (dense_index * graph->edge_size);
            copy_edge(subgraph_edge, graph_edge, ace);
        }
    }

    return subgraph;
}

void _each_vertex_iam_graph(iam_graph_s const * const graph, manage_fn const manage, void * const am) {
    error(graph && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const s = graph->vertex_size;
    for (char * v = graph->vertices; v < graph->vertices + (graph->vertex_length * s) && manage(v, am); v += s) {}
}

void _each_edge_iam_graph(iam_graph_s const * const graph, manage_fn const manage, void * const am) {
    error(graph && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // since an edge is defined as a relation between 2 vertices,
    // edges are handled as 'iam_edge_s' structures
    // for each unique vertex combination (Ni x Nj) except Ni == Nj
    for (size_t i = 0; i < graph->vertex_length; ++i) {
        for (size_t j = i + 1; j < graph->vertex_length; ++j) {
            // if edge is special (none) continue since manage can't be performed
            // if handler terminates (returns false) end loop by returning
            size_t const dense_index = graph->sparse[((j * (j - 1)) / 2) + i];
            if (IAM_GRAPH_SPECIAL != dense_index) {
                iam_edge_s edge = { .weight = graph->edges + (dense_index * graph->edge_size), .vertices = { i, j } };
                if (!manage(&edge, am)) return;
            }
        }
    }
}

void _each_weight_iam_graph(iam_graph_s const * const graph, manage_fn const manage, void * const am) {
    error(graph && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const s = graph->vertex_size;
    for (char * e = graph->edges; e < graph->edges + (graph->edge_length * s) && manage(e, am); e += s) {}
}

void _each_neighbor_weight_iam_graph(iam_graph_s const * const graph, size_t const index, manage_fn const manage, void * const am) {
    error(graph && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");
    error(index < graph->vertex_length && "Parameter can't exceed length.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const v = index;
    size_t const v_off = (v * (v - 1)) / 2;
    for (size_t i = 0; i < graph->vertex_length - 1; i++) {
        size_t const i_off = (i * (i + 1)) / 2;
        size_t const e = v <= i ? i_off + v : v_off + i;

        size_t const dense_index = graph->sparse[e];
        if (IAM_GRAPH_SPECIAL != dense_index) {
            void * weight = graph->edges + (dense_index * graph->edge_size);
            if (!manage(&weight, am)) return;
        }
    }
}

void _each_neighbor_edge_iam_graph(iam_graph_s const * const graph, size_t const index, manage_fn const manage, void * const am) {
    error(graph && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");
    error(index < graph->vertex_length && "Parameter can't exceed length.");

    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t const v = index;
    size_t const v_off = (v * (v - 1)) / 2;
    for (size_t i = 0; i < graph->vertex_length - 1; i++) {
        size_t const i_off = (i * (i + 1)) / 2;
        size_t const e = v <= i ? i_off + v : v_off + i;

        size_t const u = (i < v) ? i : i + 1;
        size_t const dense_index = graph->sparse[e];
        if (IAM_GRAPH_SPECIAL != dense_index) {
            iam_edge_s edge = { .weight = graph->edges + (dense_index * graph->edge_size), .vertices = { v, u } };
            if (!manage(&edge, am)) return;
        }
    }
}

void _each_cost_iam_graph(table_s const * const table, manage_fn const manage, void * const am) {
    error(table && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");

    iam_graph_s const * graph = table->graph;

    valid(graph && "Table graph can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // for each cost in table
    for (size_t i = 0; i < graph->vertex_length && manage(table->costs + (table->size * i), am); ++i) {}
}

size_t _each_path_iam_table(table_s const * const table, size_t const end, manage_fn const manage, void * const am) {
    error(table && "Parameter can't be NULL.");
    error(manage && "Parameter can't be NULL.");

    iam_graph_s const * graph = table->graph;
    error(end < graph->vertex_length && "Parameter can't exceed length.");

    valid(graph && "Table graph can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // initialize vertex index stack since paths (Dijkstra and A*) are traverse recursivelly
    fstack_s stack = create_fstack(sizeof(size_t), graph->vertex_length);

    // push end vertex index to stack and traverse table until no vertex doesn't have a previous parent
    // (previous is IAM_SPECIAL)
    push_fstack(&stack, &end);
    for (size_t i = table->previous[end]; IAM_GRAPH_SPECIAL != i; i = table->previous[i]) {
        push_fstack(&stack, &i);
    }

    size_t top = 0; // save top to check if path exists (top has cost 0)
    peep_fstack(&stack, &top);

    // recursivelly traverse paths from start to end index
    while (!is_empty_fstack(&stack)) {
        size_t index = 0;
        pop_fstack(&stack, &index);

        char * vertex = graph->vertices + (index * graph->vertex_size);
        if (!manage(vertex, am)) { break; }
    }

    // free path stack
    destroy_fstack(&stack, voiddst, NULL);

    // return true if last index cost/rank is zero (path exists)
    return top;
}

void _each_vertex_bfs_iam_graph(iam_graph_s const * const graph, size_t const start, size_t const end, manage_fn const manage, void * const am) {
}

void _each_vertex_dfs_iam_graph(iam_graph_s const * const graph, size_t const start, size_t const end, manage_fn const manage, void * const am) {
}

void _iam_graph_resize(iam_graph_s * const graph, size_t const size) {
    // calculate old and new edge capacity to only set expanded edges to non
    size_t const old_edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    size_t const _edge_capacity = (size * (size - 1)) / 2;

    graph->capacity = size;

    // resize edge pointer (special case)
    graph->sparse = graph->allocator->realloc(graph->edges, _edge_capacity * sizeof(size_t), graph->allocator->arg);
    graph->dense  = graph->allocator->realloc(graph->dense, _edge_capacity * sizeof(size_t), graph->allocator->arg);
    // make expanded edges non-edge (ignored if new capacity shrank)
    for (size_t i = old_edge_capacity; i < _edge_capacity; ++i) {
        graph->sparse[i] = IAM_GRAPH_SPECIAL;
    }

    // resize vertecies, edges, sparse edges and degrees pointers
    graph->vertices = graph->allocator->realloc(graph->vertices, size * graph->vertex_size, graph->allocator->arg);
    graph->edges = graph->allocator->realloc(graph->edges, size * graph->edge_size, graph->allocator->arg);
    graph->degrees = graph->allocator->realloc(graph->degrees, size * sizeof(size_t), graph->allocator->arg);
}

size_t _find_parent(table_s const * const table, size_t const vertex) {
    // non-recursive find-parent (find-set) algorithm
    size_t start = vertex;
    while (start != table->previous[vertex]) {
        start = table->previous[start];
    }

    return start;
}

void _union_set(table_s const * const table, size_t const source, size_t const destination, set_fn const increment, void * const ai, compare_fn const compare, void * const ac) {
    // find source and destination roots
    size_t const source_root      = _find_parent(table, source);
    size_t const destination_root = _find_parent(table, destination);

    // union set algorithm
    void * source_cost = table->costs + (source_root * table->size);
    void const * destination_cost = table->costs + (destination_root * table->size);
    if (compare(source_cost, destination_cost, ac) < 0) {
        table->previous[source_root] = destination_root;
    } else if (compare(source_cost, destination_cost, ac) > 0) {
        table->previous[destination_root] = source_root;
    } else {
        table->previous[destination_root] = source_root;
        increment(source_cost, ai);
    }
}

void _set_visited(struct visited_list_array * const visited, size_t const index) {
    insert_fbitwise_set(&visited->set, index);

    // if index is head, thne head must be updated to its next node index
    if (index == visited->head) {
        visited->head = visited->next[index];
    }
    // cur out visited index node from visited set list
    visited->prev[visited->next[index]] = visited->prev[index];
    visited->next[visited->prev[index]] = visited->next[index];
}

void _initialize_table(table_s * const table, iam_graph_s const * const graph, void const * const infinite, bool const is_self) {
    table->graph = graph;
    table->costs = graph->allocator->alloc(graph->vertex_length * table->size, graph->allocator->arg);
    error((!graph->vertex_length || table->costs) && "Memory allocation failed.");
    table->previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arg);
    error((!graph->vertex_length || table->previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        memcpy(table->costs + (i * table->size), infinite, table->size);
        table->previous[i] = is_self ? i : IAM_GRAPH_SPECIAL;
    }
}
