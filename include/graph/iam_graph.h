#ifndef IAM_GRAPH_H
#define IAM_GRAPH_H

#include <cerpec.h>

#if !defined(IAM_GRAPH_CHUNK)
#   define IAM_GRAPH_CHUNK CERPEC_CHUNK
#elif IAM_GRAPH_CHUNK <= 0
#   error "Chunk size must be greater than zero."
#elif (IAM_GRAPH_CHUNK & (IAM_GRAPH_CHUNK - 1))
#   error "Chunk size must be a power of 2."
#endif

#define IAM_NIL ((size_t)(-1))

/// @brief Infinte adjacency matrix graph data structure.
typedef struct infinite_adjacency_matrix_graph {
    char * vertices, * edges; // arrays of elements
    void * none;              // non-edge
    compare_fn compare;       // comapres edges and determines if they're smaller, bigger or equal
    size_t vertex_size, edge_size, vertex_length, edge_length, capacity; // size of single element, structure length and its capacity
    memory_s const * allocator;
} iam_graph_s;

/// @brief Graph edge represented by itself (or weight) and two vertex indexes.
typedef struct infinite_adjacency_matrix_graph_edge {
    char * edge; // edge first to simplify weight comparison by dereferencing it directly to edge type
    size_t vertices[2];
} iam_edge_s;

/// @brief Cost value of size that is comparable and is bound to <0, inf>
typedef struct infinite_adjacency_matrix_graph_cost {
    compare_fn compare;
    copy_fn sum, convert;
    size_t size;
    void * infinite, * zero, * ac, * as;
} iam_cost_s;

typedef struct infinite_matrix_graph_table {
    iam_graph_s const * graph;
    iam_cost_s const * data;
    size_t * previous;
    char * costs;
} iam_table_s;

/// @brief Composes a structure via its parametric properties.
/// @param size Size of cost.
/// @param compare Compare function pointer to do costs comparison.
/// @param convert Convert function pointer to convert an edge (weight) into a cost.
/// @param ac Arguments for convert function pointer.
/// @param sum Add function pointer to sum two cost.
/// @param as Arguments for sum function pointer.
/// @param zero A cost of zero representing an instantly reachable cost.
/// @param infinite An infinte cost representing an impossiblely reachable cost.
/// @return Cost structure.
iam_cost_s compose_iam_cost(size_t const size, compare_fn const compare, copy_fn const convert, void * const ac, copy_fn const sum, void * as, void * const zero, void * const infinite);

/// @brief Creates an empty structure.
/// @param vertex_size Size of a single vertex element.
/// @param edge_size Size of a single edge element.
/// @param compare Compare function for edge elements.
/// @param none Non-edge element to represent absence of an edge.
/// @return Graph structure.
/// @note Compare function must return equal if a 'none' edge is compared with 'none' parameter.
iam_graph_s create_iam_graph(size_t const vertex_size, size_t const edge_size, compare_fn const compare, void * const none);

/// @brief Creates an empty structure.
/// @param vertex_size Size of a single vertex element.
/// @param edge_size Size of a single edge element.
/// @param compare Compare function for edge elements.
/// @param none Non-edge element to represent absence of an edge.
/// @param allocator Custom allocator structure.
/// @return Graph structure.
/// @note Compare function must return 'equal' (0) if a non-edge is compared with 'none' parameter.
iam_graph_s make_iam_graph(size_t const vertex_size, size_t const edge_size, compare_fn const compare, void * const none, memory_s const * const allocator);

/// @brief Destroys a structure and its elements, but makes it unusable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param adv Arguments for destroy function pointer.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @param ade Arguments for destroy function pointer.
void destroy_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, void * const adv, set_fn const destroy_edge, void * const ade);

/// @brief Clears a structure and destroys its elements, but remains usable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param adv Arguments for destroy function pointer.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @param ade Arguments for destroy function pointer.
void clear_iam_graph(iam_graph_s * const graph, set_fn const destroy_vertex, void * const adv, set_fn const destroy_edge, void * const ade);

/// @brief Creates a copy of a structure and all its elements.
/// @param graph Structure to copy.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param acv Arguments for copy vertex function pointer.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @param ace Arguments for copy edge function pointer.
/// @return Graph structure.
iam_graph_s copy_iam_graph(iam_graph_s const * const graph, copy_fn const copy_vertex, void * const acv, copy_fn const copy_edge, void * const ace);

/// @brief Checks if structure is empty.
/// @param graph Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_iam_graph(iam_graph_s const * const graph);

/// @brief A graph is complete if every distinct pair of vertices is connected by a unique edge.
/// @param graph Structure to check.
/// @return 'true' if graph is complete, 'false' otherwise.
bool is_complete_iam_graph(iam_graph_s const * const graph);

/// @brief A graph is connected if a path exists between every pair of vertices.
/// @param graph Structure to check.
/// @return 'true' if graph is connected, 'false' otherwise.
/// @note The algorithm uses DFS to determine connectivity.
bool is_connected_iam_graph(iam_graph_s const * const graph);

/// @brief A graph is a tree if it's connected and contains no cycles.
/// @param graph Structure to check.
/// @return 'true' if graph is a tree, 'false' otherwise.
/// @note The algorithm uses DFS to determine tree-ness.
bool is_tree_iam_graph(iam_graph_s const * const graph);

/// @brief Inserts a single vertex element into the structure.
/// @param graph Structure to insert into.
/// @param vertex Element buffer to insert.
/// @return Index of element in vertex array.
size_t insert_vertex_iam_graph(iam_graph_s * const graph, void const * const vertex);

/// @brief Remove specified vertex element at index from structure.
/// @param graph Structure to remove from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
/// @param destroy_edge Function pointer to destroy single edge element associated wtih removed vertex.
/// @param ade Arguments for destroy function pointer.
/// @return Index of last swapped element in vertex array.
/// @note The last vertex in elements array (including its edges) gets swapped with the removed index vertex.
size_t remove_vertex_iam_graph(iam_graph_s * const graph, size_t const index, void * const buffer, set_fn const destroy_edge, void * const ade);

/// @brief Gets the vertex element at index in structure.
/// @param graph Structure to get from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
void get_vertex_iam_graph(iam_graph_s const * const graph, size_t const index, void * const buffer);

/// @brief Inserts a single edge element into the structure.
/// @param graph Structure to insert into.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param edge Element buffer to insert.
/// @return Index of element in vertex array.
void insert_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void const * const edge);

/// @brief Remove specified edge element at vertex indices from structure.
/// @param graph Structure to remove from.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param buffer Element buffer to insert.
void remove_edge_iam_graph(iam_graph_s * const graph, size_t const index_one, size_t const index_two, void * const buffer);

/// @brief Checks if edge exists between two vertives.
/// @param graph Structure to check.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @return 'true' if an edge exists, 'false' otherwise.
/// @note Edge existance dependents on 'none' structure parameter.
bool contains_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two);

/// @brief Gets the edge element at vertex indices in structure.
/// @param graph Structure to get from.
/// @param index_one First vertex index of edge.
/// @param index_two Second vertex index of edge.
/// @param buffer Element buffer to save edge.
void get_edge_iam_graph(iam_graph_s const * const graph, size_t const index_one, size_t const index_two, void * const buffer);

/// @brief Calculates the number of edges that are incident to the vertex index.
/// @param graph Structure to get from.
/// @param index Vertex index.
/// @return Degree of vertex.
size_t degree_iam_graph(iam_graph_s const * const graph, size_t const index);

/// @brief Traverses the vertices of the specified structure using breadth first search.
/// @param graph Structure to traverse.
/// @param cost Cost structure that defines the distance properties in table.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IAM_NIL' if all vertex shortest path.
/// @return Breadth first search lookup table with subgraph from start node to all other nodes.
iam_table_s bfs_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end);

/// @brief Traverses the vertices of the specified structure using depth first search.
/// @param graph Structure to traverse.
/// @param cost Cost structure that defines the distance properties in table.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IAM_NIL' if all vertex paths.
/// @return Depth first search lookup table with subgraph from start node to all other nodes.
iam_table_s dfs_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end);

/// @brief Generate a Dijkstra lookup array table with nodes' edge sums and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IAM_NIL' if all vertex shortest path.
/// @param cost Cost structure that defines the distance properties in table.
/// @return Dijkstra lookup table with subgraph of shortest paths from start node to all other nodes.
iam_table_s dijkstra_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end);

/// @brief Generate an A* lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index.
/// @param cost Cost structure that defines the distance properties in table.
/// @param heuristic Function pointer to calculate heuristic cost based on two vectices.
/// @return A* lookup table with subgraph of shortest paths from start to end node.
iam_table_s a_star_iam_graph(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start, size_t const end, operate_fn const heuristic);

/// @brief Generate a Prim lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param cost Cost structure that defines the distance properties in table.
/// @return Prim lookup table with subgraph of minimum spanning tree from start node to all other nodes.
iam_table_s prim_iam_list(iam_graph_s const * const graph, iam_cost_s const * const cost, size_t const start);

/// @brief Generate a Kruskal lookup array table with nodes' ranks and previous indexes.
/// @param graph Structure to generate from.
/// @param sort Function pointer to sort special array of edge-vertices - 'iam_edge_s', for Kruskal's algorithm.
/// @param cost Cost structure that defines the distance properties in table.
/// @param as Arguments for sorting function.
/// @param inc Function pointer to increment rank cost by one.
/// @param ai Arguments for increment function pointer.
/// @return Kruskal lookup table with subgraph of minimum spanning tree from start node to all other nodes.
iam_table_s kruskal_iam_list(iam_graph_s const * const graph, iam_cost_s const * const cost, process_fn const sort, void * const as, set_fn const inc, void * const ai);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param table Structure to destroy.
void destroy_iam_list(iam_table_s * const table);

/// @brief Creates a subgraph copy of specified graph based on its table.
/// @param table Structure to reference.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param acv Arguments for copy vertex function pointer.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @param ace Arguments for copy edge function pointer.
/// @return Matrix graph structure.
/// @note The algorithm should be used for minimum spanning tree implementations like Prim and Kruskal,
/// but one can also generate trees from path finding implementations.
iam_graph_s subgraph_iam_list(iam_table_s const * const table, copy_fn const copy_vertex, void * const acv, copy_fn const copy_edge, void * const ace);

/// @brief Iterates over each vertex element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Arguments for handle function pointer.
void each_vertex_iam_graph(iam_graph_s const * const graph, handle_fn const handle, void * const ah);

/// @brief Iterates over each neighbor of a vertex index.
/// @param graph Structure to iterate.
/// @param index Index of vertex to iterate neighbors.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param ah Arguments for handle function pointer.
void each_neighbor_iam_graph(iam_graph_s const * const graph, size_t const index, handle_fn const handle, void * const ah);

/// @brief Iterates over each edge element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each edge-vertices (iam_edge_s) element reference using generic arguments.
/// @param ah Arguments for handle function pointer.
void each_edge_iam_graph(iam_graph_s const * const graph, handle_fn const handle, void * const ah);

/// @brief Iterates over each cost element in structure starting from the beginning.
/// @param table Structure to iterate.
/// @param handle Operate function pointer to operate on each vertex.
/// @param ah Arguments for handle function pointer.
void each_cost_iam_list(iam_table_s const * const table, handle_fn const handle, void * const ah);

/// @brief Traverses the costs paths of the specified structure using a generated table.
/// @param table Structure to traverse.
/// @param end End vertex index to recursively travel to from starting vertex.
/// @param handle Operate function pointer to operate on each vertex.
/// @param ah Arguments for handle function pointer.
/// @return 'true' if path exists, 'false' otherwise.
/// @note The algorithm should be used with shortest path find implementations, i.e. Dijkstra and A*,
/// to traverse the path from 'end' vertex to the start one, but one can also traverse MSP tables with it.
bool each_path_iam_list(iam_table_s const * const table, size_t const end, handle_fn const handle, void * const ah);

#endif // IAM_GRAPH_H
