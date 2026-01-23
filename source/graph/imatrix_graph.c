#include <graph/imatrix_graph.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct iam_graph_queue {
    size_t * array;
    size_t length, current;
};

struct iam_graph_stack {
    size_t * array;
    size_t length;
};

struct iam_graph_smallest {
    size_t vertex;
    void * cost;
};

void _imatrix_graph_resize(iam_graph_s * graph, const size_t size);
void _bfs_helper(const iam_graph_s * graph, const iam_list_s * table, struct iam_graph_queue * queue, bool * visited, const size_t vertex, const size_t i, const size_t e);
void _dfs_helper(const iam_graph_s * graph, const iam_list_s * table, struct iam_graph_stack * stack, bool * visited, const size_t vertex, const size_t i, const size_t e);
void _dijkstra_helper(const iam_graph_s * graph, const bool * visited, const iam_list_s * table, struct iam_graph_smallest * minimum, void * sum_cost, const size_t current_vertex, const void * current_cost, const size_t j, const size_t e);
void _bellman_ford_helper(const iam_graph_s * graph, const iam_list_s * table, void * sum_cost, const size_t start, const size_t j, const size_t current_vertex, const void * current_cost, const size_t k, const size_t e);
void _prim_helper(const iam_graph_s * graph, const iam_list_s * table, const bool * visited, struct iam_graph_smallest * minimum, void * distance_cost, const size_t current_vertex, const size_t j, const size_t e);
size_t _find_parent(const iam_list_s * table, const size_t vertex);
void _union_set(const iam_list_s * table, const size_t source, const size_t destination, const set_fn increment_cost);

iam_cost_s compose_iam_cost(const size_t size, const compare_fn compare, const copy_fn add, void * zero, void * infinite) {
    return (iam_cost_s) {
        .compare = compare, .size = size, .zero = zero, .infinite = infinite, .add = add,
    };
}

iam_graph_s create_iam_graph(const size_t vertex_size, const size_t edge_size, const compare_fn compare, void * none) {
    return (iam_graph_s) { .vertex_size = vertex_size, .edge_size = edge_size, .compare = compare, .none = none, };
}

void destroy_iam_graph(iam_graph_s * graph, const set_fn destroy_vertex, const set_fn destroy_edge) {
    for (size_t i = 0; i < graph->length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->length * (graph->length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) {
            destroy_edge(edge);
        }
    }

    free(graph->vertices);
    free(graph->edges);

    memset(graph, 0, sizeof(iam_graph_s));
}

void clear_iam_graph(iam_graph_s * graph, const set_fn destroy_vertex, const set_fn destroy_edge) {
    for (size_t i = 0; i < graph->length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->length * (graph->length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) {
            destroy_edge(edge);
        }
    }

    free(graph->vertices);
    free(graph->edges);

    graph->capacity = graph->length = 0;
    graph->vertices = graph->edges = NULL;
}

iam_graph_s copy_iam_graph(const iam_graph_s * graph, const copy_fn copy_vertex, const copy_fn copy_edge) {
    const size_t edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;

    const iam_graph_s replica = {
        .vertex_size = graph->vertex_size, .edge_size = graph->edge_size, .compare = graph->compare,
        .none = graph->none, .length = graph->length, .capacity = graph->capacity,

        .vertices = malloc(graph->capacity * graph->vertex_size),
        .edges = malloc(edge_capacity * graph->edge_size),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        copy_vertex(replica.vertices + (i * replica.vertex_size), graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->length * (graph->length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        const void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) {
            copy_edge(replica.edges + (i * replica.edge_size), edge);
        }
    }

    return replica;
}

bool is_empty_iam_graph(const iam_graph_s * graph) {
    return !(graph->length);
}

size_t insert_vertex_iam_graph(iam_graph_s * graph, const void * vertex) {
    if (graph->length == graph->capacity) {
        _imatrix_graph_resize(graph, graph->capacity + IMATRIX_CHUNK);
    }

    memcpy(graph->vertices + (graph->length * graph->vertex_size), vertex, graph->vertex_size);

    return graph->length++;
}

size_t remove_vertex_iam_graph(iam_graph_s * graph, const size_t index, void * buffer, const set_fn destroy_edge) {
    // last vertex's first matrix edge
    const size_t start_last = ((graph->length - 1) * (graph->length - 2)) / 2;

    // destroy removed vertex' row edges and replace them with last's edges
    const size_t start_row = (index * (index - 1)) / 2; // index vertex's first row edge
    for (size_t r = 0; r < index; r++) {
        // calculate removed and last vertex' edges
        void * edge_index = graph->edges + ((start_row + r) * graph->edge_size);
        void * edge_last = graph->edges + ((start_last + r) * graph->edge_size);

        // if an edge exists for removed vertex then destroy it
        if (graph->compare(graph->none, edge_index)) {
            destroy_edge(edge_index);
        }

        // memmove since pointers can point to the same memory
        memmove(edge_index, edge_last, graph->edge_size); // move last's edge into removed
        memmove(edge_last, graph->none, graph->edge_size); // change last's edge into non-edge after moving it away
    }

    // destroy removed vertex' column edges and replace them with last's edges (except the one shared with last)
    for (size_t c = index, start_col = start_row + c; c < graph->length - 1; c++, start_col += c) {
        // calculate removed and last vertex' edges
        void * edge_index = graph->edges + ((start_col + c) * graph->edge_size);
        void * edge_last = graph->edges + ((start_last + c + 1) * graph->edge_size);

        // if an edge exists for removed vertex then destroy it
        if (graph->compare(graph->none, edge_index)) {
            destroy_edge(edge_index);
        }

        // memmove since pointers can point to the same memory
        memmove(edge_index, edge_last, graph->edge_size); // move last's edge into removed
        memmove(edge_last, graph->none, graph->edge_size); // change last's edge into non-edge after moving it away
    }

    // remove edge shared with last vertex in array if removed vertex isn't last and an edge exists
    void * edge_last = graph->edges + ((start_last + index) * graph->edge_size);
    if (index != graph->length - 1 && graph->compare(graph->none, edge_last)) {
        destroy_edge(edge_last);
    }

    // save removed index into buffer and move last vertex into removed's position
    graph->length--;
    memcpy(buffer, graph->vertices + (graph->length * graph->vertex_size), graph->vertex_size);
    memmove(graph->vertices + (index * graph->vertex_size), graph->vertices + (graph->length * graph->vertex_size), graph->vertex_size);

    // shrink graph if elements fit into smaller memory chunk
    if (graph->length == graph->capacity - IMATRIX_CHUNK) {
        _imatrix_graph_resize(graph, graph->length);
    }

    // if last index was removed return an invalid index, else return the last vertex' index before it was moved
    return (index == graph->length) ? IMATRIX_NIL : graph->length;
}

void get_vertex_iam_graph(const iam_graph_s * graph, const size_t index, void * buffer) {
    memcpy(buffer, graph->vertices + (index * graph->vertex_size), graph->vertex_size);
}

void insert_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, const void * edge) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that no edge between vertices exists yet
    assert(!graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(graph->edges + ((start_row + minimum) * graph->edge_size), edge, graph->edge_size);
}

void remove_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    assert(graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(buffer, graph->edges + ((start_row + minimum) + graph->edge_size), graph->edge_size);
    memcpy(graph->edges + ((start_row + minimum) + graph->edge_size), graph->none, graph->edge_size);
}

bool contains_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    return graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size));
}

void get_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    assert(graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(buffer, graph->edges + ((start_row + minimum) + graph->edge_size), graph->edge_size);
}

iam_list_s bfs_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end) {
    bool * visited = malloc(sizeof(bool) * graph->length);

    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * cost->size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        visited[i] = false;
        memcpy(table.cost + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IMATRIX_NIL;
    }
    memcpy(table.cost + (table.data->size * start), table.data->zero, table.data->size);

    struct iam_graph_queue queue = {
        .array = malloc(sizeof(size_t) * graph->length),
    };

    visited[start] = 1;
    queue.array[queue.length++] = start;

    while (queue.length && end != queue.array[queue.current]) {
        const size_t vertex = queue.array[queue.current++];
        queue.length--;

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0, e = offset; i < vertex; ++i, e++) {
            _bfs_helper(graph, &table, &queue, visited, vertex, i, e);
        }

        for (size_t i = vertex + 1, e = offset + (2 * vertex); i < graph->length; e += i++) {
            _bfs_helper(graph, &table, &queue, visited, vertex, i, e);
        }
    }

    free(queue.array);
    free(visited);

    return table;
}

iam_list_s dfs_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end) {
    bool * visited = malloc(sizeof(bool) * graph->length);

    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        visited[i] = false;
        memcpy(table.cost + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IMATRIX_NIL;
    }
    memcpy(table.cost + (table.data->size * start), table.data->zero, table.data->size);

    struct iam_graph_stack stack = {
        .array = malloc(sizeof(size_t) * graph->length),
    };

    visited[start] = 1;
    stack.array[stack.length++] = start;

    while (stack.length && end != stack.array[stack.length - 1]) {
        const size_t vertex = stack.array[--stack.length];

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0, e = offset; i < vertex; ++i, e++) {
            _dfs_helper(graph, &table, &stack, visited, vertex, i, e);
        }

        for (size_t i = vertex + 1, e = offset + (2 * vertex); i < graph->length; e += i++) {
            _dfs_helper(graph, &table, &stack, visited, vertex, i, e);
        }
    }

    free(stack.array);
    free(visited);

    return table;
}

iam_list_s dijkstra_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end) {
    bool * visited = malloc(sizeof(bool) * graph->length);

    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * cost->size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        visited[i] = false;
        memcpy(table.cost + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IMATRIX_NIL;
    }
    memcpy(table.cost + (table.data->size * start), table.data->zero, table.data->size);

    char * buffer = malloc(2 * cost->size); // allocate for both minimum and sum
    assert(buffer && "[ERROR] Memory allocation failed.");

    struct iam_graph_smallest minimum = {
        .vertex = start, .cost = buffer,
    };
    memcpy(minimum.cost, table.data->zero, table.data->size);

    char * sum_cost = buffer + cost->size;
    for (size_t i = 0; i < graph->length - 1 && end != minimum.vertex && IMATRIX_NIL != minimum.vertex; ++i) {
        const size_t current_vertex = minimum.vertex;
        const void * current_cost = table.cost + (minimum.vertex * table.data->size);

        minimum.vertex = IMATRIX_NIL;
        memcpy(minimum.cost, table.data->infinite, table.data->size);

        visited[current_vertex] = true;

        const size_t offset = (current_vertex * (current_vertex - 1)) / 2;
        for (size_t j = 0, e = offset; j < current_vertex; ++j, e++) {
            _dijkstra_helper(graph, visited, &table, &minimum, sum_cost, current_vertex, current_cost, j, e);
        }

        for (size_t j = current_vertex + 1, e = offset + (2 * current_vertex); j < graph->length; e += j++) {
            _dijkstra_helper(graph, visited, &table, &minimum, sum_cost, current_vertex, current_cost, j, e);
        }
    }

    free(visited);
    free(buffer);

    return table;
}

iam_list_s bellman_ford_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start) {
    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * cost->size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.cost + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IMATRIX_NIL;
    }
    memcpy(table.cost + (table.data->size * start), table.data->zero, table.data->size);

    void * sum_cost = malloc(cost->size);
    for (size_t i = 0; i < graph->length - 1; ++i) {
        for (size_t j = 0; j < graph->length; ++j) {
            const size_t current_vertex = j;
            const void * current_cost = table.cost + (j * cost->size);

            const size_t offset = (current_vertex * (current_vertex - 1)) / 2;
            for (size_t k = 0, e = offset; k < current_vertex; ++k, e++) {
                _bellman_ford_helper(graph, &table, sum_cost, start, j, current_vertex, current_cost, k, e);
            }

            for (size_t k = current_vertex + 1, e = offset + (2 * current_vertex); k < graph->length; e += k++) {
                _bellman_ford_helper(graph, &table, sum_cost, start, j, current_vertex, current_cost, k, e);
            }
        }
    }

    free(sum_cost);

    return table;
}

iam_list_s prim_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start) {
    bool * visited = malloc(sizeof(bool) * graph->length);

    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * cost->size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        visited[i] = false;
        memcpy(table.cost + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IMATRIX_NIL;
    }
    memcpy(table.cost + (table.data->size * start), table.data->zero, table.data->size);

    char * buffer = malloc(2 * cost->size); // allocate for both minimum and sum
    assert(buffer && "[ERROR] Memory allocation failed.");

    struct iam_graph_smallest minimum = {
        .vertex = start, .cost = buffer,
    };
    memcpy(minimum.cost, table.data->zero, table.data->size);

    char * distance_cost = buffer + cost->size;

    for (size_t i = 0; i < graph->length - 1 && IMATRIX_NIL != minimum.vertex; ++i) {
        const size_t current_vertex = minimum.vertex;

        minimum.vertex = IMATRIX_NIL;
        visited[current_vertex] = true;

        const size_t offset = (current_vertex * (current_vertex - 1)) / 2;
        for (size_t j = 0, e = offset; j < current_vertex; ++j, e++) {
            _prim_helper(graph, &table, visited, &minimum, distance_cost, current_vertex, j, e);
        }

        for (size_t j = current_vertex + 1, e = offset + (2 * current_vertex); j < graph->length; e += j++) {
            _prim_helper(graph, &table, visited, &minimum, distance_cost, current_vertex, j, e);
        }
    }

    free(visited);
    free(buffer);

    return table;
}

iam_list_s kruskal_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const process_fn sort, void * sort_arguments, const set_fn increment_cost) {
    const iam_list_s table = {
        .data = cost,
        .cost = malloc(graph->length * cost->size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.cost + (i * table.data->size), table.data->zero, table.data->size);
        table.previous[i] = i;
    }

    iam_edge_s * kruskal_edges = malloc(graph->length * sizeof(iam_edge_s));

    size_t edge_length = 0;
    for (size_t i = 0; i < graph->length; ++i) {
        for (size_t j = i + 1; j < graph->length; ++j) {
            const size_t offset = (j * (j - 1)) / 2;
            void * edge_weight = graph->edges + ((offset + i) * graph->edge_size);

            if (graph->compare(edge_weight, graph->none)) {
                kruskal_edges->edge = edge_weight;
                kruskal_edges->vertices[0] = i;
                kruskal_edges->vertices[1] = j;

                edge_length++;
            }
        }
    }

    sort(kruskal_edges, graph->length, sort_arguments);

    for (int i = 0; i < edge_length; ++i) {
        const iam_edge_s smallest = kruskal_edges[i]; // get the next smallest edge from sorted array of edges

        const size_t source = _find_parent(&table, smallest.vertices[0]);
        const size_t destination = _find_parent(&table, smallest.vertices[1]);

        if (source != destination) {
            _union_set(&table, source, destination, increment_cost);
        }
    }

    free(kruskal_edges);

    return table;
}

iam_list_s a_star_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end, const operate_fn heuristic, const copy_fn sum_cost) {
    return (iam_list_s) { 0 };
}

void map_cost_iam_list(const iam_graph_s * graph, const iam_list_s * table, const size_t start, const handle_fn handle, void * arguments) {
}

void destroy_iam_list(const iam_graph_s * graph, iam_list_s * table) {
    free(table->cost);
    free(table->previous);

    memset(table, 0, sizeof(iam_list_s));
}

iam_graph_s subgraph_iam_list(const iam_graph_s * graph, const iam_list_s * table, const copy_fn copy_vertex, const copy_fn copy_edge) {
    return (iam_graph_s) { 0 };
}

void map_vertex_iam_graph(const iam_graph_s * graph, const handle_fn handle, void * arguments) {
    for (char * v = graph->vertices; v < graph->vertices + (graph->length * graph->vertex_size); v += graph->vertex_size) {
        if (!handle(v, arguments)) { return; } // if handler terminates (returns false) end loop
    }
}

void map_edge_iam_graph(const iam_graph_s * graph, const handle_fn handle, void * arguments) {
    const size_t edge_length = (graph->length * (graph->length - 1)) / 2;
    for (char * e = graph->edges; e < graph->edges + (edge_length * graph->edge_size); e += graph->edge_size) {
        if (!graph->compare(e, graph->none)) { continue; } // if edge is none continue since handle can't be performed
        if (!handle(e, arguments)) { return; } // if handler terminates (returns false) end loop
    }
}

void _imatrix_graph_resize(iam_graph_s * graph, const size_t size) {
    const size_t old_edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    const size_t new_edge_capacity = (size * (size - 1)) / 2;

    graph->capacity = size;

    graph->edges = realloc(graph->edges, new_edge_capacity * graph->edge_size);
    for (size_t i = old_edge_capacity; i < new_edge_capacity; ++i) {
        memcpy(graph->edges + (i * graph->edge_size), graph->none, graph->edge_size);
    }

    graph->vertices = realloc(graph->vertices, size * graph->vertex_size);
}

void _bfs_helper(const iam_graph_s * graph, const iam_list_s * table, struct iam_graph_queue * queue, bool * visited, const size_t vertex, const size_t i, const size_t e) {
    const void * edge = graph->edges + (e * graph->edge_size);
    if (graph->compare(graph->none, edge) && !visited[i]) {
        visited[i] = true;
        queue->array[queue->current + queue->length++] = i;
    }

    table->previous[i] = vertex;
    memcpy(table->cost + (i * table->data->size), table->data->zero, table->data->size);
    table->data->add(table->cost + (i * table->data->size), edge);
}

void _dfs_helper(const iam_graph_s * graph, const iam_list_s * table, struct iam_graph_stack * stack, bool * visited, const size_t vertex, const size_t i, const size_t e) {
    const void * edge = graph->edges + (e * graph->edge_size);
    if (graph->compare(graph->none, edge) && !visited[i]) {
        visited[i] = true;
        stack->array[stack->length++] = i;
    }

    table->previous[i] = vertex;
    memcpy(table->cost + (i * table->data->size), table->data->zero, table->data->size);
    table->data->add(table->cost + (i * table->data->size), edge);
}

void _dijkstra_helper(const iam_graph_s * graph, const bool * visited, const iam_list_s * table, struct iam_graph_smallest * minimum, void * sum_cost, const size_t current_vertex, const void * current_cost, const size_t j, const size_t e) {
    if (visited[j]) { return; }

    // save edges for access
    const void * edge_weight = graph->edges + (e * graph->edge_size);
    void * table_cost = table->cost + (j * table->data->size);

    if (graph->compare(edge_weight, graph->none)) {
        // set alternative edge to none and add current edge and index edge together
        memcpy(sum_cost, current_cost, table->data->size);
        table->data->add(sum_cost, edge_weight);
    } else {
        memcpy(sum_cost, table->data->infinite, table->data->size);
    }

    // if sum's cost is smaller destroy table's non-infinite and set it to sum
    if (table->data->compare(sum_cost, table_cost) < 0) {
        table->previous[j] = current_vertex;
        memcpy(table_cost, sum_cost, table->data->size);
    }

    // if index edge exists and either distance edge is none or distance edge is bigger than index edge
    // then make index edge the new distance edge, and set minimum node to loop index
    if (table->data->compare(minimum->cost, table_cost) > 0) {
        memcpy(minimum->cost, table_cost, table->data->size);
        minimum->vertex = j;
    }
}

void _bellman_ford_helper(const iam_graph_s * graph, const iam_list_s * table, void * sum_cost, const size_t start, const size_t j, const size_t current_vertex, const void * current_cost, const size_t k, const size_t e) {
    // save edges for access
    const void * edge_weight = graph->edges + (e * graph->edge_size);
    void * table_cost = table->cost + (j * table->data->size);

    if (!graph->compare(edge_weight, graph->none) || k == start) {
        return;
    }

    memcpy(sum_cost, current_cost, table->data->size);
    table->data->add(sum_cost, edge_weight);

    // if sum's cost is smaller destroy table's non-infinite and set it to sum
    if (table->data->compare(sum_cost, table_cost) < 0) {
        table->previous[j] = current_vertex;
        memcpy(table_cost, sum_cost, table->data->size);
    }
}

void _prim_helper(const iam_graph_s * graph, const iam_list_s * table, const bool * visited, struct iam_graph_smallest * minimum, void * distance_cost, const size_t current_vertex, const size_t j, const size_t e) {
    if (visited[j]) { return; }

    // save edges for access
    const void * edge_weight = graph->edges + (e * graph->edge_size);
    void * table_cost = table->cost + (j * table->data->size);

    if (graph->compare(edge_weight, graph->none)) {
        // set alternative edge to none and add current edge and index edge together
        memcpy(distance_cost, table->data->zero, table->data->size);
        table->data->add(distance_cost, edge_weight);
    } else {
        memcpy(distance_cost, table->data->infinite, table->data->size);
    }

    // if sum's cost is smaller destroy table's non-infinite and set it to sum
    if (table->data->compare(distance_cost, table_cost) < 0) {
        table->previous[j] = current_vertex;
        memcpy(table_cost, distance_cost, table->data->size);
    }

    // if index edge exists and either distance edge is none or distance edge is bigger than index edge
    // then make index edge the new distance edge, and set minimum node to loop index
    if (table->data->compare(minimum->cost, table_cost) > 0) {
        memcpy(minimum->cost, table_cost, table->data->size);
        minimum->vertex = j;
    }
}

size_t _find_parent(const iam_list_s * table, const size_t vertex) {
    size_t start = vertex;
    while (start != table->previous[vertex]) {
        start = table->previous[start];
    }

    return start;
}

void _union_set(const iam_list_s * table, const size_t source, const size_t destination, const set_fn increment_cost) {
    const size_t source_root = _find_parent(table, source);
    const size_t destination_root = _find_parent(table, destination);

    void * source_cost = table->cost + (source_root * table->data->size);
    const void * destination_cost = table->cost + (destination_root * table->data->size);
    if (table->data->compare(source_cost, destination_cost) < 0) {
        table->previous[source_root] = destination_root;
    } else if (table->data->compare(source_cost, destination_cost) > 0) {
        table->previous[destination_root] = source_root;
    } else {
        table->previous[destination_root] = source_root;
        increment_cost(source_cost);
    }
}
