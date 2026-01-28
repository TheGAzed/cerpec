#ifndef IMATRIX_H
#define IMATRIX_H

#include <cerpec.h>

#if !defined(IMATRIX_CHUNK)
#   define IMATRIX_CHUNK CERPEC_CHUNK
#endif

#define IMATRIX_NIL ((size_t)(-1))

/// @brief Stack data structure.
typedef struct infinite_matrix_graph {
    char * vertices; // array of elements
    char * edges;    // array of elements
    void * none;     // non-edge
    compare_fn compare; // comapres edges and determines if they're smaller, bigger or equal
    size_t vertex_size, edge_size, length, capacity; // size of single element, structure length and its capacity
} iam_graph_s;

typedef struct infinite_matrix_graph_edge {
    char * edge; // edge first to simplify weight comparison by dereferencing it directly to edge type
    size_t vertices[2];
} iam_edge_s;

typedef struct infinite_matrix_graph_cost {
    compare_fn compare;
    copy_fn add;
    size_t size;
    void * infinite, * zero;
} iam_cost_s;

typedef struct infinite_matrix_graph_table {
    iam_cost_s const * data;
    size_t * previous;
    char * cost;
} iam_list_s;

iam_cost_s compose_iam_cost(const size_t size, compare_fn compare, const copy_fn add, void * zero, void * infinite);

/// @brief Creates an empty structure.
/// @param vertex_size Size of a single vertex element.
/// @param edge_size Size of a single edge element.
/// @param compare Compare function for edge elements.
/// @param none Non-edge element to represent absence of an edge.
/// @return Graph structure.
/// @note Compare function must return equal if a 'none' edge is compared with 'none' parameter.
iam_graph_s create_iam_graph(const size_t vertex_size, const size_t edge_size, const compare_fn compare, void * none);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param destroy_edge Function pointer to destroy a single edge element.
void destroy_iam_graph(iam_graph_s * graph, const set_fn destroy_vertex, const set_fn destroy_edge);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param destroy_edge Function pointer to destroy a single edge element.
void clear_iam_graph(iam_graph_s * graph, const set_fn destroy_vertex, const set_fn destroy_edge);

/// @brief Creates a copy of a structure and all its elements.
/// @param graph Structure to copy.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @return Matrix graph structure.
iam_graph_s copy_iam_graph(const iam_graph_s * graph, const copy_fn copy_vertex, const copy_fn copy_edge);

/// @brief Checks if structure is empty.
/// @param graph Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_iam_graph(const iam_graph_s * graph);

/// @brief Inserts a single vertex element into the structure.
/// @param graph Structure to insert into.
/// @param vertex Element buffer to insert.
/// @return Index of element in vertex array.
size_t insert_vertex_iam_graph(iam_graph_s * graph, const void * vertex);

/// @brief Remove specified vertex element at index from structure.
/// @param graph Structure to remove from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
/// @param destroy_edge Function pointer to destroy single edge element associated wtih removed vertex.
/// @return Index of last swapped element in vertex array or 'IGRAPH_NIL', if removed element was last.
/// @note The last vertex in elements array (including its edges) gets swapped with the removed index vertex.
size_t remove_vertex_iam_graph(iam_graph_s * graph, const size_t index, void * buffer, const set_fn destroy_edge);

/// @brief Gets the vertex element at index in structure.
/// @param graph Structure to get from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
void get_vertex_iam_graph(const iam_graph_s * graph, const size_t index, void * buffer);

/// @brief Inserts a single edge element into the structure.
/// @param graph Structure to insert into.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param edge Element buffer to insert.
/// @return Index of element in vertex array.
void insert_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, const void * edge);

/// @brief Remove specified edge element at vertex indices from structure.
/// @param graph Structure to remove from.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param buffer Element buffer to insert.
void remove_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer);

/// @brief Checks if edge exists between two vertives.
/// @param graph Structure to check.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @return 'true' if an edge exists, 'false' otherwise.
/// @note Existance is dependent on 'none' structure parameter.
bool contains_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two);

/// @brief Gets the edge element at vertex indices in structure.
/// @param graph Structure to get from.
/// @param index_one First vertex index of edge.
/// @param index_two Second vertex index of edge.
/// @param buffer Element buffer to save edge.
void get_edge_iam_graph(const iam_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer);

/// @brief Traverses the vertices of the specified structure using breadth first search.
/// @param graph Structure to traverse.
/// @param cost Cost structure that defines the distance properties in table.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_NIL' if all vertex shortest path.
/// @return Breadth first search lookup table with subgraph from start node to all other nodes.
iam_list_s bfs_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end);

/// @brief Traverses the vertices of the specified structure using depth first search.
/// @param graph Structure to traverse.
/// @param cost Cost structure that defines the distance properties in table.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_NIL' if all vertex shortest path.
/// @return Depth first search lookup table with subgraph from start node to all other nodes.
iam_list_s dfs_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end);

/// @brief Generate a Dijkstra lookup array table with nodes' edge sums and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_NIL' if all vertex shortest path.
/// @param cost Cost structure that defines the distance properties in table.
/// @return Dijkstra lookup table with subgraph of shortest paths from start node to all other nodes.
iam_list_s dijkstra_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end);

/// @brief Generate a Bellman-Ford lookup array table with nodes' edge sums and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param cost Cost structure that defines the distance properties in table.
/// @return Bellman-Ford lookup table with subgraph of shortest paths from start node to all other nodes.
iam_list_s bellman_ford_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start);

/// @brief Generate an A* lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index.
/// @param sum_cost Function pointer to add two costs into one.
/// @param cost Cost structure that defines the distance properties in table.
/// @param heuristic Function pointer to determine heuristic distance based on two vectices.
/// @return A* lookup table with subgraph of shortest paths from start to end node.
iam_list_s a_star_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start, const size_t end, const operate_fn heuristic, const copy_fn sum_cost);

/// @brief Generate a Prim lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param cost Cost structure that defines the distance properties in table.
/// @return Prim lookup table with subgraph of minimum spanning tree from start node to all other nodes.
iam_list_s prim_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const size_t start);

/// @brief Generate a Kruskal lookup array table with nodes' ranks and previous indexes.
/// @param graph Structure to generate from.
/// @param sort Function pointer to sort special array of edge-vertices - 'iedge_s', for Kruskal's algorithm.
/// @param cost Cost structure that defines the distance properties in table.
/// @param sort_arguments Arguments for sorting function.
/// @param increment_cost Function pointer to increment rank cost by one.
/// @return Kruskal lookup table with subgraph of minimum spanning tree from start node to all other nodes.
iam_list_s kruskal_iam_list(const iam_graph_s * graph, const iam_cost_s * cost, const process_fn sort, void * sort_arguments, const set_fn increment_cost);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param graph Structure to reference.
/// @param table Structure to destroy.
void destroy_iam_list(const iam_graph_s * graph, iam_list_s * table);

/// @brief Creates a subgraph copy of specified graph based on its table.
/// @param graph Structure to copy.
/// @param table Structure to reference.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @return Matrix graph structure.
iam_graph_s subgraph_iam_list(const iam_graph_s * graph, const iam_list_s * table, const copy_fn copy_vertex, const copy_fn copy_edge);

/// @brief Iterates over each vertex element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Arguments for operate function pointer.
void each_vertex_iam_graph(const iam_graph_s * graph, const handle_fn handle, void * arguments);

void each_neighbor_iam_graph(iam_graph_s const * const graph, size_t const index, handle_fn const handle, void * const arguments);

/// @brief Iterates over each edge element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Arguments for operate function pointer.
void each_edge_iam_graph(const iam_graph_s * graph, const handle_fn handle, void * arguments);

/// @brief Traverses the costs of the specified structure using a generated table.
/// @param graph Structure to traverse.
/// @param table Structure to reference.
/// @param start Starting vertex index.
/// @param handle Operate function pointer to operate on each vertex.
/// @param arguments Arguments for operate function pointer.
void each_cost_iam_list(const iam_graph_s * graph, const iam_list_s * table, const size_t start, const handle_fn handle, void * arguments);

#endif // IMATRIX_H
