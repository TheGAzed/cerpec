#include <graph/iam_graph.h>

#include <stdlib.h>
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

void _imatrix_graph_resize(iam_graph_s * const graph, size_t const size);
size_t _find_parent(iam_list_s const * const table, size_t const vertex);
void _union_set(iam_list_s const * const table, size_t const source, size_t const destination, set_fn const increment_cost);

iam_cost_s compose_iam_cost(size_t const size, compare_fn const compare, copy_fn const convert, copy_fn const sum, void * const zero, void * const infinite) {
    error(size && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");
    error(convert && "Parameter can't be NULL.");
    error(sum && "Parameter can't be NULL.");
    error(zero && "Parameter can't be NULL.");
    error(infinite && "Parameter can't be NULL.");
    error(compare(infinite, zero) > 0 && "Infinite must be greater than zero.");
    error(compare(zero, infinite) < 0 && "Zero must be smaller than infinity.");

    return (iam_cost_s) {
        .compare = compare, .size = size, .zero = zero, .infinite = infinite, .sum = sum, .convert = convert,
    };
}

iam_graph_s create_iam_graph(size_t const vertex_size, size_t const edge_size, compare_fn const compare, void * const none) {
    error(vertex_size && "Parameter can't be zero.");
    error(edge_size && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");
    error(none && "Parameter can't be NULL.");

    return (iam_graph_s) {
        .vertex_size = vertex_size, .edge_size = edge_size, .compare = compare, .none = none, .allocator = &standard,
    };
}

iam_graph_s make_iam_graph(size_t const vertex_size, size_t const edge_size, compare_fn const compare, void * const none, memory_s const * const allocator) {
    error(vertex_size && "Parameter can't be zero.");
    error(edge_size && "Parameter can't be zero.");
    error(compare && "Parameter can't be NULL.");
    error(none && "Parameter can't be NULL.");
    error(allocator && "Parameter can't be NULL.");

    return (iam_graph_s) {
        .vertex_size = vertex_size, .edge_size = edge_size, .compare = compare, .none = none, .allocator = allocator,
    };
}

void destroy_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, set_fn const destroy_edge) {
    error(graph && "Parameter can't be NULL.");
    error(destroy_vertex && "Parameter can't be NULL.");
    error(destroy_edge && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->vertex_length * (graph->vertex_length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) {
            destroy_edge(edge);
        }
    }

    graph->allocator->free(graph->vertices, graph->allocator->arguments);
    graph->allocator->free(graph->edges, graph->allocator->arguments);

    memset(graph, 0, sizeof(iam_graph_s));
}

void clear_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, set_fn const destroy_edge) {
    error(graph && "Parameter can't be NULL.");
    error(destroy_vertex && "Parameter can't be NULL.");
    error(destroy_edge && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        destroy_vertex(graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->vertex_length * (graph->vertex_length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) { destroy_edge(edge); }
    }

    graph->allocator->free(graph->vertices, graph->allocator->arguments);
    graph->allocator->free(graph->edges, graph->allocator->arguments);

    graph->capacity = graph->vertex_length = 0;
    graph->vertices = graph->edges = NULL;
}

iam_graph_s copy_iam_graph(iam_graph_s const * const graph, copy_fn const copy_vertex, copy_fn const copy_edge) {
    error(graph && "Parameter can't be NULL.");
    error(copy_vertex && "Parameter can't be NULL.");
    error(copy_edge && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    const iam_graph_s replica = {
        .vertex_size = graph->vertex_size, .edge_size = graph->edge_size, .compare = graph->compare,
        .none = graph->none, .vertex_length = graph->vertex_length, .capacity = graph->capacity, .allocator = graph->allocator,

        .vertices = graph->allocator->alloc(graph->capacity * graph->vertex_size, graph->allocator->arguments),
        .edges = graph->allocator->alloc(edge_capacity * graph->edge_size, graph->allocator->arguments),
    };
    error((!replica.capacity || replica.vertices) && "Memory allocation failed.");
    error((!edge_capacity || replica.edges) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        copy_vertex(replica.vertices + (i * replica.vertex_size), graph->vertices + (i * graph->vertex_size));
    }

    const size_t edge_length = (graph->vertex_length * (graph->vertex_length - 1)) / 2;
    for (size_t i = 0; i < edge_length; ++i) {
        const void * edge = graph->edges + (i * graph->edge_size);
        if (graph->compare(graph->none, edge)) { copy_edge(replica.edges + (i * replica.edge_size), edge); }
    }

    return replica;
}

bool is_empty_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    return !(graph->vertex_length);
}

bool is_complete_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    return graph->edge_length == ((graph->vertex_length * (graph->vertex_length - 1)) / 2);
}

bool is_connected_iam_graph(iam_graph_s const * const graph) {
    error(graph && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // early return since an empty graph is NOT connected
    // also avoid unnecessary allocation and invalid memory access
    if (!graph->vertex_length) { return false; }

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error(visited && "Memory allocation failed.");
    memset(visited, 0, sizeof(bool) * graph->vertex_length); // set visited to false

    struct iam_graph_stack stack = {
        .array = graph->allocator->alloc(sizeof(size_t) * graph->vertex_length, graph->allocator->arguments),
    };
    error(stack.array && "Memory allocation failed.");

    stack.array[stack.length++] = 0;
    visited[0] = true;
    size_t visited_count = 1;

    while (stack.length) {
        size_t const vertex = stack.array[--stack.length];

        size_t const offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0, e = offset; i < vertex; ++i, e++) {
            void const * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                stack.array[stack.length++] = i;
                visited_count++;
            }
        }

        for (size_t i = vertex + 1, e = offset + (2 * vertex); i < graph->vertex_length; e += i++) {
            void const * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                stack.array[stack.length++] = i;
                visited_count++;
            }
        }
    }

    graph->allocator->free(stack.array, graph->allocator->arguments);
    graph->allocator->free(visited, graph->allocator->arguments);

    return graph->vertex_length == visited_count;
}

size_t insert_vertex_iam_graph(iam_graph_s * const graph, void const * const vertex) {
    error(graph && "Parameter can't be NULL.");
    error(vertex && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    if (graph->vertex_length == graph->capacity) {
        _imatrix_graph_resize(graph, graph->capacity + IAM_GRAPH_CHUNK);
    }

    memcpy(graph->vertices + (graph->vertex_length * graph->vertex_size), vertex, graph->vertex_size);

    return graph->vertex_length++;
}

size_t remove_vertex_iam_graph(iam_graph_s * const graph, size_t const index, void * const buffer, set_fn const destroy_edge) {
    error(graph && "Parameter can't be NULL.");
    error(index < graph->vertex_length && "Parameter can't exceed length.");
    error(buffer && "Parameter can't be NULL.");
    error(destroy_edge && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // last vertex's first matrix edge
    const size_t start_last = ((graph->vertex_length - 1) * (graph->vertex_length - 2)) / 2;

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
    for (size_t c = index, start_col = start_row + c; c < graph->vertex_length - 1; c++, start_col += c) {
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
    if (index != graph->vertex_length - 1 && graph->compare(graph->none, edge_last)) {
        destroy_edge(edge_last);
    }

    // save removed index into buffer and move last vertex into removed's position
    graph->vertex_length--;
    memcpy(buffer, graph->vertices + (graph->vertex_length * graph->vertex_size), graph->vertex_size);
    memmove(graph->vertices + (index * graph->vertex_size), graph->vertices + (graph->vertex_length * graph->vertex_size), graph->vertex_size);

    // shrink graph if elements fit into smaller memory chunk
    if (graph->vertex_length == graph->capacity - IAM_GRAPH_CHUNK) {
        _imatrix_graph_resize(graph, graph->vertex_length);
    }

    // if last index was removed return an invalid index, else return the last vertex' index before it was moved
    return graph->vertex_length;
}

void get_vertex_iam_graph(iam_graph_s const * const graph, size_t const index, void * const buffer) {
    error(graph && "Parameter can't be NULL.");
    error(index < graph->vertex_length && "Parameter can't exceed length.");
    error(buffer && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    memcpy(buffer, graph->vertices + (index * graph->vertex_size), graph->vertex_size);
}

void insert_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void const * const edge) {
    error(graph && "Parameter can't be NULL.");
    error(index_one < graph->vertex_length && "Parameter can't exceed length.");
    error(index_two < graph->vertex_length && "Parameter can't exceed length.");
    error(index_one != index_two && "Parameters can't be equal.");
    error(edge && "Parameter can't be NULL.");
    error(graph->compare(graph->none, edge) && "Parameter can't be none.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that no edge between vertices exists yet
    char * current_edge = graph->edges + ((start_row + minimum) * graph->edge_size);
    error(!graph->compare(graph->none, current_edge));

    memcpy(current_edge, edge, graph->edge_size);
    graph->edge_length++;
}

void remove_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void * const buffer) {
    error(graph && "Parameter can't be NULL.");
    error(index_one < graph->vertex_length && "Parameter can't exceed length.");
    error(index_two < graph->vertex_length && "Parameter can't exceed length.");
    error(index_one != index_two && "Parameters can't be equal.");
    error(buffer && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    char * edge = graph->edges + ((start_row + minimum) * graph->edge_size);
    error(graph->compare(graph->none, edge) && "Can't remove none.");

    memcpy(buffer, edge, graph->edge_size);
    memcpy(edge, graph->none, graph->edge_size);
    graph->edge_length--;
}

bool contains_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two) {
    error(graph && "Parameter can't be NULL.");
    error(index_one < graph->vertex_length && "Parameter can't exceed length.");
    error(index_two < graph->vertex_length && "Parameter can't exceed length.");
    error(index_one != index_two && "Parameters can't be equal.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    return graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size));
}

void get_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two, void * const buffer) {
    error(graph && "Parameter can't be NULL.");
    error(index_one < graph->vertex_length && "Parameter can't exceed length.");
    error(index_two < graph->vertex_length && "Parameter can't exceed length.");
    error(index_one != index_two && "Parameters can't be equal.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t minimum = index_one < index_two ? index_one : index_two;
    const size_t maximum = index_one >= index_two ? index_one : index_two;

    const size_t start_row = (maximum * (maximum - 1)) / 2;

    // assert that an edge exists between vertices
    assert(graph->compare(graph->none, graph->edges + ((start_row + minimum) * graph->edge_size)));

    memcpy(buffer, graph->edges + ((start_row + minimum) + graph->edge_size), graph->edge_size);
}

size_t degree_iam_graph(iam_graph_s const * const graph, size_t const index) {
    error(graph && "Parameter can't be NULL.");
    error(index < graph->vertex_length && "Parameter can't exceed length.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    size_t count = 0;
    size_t const offset = (index * (index - 1)) / 2;
    for (size_t i = 0, e = offset; i < index; ++i, e++) {
        char const * edge = graph->edges + (e * graph->edge_size);
        if (graph->compare(edge, graph->none)) {
            count++;
        }
    }

    for (size_t i = index + 1, e = offset + (2 * index); i < graph->vertex_length; e += i++) {
        char const * edge = graph->edges + (e * graph->edge_size);
        if (graph->compare(edge, graph->none)) {
            count++;
        }
    }

    return count;
}

iam_list_s bfs_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_NIL) && "Parameter can't exceed length.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error((!graph->vertex_length || visited) && "Memory allocation failed.");

    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * cost->size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited[i] = false;
        memcpy(table.costs + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IAM_NIL;
    }
    memcpy(table.costs + (table.data->size * start), table.data->zero, table.data->size);

    struct iam_graph_queue queue = {
        .array = graph->allocator->alloc(sizeof(size_t) * graph->vertex_length, graph->allocator->arguments),
    };
    error((!graph->vertex_length || queue.array) && "Memory allocation failed.");

    if (graph->vertex_length) {
        visited[start] = true;
        queue.array[queue.length++] = start;
    }

    while (queue.length && end != queue.array[queue.current]) {
        const size_t vertex = queue.array[queue.current++];
        queue.length--;

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0, e = offset; i < vertex; ++i, e++) {
            const void * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                queue.array[queue.current + queue.length++] = i;
            }

            table.previous[i] = vertex;
            memcpy(table.costs + (i * table.data->size), table.data->zero, table.data->size);
            table.data->convert(table.costs + (i * table.data->size), edge);
        }

        for (size_t i = vertex + 1, e = offset + (2 * vertex); i < graph->vertex_length; e += i++) {
            const void * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                queue.array[queue.current + queue.length++] = i;
            }

            table.previous[i] = vertex;
            memcpy(table.costs + (i * table.data->size), table.data->zero, table.data->size);
            table.data->convert(table.costs + (i * table.data->size), edge);
        }
    }

    graph->allocator->free(queue.array, graph->allocator->arguments);
    graph->allocator->free(visited, graph->allocator->arguments);

    return table;
}

iam_list_s dfs_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_NIL) && "Parameter can't exceed length.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error((!graph->vertex_length || visited) && "Memory allocation failed.");

    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * graph->edge_size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited[i] = false;
        memcpy(table.costs + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IAM_NIL;
    }
    memcpy(table.costs + (table.data->size * start), table.data->zero, table.data->size);

    struct iam_graph_stack stack = {
        .array = graph->allocator->alloc(sizeof(size_t) * graph->vertex_length, graph->allocator->arguments),
    };
    error((!graph->vertex_length || stack.array) && "Memory allocation failed.");

    if (graph->vertex_length) {
        visited[start] = true;
        stack.array[stack.length++] = start;
    }

    while (stack.length && end != stack.array[stack.length - 1]) {
        const size_t vertex = stack.array[--stack.length];

        const size_t offset = (vertex * (vertex - 1)) / 2;
        for (size_t i = 0, e = offset; i < vertex; ++i, e++) {
            const void * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                stack.array[stack.length++] = i;
            }

            table.previous[i] = vertex;
            memcpy(table.costs + (i * table.data->size), table.data->zero, table.data->size);
            table.data->convert(table.costs + (i * table.data->size), edge);
        }

        for (size_t i = vertex + 1, e = offset + (2 * vertex); i < graph->vertex_length; e += i++) {
            const void * edge = graph->edges + (e * graph->edge_size);
            if (graph->compare(graph->none, edge) && !visited[i]) {
                visited[i] = true;
                stack.array[stack.length++] = i;
            }

            table.previous[i] = vertex;
            memcpy(table.costs + (i * table.data->size), table.data->zero, table.data->size);
            table.data->convert(table.costs + (i * table.data->size), edge);
        }
    }

    graph->allocator->free(stack.array, graph->allocator->arguments);
    graph->allocator->free(visited, graph->allocator->arguments);

    return table;
}

iam_list_s dijkstra_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error((end < graph->vertex_length || end == IAM_NIL) && "Parameter can't exceed length.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error((!graph->vertex_length || visited) && "Memory allocation failed.");

    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * cost->size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited[i] = false;
        memcpy(table.costs + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IAM_NIL;
    }
    memcpy(table.costs + (table.data->size * start), table.data->zero, table.data->size);

    // allocate for both minimum and sum
    char * buffer = graph->allocator->alloc(2 * cost->size, graph->allocator->arguments);
    error(buffer && "[ERROR] Memory allocation failed.");

    struct iam_graph_smallest minimum = {
        .vertex = start, .cost = buffer,
    };
    memcpy(minimum.cost, table.data->zero, table.data->size);

    char * sum_cost = buffer + cost->size;
    for (size_t i = 0; graph->vertex_length && i < graph->vertex_length - 1 && end != minimum.vertex && IAM_NIL != minimum.vertex; ++i) {
        const size_t current = minimum.vertex;
        const void * current_cost = table.costs + (minimum.vertex * table.data->size);

        minimum.vertex = IAM_NIL;
        memcpy(minimum.cost, table.data->infinite, table.data->size);

        visited[current] = true;

        const size_t offset = (current * (current - 1)) / 2;
        for (size_t j = 0, e = offset; j < current; ++j, e++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_add = graph->compare(edge_weight, graph->none);
            memmove(sum_cost, can_add ? cost->sum(cost->convert(sum_cost, edge_weight), current_cost) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(sum_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, sum_cost, table.data->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (table.data->compare(minimum.cost, g_cost) > 0) {
                memcpy(minimum.cost, g_cost, table.data->size);
                minimum.vertex = j;
            }
        }

        for (size_t j = current + 1, e = offset + (2 * current); j < graph->vertex_length; e += j++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_add = graph->compare(edge_weight, graph->none);
            memmove(sum_cost, can_add ? cost->sum(cost->convert(sum_cost, edge_weight), current_cost) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(sum_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, sum_cost, table.data->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (table.data->compare(minimum.cost, g_cost) > 0) {
                memcpy(minimum.cost, g_cost, table.data->size);
                minimum.vertex = j;
            }
        }
    }

    graph->allocator->free(visited, graph->allocator->arguments);
    graph->allocator->free(buffer, graph->allocator->arguments);

    return table;
}

iam_list_s a_star_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end, operate_fn const heuristic) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");
    error(end < graph->vertex_length && "Parameter can't exceed length.");
    error(heuristic && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error((!graph->vertex_length || visited) && "Memory allocation failed.");

    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * cost->size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited[i] = false;
        memcpy(table.costs + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IAM_NIL;
    }
    memcpy(table.costs + (table.data->size * start), table.data->zero, table.data->size);

    // allocate for all costs to later free at once
    char * buffer = graph->allocator->alloc(4 * cost->size, graph->allocator->arguments);
    error(buffer && "[ERROR] Memory allocation failed.");

    struct iam_graph_smallest minimum = {
        .vertex = start, .cost = buffer,
    };
    memcpy(minimum.cost, table.data->zero, table.data->size);

    char * sum_cost = buffer + cost->size;
    char * h_cost = buffer + (2 * cost->size);
    char * f_cost = buffer + (3 * cost->size);
    for (size_t i = 0; graph->vertex_length && i < graph->vertex_length - 1 && end != minimum.vertex && IAM_NIL != minimum.vertex; ++i) {
        const size_t current = minimum.vertex;
        const void * current_cost = table.costs + (minimum.vertex * table.data->size);

        minimum.vertex = IAM_NIL;
        memcpy(minimum.cost, table.data->infinite, table.data->size);

        visited[current] = true;

        const size_t offset = (current * (current - 1)) / 2;
        for (size_t j = 0, e = offset; j < current; ++j, e++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_add = graph->compare(edge_weight, graph->none);
            memmove(sum_cost, can_add ? cost->sum(cost->convert(sum_cost, edge_weight), current_cost) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(sum_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, sum_cost, table.data->size);
            }

            // after vertex cost was updated determine F(n) by adding updated G(n) {as vertex cost} and H(n) together
            heuristic(h_cost, graph->vertices + (current * graph->vertex_size), graph->vertices + (end * graph->vertex_size));
            // neither g_cost nor h_cost is infinite (used to prevent potential overflow while summing)
            bool const can_sum = cost->compare(g_cost, cost->infinite) && cost->compare(h_cost, cost->infinite);
            memcpy(f_cost, can_sum ? cost->sum(h_cost, g_cost) : cost->infinite, cost->size);

            // if F(n)'s cost is smaller than the current minimum cost then set minimum cost to F(n)'s
            if (table.data->compare(minimum.cost, f_cost) > 0) {
                memcpy(minimum.cost, f_cost, table.data->size);
                minimum.vertex = j;
            }
        }

        for (size_t j = current + 1, e = offset + (2 * current); j < graph->vertex_length; e += j++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_add = graph->compare(edge_weight, graph->none);
            memmove(sum_cost, can_add ? cost->sum(cost->convert(sum_cost, edge_weight), current_cost) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(sum_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, sum_cost, table.data->size);
            }

            // after vertex cost was updated determine F(n) by adding updated G(n) {as vertex cost} and H(n) together
            heuristic(h_cost, graph->vertices + (current * graph->vertex_size), graph->vertices + (end * graph->vertex_size));
            // neither g_cost nor h_cost is infinite (used to prevent potential overflow while summing)
            bool const can_sum = cost->compare(g_cost, cost->infinite) && cost->compare(h_cost, cost->infinite);
            memcpy(f_cost, can_sum ? cost->sum(h_cost, g_cost) : cost->infinite, cost->size);

            // if F(n)'s cost is smaller than the current minimum cost then set minimum cost to F(n)'s
            if (table.data->compare(minimum.cost, f_cost) > 0) {
                memcpy(minimum.cost, f_cost, table.data->size);
                minimum.vertex = j;
            }
        }
    }

    graph->allocator->free(visited, graph->allocator->arguments);
    graph->allocator->free(buffer, graph->allocator->arguments);

    return table;
}

iam_list_s prim_iam_list(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(start < graph->vertex_length && "Parameter can't exceed length.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    bool * visited = graph->allocator->alloc(sizeof(bool) * graph->vertex_length, graph->allocator->arguments);
    error((!graph->vertex_length || visited) && "Memory allocation failed.");

    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * cost->size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        visited[i] = false;
        memcpy(table.costs + (i * table.data->size), table.data->infinite, table.data->size);
        table.previous[i] = IAM_NIL;
    }
    memcpy(table.costs + (table.data->size * start), table.data->zero, table.data->size);

    // allocate for both minimum and sum
    char * buffer = graph->allocator->alloc(2 * cost->size, graph->allocator->arguments);
    error(buffer && "[ERROR] Memory allocation failed.");

    struct iam_graph_smallest minimum = {
        .vertex = start, .cost = buffer,
    };
    memcpy(minimum.cost, table.data->zero, table.data->size);

    char * weight_cost = buffer + cost->size;
    for (size_t i = 0; graph->vertex_length && i < graph->vertex_length - 1 && IAM_NIL != minimum.vertex; ++i) {
        const size_t current = minimum.vertex;

        minimum.vertex = IAM_NIL;
        memcpy(minimum.cost, table.data->infinite, table.data->size);

        visited[current] = true;

        const size_t offset = (current * (current - 1)) / 2;
        for (size_t j = 0, e = offset; j < current; ++j, e++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_convert = graph->compare(edge_weight, graph->none);
            memmove(weight_cost, can_convert ? cost->convert(weight_cost, edge_weight) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(weight_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, weight_cost, table.data->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (table.data->compare(minimum.cost, g_cost) > 0) {
                memcpy(minimum.cost, g_cost, table.data->size);
                minimum.vertex = j;
            }
        }

        for (size_t j = current + 1, e = offset + (2 * current); j < graph->vertex_length; e += j++) {
            if (visited[j]) { continue; }

            // save edges for access
            const void * edge_weight = graph->edges + (e * graph->edge_size);
            void * g_cost = table.costs + (j * table.data->size);

            bool const can_convert = graph->compare(edge_weight, graph->none);
            memmove(weight_cost, can_convert ? cost->convert(weight_cost, edge_weight) : cost->infinite, cost->size);

            // if G's cost is smaller destroy table's non-infinite and set it to sum
            if (table.data->compare(weight_cost, g_cost) < 0) {
                table.previous[j] = current;
                memcpy(g_cost, weight_cost, table.data->size);
            }

            // if index edge exists and either distance edge is none or distance edge is bigger than index edge
            // then make index edge the new distance edge, and set minimum node to loop index
            if (table.data->compare(minimum.cost, g_cost) > 0) {
                memcpy(minimum.cost, g_cost, table.data->size);
                minimum.vertex = j;
            }
        }
    }

    graph->allocator->free(visited, graph->allocator->arguments);
    graph->allocator->free(buffer, graph->allocator->arguments);

    return table;
}

iam_list_s kruskal_iam_list(iam_graph_s const * const graph, iam_cost_s const * const cost, process_fn const sort, void * const arguments, set_fn const increment) {
    error(graph && "Parameter can't be NULL.");
    error(cost && "Parameter can't be NULL.");
    error(sort && "Parameter can't be NULL.");
    error(increment && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    // initialize return table
    const iam_list_s table = {
        .data = cost, .graph = graph,
        .costs = graph->allocator->alloc(graph->vertex_length * cost->size, graph->allocator->arguments),
        .previous = graph->allocator->alloc(graph->vertex_length * sizeof(size_t), graph->allocator->arguments),
    };
    error((!graph->vertex_length || table.costs) && "Memory allocation failed.");
    error((!graph->vertex_length || table.previous) && "Memory allocation failed.");

    // initialize edge array defined by the edge itself and vertices making it up
    iam_edge_s * kruskal_edges = graph->allocator->alloc(graph->edge_length * sizeof(iam_edge_s), graph->allocator->arguments);
    error((!graph->edge_length || kruskal_edges) && "Memory allocation failed.");

    // set edge array
    size_t edge_index = 0;
    for (size_t i = 0; i < graph->vertex_length; ++i) {
        // initializer table's previous and rank costs
        memcpy(table.costs + (i * table.data->size), table.data->zero, table.data->size);
        table.previous[i] = i;

        // initialize edge array to sort
        for (size_t j = i + 1; j < graph->vertex_length; ++j) {
            const size_t offset = (j * (j - 1)) / 2;
            void * edge_weight = graph->edges + ((offset + i) * graph->edge_size);

            if (graph->compare(edge_weight, graph->none)) {
                valid(edge_index < graph->edge_length && "Edge index exceeds lenght.");
                kruskal_edges[edge_index++] = (iam_edge_s) { .edge = edge_weight, .vertices = { i, j }};
            }
        }
    }

    // sort edges based on its edge (weight)
    sort(kruskal_edges, graph->vertex_length, arguments);

    // Kruskal's algorithm with sorted edges
    for (int i = 0; i < graph->edge_length; ++i) {
        const iam_edge_s smallest = kruskal_edges[i]; // get the next smallest edge from sorted array of edges

        const size_t source = _find_parent(&table, smallest.vertices[0]);
        const size_t destination = _find_parent(&table, smallest.vertices[1]);

        if (source != destination) {
            _union_set(&table, source, destination, increment);
        }
    }

    graph->allocator->free(kruskal_edges, graph->allocator->arguments);

    return table;
}

void destroy_iam_list(iam_list_s * const table) {
    error(table && "Parameter can't be NULL.");

    valid(table->data && "Cost data can't be NULL.");
    valid(table->graph && "Table graph can't be NULL.");
    valid(table->graph->compare && "Compare function can't be NULL.");
    valid(table->graph->edge_size && "Edge size can't be zero.");
    valid(table->graph->vertex_size && "Edge size can't be zero.");
    valid(table->graph->none && "Non-edge can't be NULL.");
    valid(table->graph->allocator && "Allocator can't be NULL.");

    table->graph->allocator->free(table->costs, table->graph->allocator->arguments);
    table->graph->allocator->free(table->previous, table->graph->allocator->arguments);

    memset(table, 0, sizeof(iam_list_s));
}

iam_graph_s subgraph_iam_list(iam_list_s const * const table, copy_fn const copy_vertex, copy_fn const copy_edge) {
    error(table && "Parameter can't be NULL.");
    error(copy_vertex && "Parameter can't be NULL.");
    error(copy_edge && "Parameter can't be NULL.");

    valid(table->data && "Cost data can't be NULL.");
    valid(table->graph && "Table graph can't be NULL.");
    valid(table->graph->compare && "Compare function can't be NULL.");
    valid(table->graph->edge_size && "Edge size can't be zero.");
    valid(table->graph->vertex_size && "Edge size can't be zero.");
    valid(table->graph->none && "Non-edge can't be NULL.");
    valid(table->graph->allocator && "Allocator can't be NULL.");

    const size_t edge_capacity = (table->graph->capacity * (table->graph->capacity - 1)) / 2;
    const iam_graph_s subgraph = {
        .vertex_size = table->graph->vertex_size, .edge_size = table->graph->edge_size, .compare = table->graph->compare,
        .none = table->graph->none, .vertex_length = table->graph->vertex_length, .capacity = table->graph->capacity,
        .allocator = table->graph->allocator,

        .vertices = table->graph->allocator->alloc(table->graph->capacity * table->graph->vertex_size, table->graph->allocator->arguments),
        .edges = table->graph->allocator->alloc(edge_capacity * table->graph->edge_size, table->graph->allocator->arguments),
    };
    error((!table->graph->capacity || subgraph.vertices) && "Memory allocation failed.");
    error((!table->graph->capacity || subgraph.edges) && "Memory allocation failed.");

    // initialize all edges to none/invalid
    for (size_t i = 0; i < edge_capacity; ++i) {
        char * subgraph_edge = subgraph.edges + (i * subgraph.edge_size);
        memcpy(subgraph_edge, subgraph.none, subgraph.edge_size);
    }

    // create vertex copies and subgraph edge copies based on existing cost
    for (size_t i = 0; i < table->graph->vertex_length; ++i) {
        copy_vertex(subgraph.vertices + (i * subgraph.vertex_size), table->graph->vertices + (i * table->graph->vertex_size));

        char const * cost = table->costs + (table->data->size * i);
        if (table->data->compare(cost, table->data->zero) && table->data->compare(cost, table->data->infinite)) {
            size_t const index_one = i;
            size_t const index_two = table->previous[i];

            size_t const minimum = index_one < index_two ? index_one : index_two;
            size_t const maximum = index_one >= index_two ? index_one : index_two;

            size_t const start_row = (maximum * (maximum - 1)) / 2;

            char * subgraph_edge = subgraph.edges + ((start_row + minimum) * subgraph.edge_size);
            char const * graph_edge = table->graph->edges + ((start_row + minimum) * table->graph->edge_size);
            copy_edge(subgraph_edge, graph_edge);
        }
    }

    return subgraph;
}

void each_vertex_iam_graph(iam_graph_s const * const graph, handle_fn const handle, void * const arguments) {
    error(graph && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (char * v = graph->vertices; v < graph->vertices + (graph->vertex_length * graph->vertex_size); v += graph->vertex_size) {
        if (!handle(v, arguments)) { return; } // if handler terminates (returns false) end loop
    }
}

void each_edge_iam_graph(iam_graph_s const * const graph, handle_fn const handle, void * const arguments) {
    error(graph && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < graph->vertex_length; ++i) {
        for (size_t j = i + 1; j < graph->vertex_length; ++j) {
            const size_t offset = (j * (j - 1)) / 2;
            void * weight = graph->edges + ((offset + i) * graph->edge_size);
            iam_edge_s edge = { .edge = weight, .vertices = { i, j } };

            // if edge is none continue since handle can't be performed (first condition is false so other won't be checked.)
            // if handler terminates (returns false) end loop
            if (graph->compare(&weight, graph->none) && !handle(&edge, arguments)) {
                return;
            }
        }
    }
}

void each_neighbor_iam_graph(iam_graph_s const * const graph, size_t const index, handle_fn const handle, void * const arguments) {
    error(graph && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(graph->compare && "Compare function can't be NULL.");
    valid(graph->edge_size && "Edge size can't be zero.");
    valid(graph->vertex_size && "Edge size can't be zero.");
    valid(graph->none && "Non-edge can't be NULL.");
    valid(graph->allocator && "Allocator can't be NULL.");

    const size_t offset = (index * (index - 1)) / 2;
    for (size_t i = 0, e = offset; i < index; ++i, e++) {
        char const * edge = graph->edges + (e * graph->edge_size);
        char * vertex = graph->vertices + (i * graph->vertex_size);

        // if edge is none continue since handle can't be performed (first condition is false so other won't be checked.)
        // if handler terminates (returns false) end loop
        if (graph->compare(edge, graph->none) && !handle(vertex, arguments)) {
            return;
        }
    }

    for (size_t i = index + 1, e = offset + (2 * index); i < graph->vertex_length; e += i++) {
        char const * edge = graph->edges + (e * graph->edge_size);
        char * vertex = graph->vertices + (i * graph->vertex_size);

        // if edge is none continue since handle can't be performed (first condition is false so other won't be checked.)
        // if handler terminates (returns false) end loop
        if (graph->compare(edge, graph->none) && !handle(vertex, arguments)) {
            return;
        }
    }
}

void each_cost_iam_list(iam_list_s const * const table, handle_fn const handle, void * const arguments) {
    error(table && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");

    valid(table->data && "Cost data can't be NULL.");
    valid(table->graph && "Table graph can't be NULL.");
    valid(table->graph->compare && "Compare function can't be NULL.");
    valid(table->graph->edge_size && "Edge size can't be zero.");
    valid(table->graph->vertex_size && "Edge size can't be zero.");
    valid(table->graph->none && "Non-edge can't be NULL.");
    valid(table->graph->allocator && "Allocator can't be NULL.");

    for (size_t i = 0; i < table->graph->vertex_length; ++i) {
        if (!handle(table->costs + (table->data->size * i), arguments)) { break; }
    }
}

bool each_path_iam_list(iam_list_s const * const table, size_t const end, handle_fn const handle, void * const arguments) {
    error(table && "Parameter can't be NULL.");
    error(handle && "Parameter can't be NULL.");
    error(end < table->graph->vertex_length && "Parameter can't exceed length.");

    valid(table->data && "Cost data can't be NULL.");
    valid(table->graph && "Table graph can't be NULL.");
    valid(table->graph->compare && "Compare function can't be NULL.");
    valid(table->graph->edge_size && "Edge size can't be zero.");
    valid(table->graph->vertex_size && "Edge size can't be zero.");
    valid(table->graph->none && "Non-edge can't be NULL.");
    valid(table->graph->allocator && "Allocator can't be NULL.");

    struct iam_graph_stack stack = {
        .array = table->graph->allocator->alloc(sizeof(size_t) * table->graph->vertex_length, table->graph->allocator->arguments),
    };
    error((!table->graph->vertex_length || stack.array) && "Memory allocation failed.");

    stack.array[stack.length++] = end;
    for (size_t i = table->previous[stack.array[stack.length - 1]]; IAM_NIL != i;) {
        stack.array[stack.length++] = i;

        i = table->previous[stack.array[stack.length - 1]];
    }
    size_t const top = stack.array[stack.length - 1];

    while (stack.length--) {
        char * vertex = table->graph->vertices + (stack.array[stack.length] * table->graph->vertex_size);
        if (!handle(vertex, arguments)) {
            break;
        }
    }

    table->graph->allocator->free(stack.array, table->graph->allocator->arguments);

    return !table->data->compare(table->costs + (top * table->data->size), table->data->zero);
}

void _imatrix_graph_resize(iam_graph_s * const graph, size_t const size) {
    const size_t old_edge_capacity = (graph->capacity * (graph->capacity - 1)) / 2;
    const size_t new_edge_capacity = (size * (size - 1)) / 2;

    graph->capacity = size;

    graph->edges = graph->allocator->realloc(graph->edges, new_edge_capacity * graph->edge_size, graph->allocator->arguments);
    for (size_t i = old_edge_capacity; i < new_edge_capacity; ++i) {
        memcpy(graph->edges + (i * graph->edge_size), graph->none, graph->edge_size);
    }

    graph->vertices = graph->allocator->realloc(graph->vertices, size * graph->vertex_size, graph->allocator->arguments);
}

size_t _find_parent(iam_list_s const * const table, size_t const vertex) {
    size_t start = vertex;
    while (start != table->previous[vertex]) {
        start = table->previous[start];
    }

    return start;
}

void _union_set(iam_list_s const * const table, size_t const source, size_t const destination, set_fn const increment_cost) {
    const size_t source_root = _find_parent(table, source);
    const size_t destination_root = _find_parent(table, destination);

    void * source_cost = table->costs + (source_root * table->data->size);
    const void * destination_cost = table->costs + (destination_root * table->data->size);
    if (table->data->compare(source_cost, destination_cost) < 0) {
        table->previous[source_root] = destination_root;
    } else if (table->data->compare(source_cost, destination_cost) > 0) {
        table->previous[destination_root] = source_root;
    } else {
        table->previous[destination_root] = source_root;
        increment_cost(source_cost);
    }
}
