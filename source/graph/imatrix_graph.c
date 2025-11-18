#include <graph/imatrix_graph.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

void imatrix_graph_resize(imatrix_graph_s * graph, const size_t size);

imatrix_graph_s create_imatrix_graph(const size_t vertex_size, const size_t edge_size, const compare_fn compare, void * none) {
    return (imatrix_graph_s) { .vertex_size = vertex_size, .edge_size = edge_size, .compare = compare, .none = none, };
}

void destroy_imatrix_graph(imatrix_graph_s * graph, const destroy_fn destroy_vertex, const destroy_fn destroy_edge) {
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

    memset(graph, 0, sizeof(imatrix_graph_s));
}

void clear_imatrix_graph(imatrix_graph_s * graph, const destroy_fn destroy_vertex, const destroy_fn destroy_edge) {
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

imatrix_graph_s copy_imatrix_graph(const imatrix_graph_s * graph, const copy_fn copy_vertex, const copy_fn copy_edge) {
    const size_t edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;

    const imatrix_graph_s replica = {
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
        copy_edge(replica.edges + (i * replica.edge_size), graph->edges + (i * graph->edge_size));
    }

    return replica;
}

bool is_empty_imatrix_graph(const imatrix_graph_s * graph) {
    return !(graph->length);
}

size_t insert_vertex_imatrix_graph(imatrix_graph_s * graph, const void * vertex) {
    if (graph->length == graph->capacity) {
        imatrix_graph_resize(graph, graph->capacity + IMATRIX_GRAPH_CHUNK);
    }

    memcpy(graph->vertices + (graph->length * graph->vertex_size), vertex, graph->vertex_size);

    return graph->length++;
}

size_t remove_vertex_imatrix_graph(imatrix_graph_s * graph, const size_t index, void * buffer, const destroy_fn destroy_edge) {
    // last vertex's first matrix edge
    const size_t start_last = ((graph->length - 1) * ((graph->length - 1) - 1)) / 2;

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
    for (size_t c = index, start_col = ((index * (index + 1)) / 2) + c; c < graph->length - 1; c++, start_col += c) {
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
    if (graph->length == graph->capacity - IMATRIX_GRAPH_CHUNK) {
        imatrix_graph_resize(graph, graph->length);
    }

    // if last index was removed return an invalid index, else return the last vertex' index before it was moved
    return (index == graph->length) ? IMATRIX_NIL : graph->length;
}

void get_vertex_imatrix_graph(const imatrix_graph_s * graph, const size_t index, void * buffer) {
    memcpy(buffer, graph->vertices + (index * graph->vertex_size), graph->vertex_size);
}

void insert_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, const void * edge) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that no edge between vertices exists yet
    assert(!graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(graph->edges + ((start_row + minimum) * graph->edge_size), edge, graph->edge_size);
}

void remove_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    assert(graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(buffer, graph->edges + ((start_row + minimum) + graph->edge_size), graph->edge_size);
    memcpy(graph->edges + ((start_row + minimum) + graph->edge_size), graph->none, graph->edge_size);
}

bool contains_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    return graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size));
}

void get_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer) {
    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    assert(graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(buffer, graph->edges + ((start_row + minimum) + graph->edge_size), graph->edge_size);
}

imatrix_table_s breadth_first_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn copy_edge) {

    bool * visited = malloc(sizeof(bool) * graph->length);
    memset(visited, 0, sizeof(bool) * graph->length);

    const imatrix_table_s table = {
        .edges = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.edges + (i * graph->edge_size), graph->none, graph->edge_size);
        table.previous[i] = IMATRIX_NIL;
    }

    struct queue { size_t * array, length, current; } queue = {
        .array = malloc(sizeof(size_t) * graph->length),
    };

    visited[start] = 1;
    queue.array[queue.length++] = start;

    while (queue.length && end != queue.array[queue.current]) {
        const size_t vertex = queue.array[queue.current++];
        queue.length--;

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0; i < vertex; ++i) {
            const void * edge = graph->edges + ((offset + i) * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = 1;
                queue.array[queue.current + queue.length++] = i;
            }

            table.previous[i] = vertex;
            copy_edge(table.edges + (i * graph->edge_size), edge);
        }

        for (size_t i = vertex + 1, s = offset + (2 * vertex); i < graph->length; s += i++) {
            const void * edge = graph->edges + (s * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = 1;
                queue.array[queue.current + queue.length++] = i;
            }

            table.previous[i] = vertex;
            copy_edge(table.edges + (i * graph->edge_size), edge);
        }
    }

    free(queue.array);
    free(visited);

    return table;
}

imatrix_table_s depth_first_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn copy_edge) {
    bool * visited = malloc(sizeof(bool) * graph->length);
    memset(visited, 0, sizeof(bool) * graph->length);

    const imatrix_table_s table = {
        .edges = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.edges + (i * graph->edge_size), graph->none, graph->edge_size);
        table.previous[i] = IMATRIX_NIL;
    }

    struct stack { size_t * array, length; } stack = {
        .array = malloc(sizeof(size_t) * graph->length),
    };

    visited[start] = 1;
    stack.array[stack.length++] = start;

    while (stack.length && end != stack.array[stack.length - 1]) {
        const size_t vertex = stack.array[--stack.length];

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0; i < vertex; ++i) {
            const void * edge = graph->edges + ((offset + i) * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = 1;
                stack.array[stack.length++] = i;
            }

            table.previous[i] = vertex;
            copy_edge(table.edges + (i * graph->edge_size), edge);
        }

        for (size_t i = vertex + 1, s = offset + (2 * vertex); i < graph->length; s += i++) {
            const void * edge = graph->edges + (s * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = 1;
                stack.array[stack.length++] = i;
            }

            table.previous[i] = vertex;
            copy_edge(table.edges + (i * graph->edge_size), edge);
        }
    }

    free(stack.array);
    free(visited);

    return table;
}

imatrix_table_s dijkstra_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn add_edge, const destroy_fn destroy_edge) {
    bool * visited = malloc(sizeof(bool) * graph->length);
    memset(visited, 0, sizeof(bool) * graph->length);

    const imatrix_table_s table = {
        .edges = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.edges + (i * graph->edge_size), graph->none, graph->edge_size);
        table.previous[i] = IMATRIX_NIL;
    }

    //void * sum = malloc(graph->edge_size), * distance = malloc(graph->edge_size);
    void * sum = malloc(graph->edge_size);
    void * distance = malloc(graph->edge_size);
    for (size_t i = 0, minimum = start; i < graph->length - 1 && end != minimum; ++i) {
        const size_t current = minimum;
        visited[minimum] = 1;

        memcpy(distance, graph->none, graph->edge_size);

        void * cur = table.edges + (current * graph->edge_size);

        const size_t offset = (current * (current - 1)) / 2;
        for (size_t j = 0; j < current; ++j) {
            if (visited[j]) { // if node was visited continue
                continue;
            }

            // save edges for access
            void * idx = graph->edges + ((offset + j) * graph->edge_size);
            void * tab = table.edges + (j * graph->edge_size);

            // set alternative edge to none and add current edge and index edge together
            memcpy(sum, graph->none, graph->edge_size);
            add_edge(add_edge(sum, cur), idx);

            // if alternative edge is not none
            if (graph->compare(sum, graph->none)) {
                // if table edge is none or alternative edge is smaller, then replace old edge in table with new one
                if (graph->compare(idx, graph->none) && (!graph->compare(tab, graph->none) || graph->compare(sum, tab) < 0)) {
                    if (graph->compare(tab, graph->none)) {
                        destroy_edge(tab);
                    }

                    table.previous[j] = current;
                    memcpy(tab, sum, graph->edge_size);
                } else { // else get rid of new alternative edge
                    destroy_edge(sum);
                }
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (graph->compare(tab, graph->none) && (!graph->compare(distance, graph->none) || graph->compare(distance, tab) > 0)) {
                memcpy(distance, tab, graph->edge_size);
                minimum = j;
            }
        }

        for (size_t j = current + 1, s = offset + (2 * current); j < graph->length; s += j++) {
            if (visited[j]) { // if node was visited continue
                continue;
            }

            // save edges for access
            void * idx = graph->edges + (s * graph->edge_size);
            void * tab = table.edges + (j * graph->edge_size);

            // set alternative edge to none and add current edge and index edge together
            memcpy(sum, graph->none, graph->edge_size);
            add_edge(add_edge(sum, cur), idx);

            // if alternative edge is not none
            if (graph->compare(sum, graph->none)) {
                // if table edge is none or alternative edge is smaller, then replace old edge in table with new one
                if (graph->compare(idx, graph->none) && (!graph->compare(tab, graph->none) || graph->compare(sum, tab) < 0)) {
                    if (graph->compare(tab, graph->none)) {
                        destroy_edge(tab);
                    }

                    table.previous[j] = current;
                    memcpy(tab, sum, graph->edge_size);
                } else { // else get rid of new alternative edge
                    destroy_edge(sum);
                }
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (graph->compare(tab, graph->none) && (!graph->compare(distance, graph->none) || graph->compare(distance, tab) > 0)) {
                memcpy(distance, tab, graph->edge_size);
                minimum = j;
            }
        }
    }

    free(visited);
    free(sum);
    free(distance);

    return table;
}

imatrix_table_s bellman_ford_imatrix_table(const imatrix_graph_s * graph, const size_t start, const copy_fn add_edge, const destroy_fn destroy_edge) {
    const imatrix_table_s table = {
        .edges = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.edges + (i * graph->edge_size), graph->none, graph->edge_size);
        table.previous[i] = IMATRIX_NIL;
    }

    void * sum = malloc(graph->edge_size);
    for (size_t i = 0; i < graph->length - 1; ++i) {
        for (size_t j = 0; j < graph->length; ++j) {
            void * cur = table.edges + (j * graph->edge_size);

            const size_t offset = (j * (j - 1)) / 2;
            for (size_t k = 0; k < j; ++k) {
                void * idx = graph->edges + ((offset + k) * graph->edge_size);
                if (!graph->compare(idx, graph->none) || k == start) {
                    continue;
                }
                void * tab = table.edges + (k * graph->edge_size);

                // set alternative edge to none and add current edge and index edge together
                memcpy(sum, graph->none, graph->edge_size);
                add_edge(add_edge(sum, cur), idx);

                // if alternative edge is not none
                if (graph->compare(sum, graph->none)) {
                    // if table edge is none or alternative edge is smaller, then replace old edge in table with new one
                    if (!graph->compare(tab, graph->none) || graph->compare(sum, tab) < 0) {
                        if (graph->compare(tab, graph->none)) {
                            destroy_edge(tab);
                        }

                        table.previous[k] = j;
                        memcpy(tab, sum, graph->edge_size);
                    } else { // else get rid of new alternative edge
                        destroy_edge(sum);
                    }
                }
            }

            for (size_t k = j + 1, s = offset + (2 * j); k < graph->length; s += k++) {
                void * idx = graph->edges + (s * graph->edge_size);
                if (!graph->compare(idx, graph->none) || k == start) {
                    continue;
                }
                void * tab = table.edges + (k * graph->edge_size);

                // set alternative edge to none and add current edge and index edge together
                memcpy(sum, graph->none, graph->edge_size);
                add_edge(add_edge(sum, cur), idx);

                // if alternative edge is not none
                if (graph->compare(sum, graph->none)) {
                    // if table edge is none or alternative edge is smaller, then replace old edge in table with new one
                    if (!graph->compare(tab, graph->none) || graph->compare(sum, tab) < 0) {
                        if (graph->compare(tab, graph->none)) {
                            destroy_edge(tab);
                        }

                        table.previous[k] = j;
                        memcpy(tab, sum, graph->edge_size);
                    } else { // else get rid of new alternative edge
                        destroy_edge(sum);
                    }
                }
            }
        }
    }

    free(sum);

    return table;
}

imatrix_table_s prim_imatrix_table(const imatrix_graph_s * graph, const size_t start, const copy_fn copy_edge, const destroy_fn destroy_edge) {
    bool * visited = malloc(sizeof(bool) * graph->length);
    memset(visited, 0, sizeof(bool) * graph->length);

    const imatrix_table_s table = {
        .edges = malloc(graph->length * graph->edge_size),
        .previous = malloc(graph->length * sizeof(size_t)),
    };

    for (size_t i = 0; i < graph->length; ++i) {
        memcpy(table.edges + (i * graph->edge_size), graph->none, graph->edge_size);
        table.previous[i] = IMATRIX_NIL;
    }

    void * distance = malloc(graph->edge_size);
    for (size_t i = 0, minimum = start; i < graph->length - 1; ++i) {
        const size_t current = minimum;
        visited[minimum] = 1;

        memcpy(distance, graph->none, graph->edge_size);

        size_t start_edge = (current * (current - 1)) / 2;
        for (size_t j = 0; j < graph->length; start_edge += j < current ? 1 : j, ++j) {
            if (visited[j]) {
                continue;
            }

            // save edges for access
            void * idx = graph->edges + (start_edge * graph->edge_size);
            void * tab = table.edges + (j * graph->edge_size);

            if (graph->compare(idx, graph->none) && graph->compare(idx, tab) < 0) {
                if (graph->compare(tab, graph->none)) {
                    destroy_edge(tab);
                }
                copy_edge(tab, idx);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (graph->compare(tab, graph->none) && !graph->compare(distance, graph->none) || graph->compare(distance, tab) > 0) {
                memcpy(distance, tab, graph->edge_size);
                minimum = j;
            }
        }
    }

    free(visited);
    free(distance);

    return table;
}

imatrix_table_s kruskal_imatrix_table(const imatrix_graph_s * graph, const process_fn sort, void * sort_args, const copy_fn copy_edge, const destroy_fn destroy_edge) {
    return (imatrix_table_s) { 0 };
}

imatrix_table_s a_star_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const operate_fn add_edge, const operate_fn heuristic, const destroy_fn destroy_edge) {
    return (imatrix_table_s) { 0 };
}

void search_imatrix_table(const imatrix_graph_s * graph, const imatrix_table_s * table, const size_t start, const handle_fn operate, void * arguments) {
}

void destroy_imatrix_table(const imatrix_graph_s * graph, imatrix_table_s * table, const destroy_fn destroy_edge) {
    for (size_t i = 0; i < graph->length; ++i) {
        void * edge = table->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) {
            destroy_edge(edge);
        }
    }

    free(table->edges);
    free(table->previous);

    memset(table, 0, sizeof(imatrix_table_s));
}

imatrix_graph_s subgraph_imatrix_table(const imatrix_graph_s * graph, const imatrix_table_s * table, const copy_fn copy_vertex, const copy_fn copy_edge) {
    return (imatrix_graph_s) { 0 };
}

void map_vertex_imatrix_graph(const imatrix_graph_s * graph, const handle_fn handle, void * arguments) {
}

void map_edge_imatrix_graph(const imatrix_graph_s * graph, const handle_fn handle, void * arguments) {
}

void imatrix_graph_resize(imatrix_graph_s * graph, const size_t size) {
    const size_t old_edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    const size_t new_edge_capacity = (size * (size - 1)) / 2;

    graph->capacity = size;

    graph->edges = realloc(graph->edges, new_edge_capacity * graph->edge_size);
    for (size_t i = old_edge_capacity; i < new_edge_capacity; ++i) {
        memcpy(graph->edges + (i * graph->edge_size), graph->none, graph->edge_size);
    }

    graph->vertices = realloc(graph->vertices, size * graph->vertex_size);
}
