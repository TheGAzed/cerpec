#ifndef IMATRIX_GRAPH_H
#define IMATRIX_GRAPH_H

#include <cerpec.h>

#if !defined(IMATRIX_GRAPH_CHUNK)
#   define IMATRIX_GRAPH_CHUNK CERPEC_CHUNK
#endif

#define IMATRIX_NIL ((size_t)(-1))

/// @brief Stack data structure.
typedef struct infinite_matrix_graph {
    char * vertices; // array of elements
    char * edges;    // array of elements
    void * none;     // non-edge
    compare_fn compare; // comapres edges and determines if they're smaller, bigger or equal
    size_t vertex_size, edge_size, length, capacity; // size of single element, structure length and its capacity
} imatrix_graph_s;

typedef struct infinite_matrix_graph_edge {
    char * edge; // edge first to simplify weight comparison by dereferencing it directly to edge type
    size_t index[2];
} imatrix_edge_s;

typedef struct infinite_matrix_graph_pair {
    char * edge; // edge first to simplify weight comparison by dereferencing it directly to edge type
    char * vertex[2];
} imatrix_pair_s;

typedef struct infinite_matrix_graph_table {
    size_t * previous;
    char * edges;
} imatrix_table_s;

/// @brief Creates an empty structure.
/// @param vertex_size Size of a single vertex element.
/// @param edge_size Size of a single edge element.
/// @param compare Compare function for edge elements.
/// @param none Non-edge element to represent absence of an edge.
/// @return Graph structure.
/// @note Compare function must return equal if a 'none' edge is compared with 'none' parameter.
imatrix_graph_s create_imatrix_graph(const size_t vertex_size, const size_t edge_size, const compare_fn compare, void * none);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param destroy_edge Function pointer to destroy a single edge element.
void destroy_imatrix_graph(imatrix_graph_s * graph, const destroy_fn destroy_vertex, const destroy_fn destroy_edge);

/// @brief Clears a structure, and destroys its elements, but remains usable.
/// @param graph Structure to destroy.
/// @param destroy_vertex Function pointer to destroy a single vertex element.
/// @param destroy_edge Function pointer to destroy a single edge element.
void clear_imatrix_graph(imatrix_graph_s * graph, const destroy_fn destroy_vertex, const destroy_fn destroy_edge);

/// @brief Creates a copy of a structure and all its elements.
/// @param graph Structure to copy.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @return Matrix graph structure.
imatrix_graph_s copy_imatrix_graph(const imatrix_graph_s * graph, const copy_fn copy_vertex, const copy_fn copy_edge);

/// @brief Checks if structure is empty.
/// @param graph Structure to check.
/// @return 'true' if empty, 'false' if not.
bool is_empty_imatrix_graph(const imatrix_graph_s * graph);

/// @brief Inserts a single vertex element into the structure.
/// @param graph Structure to insert into.
/// @param vertex Element buffer to insert.
/// @return Index of element in vertex array.
size_t insert_vertex_imatrix_graph(imatrix_graph_s * graph, const void * vertex);

/// @brief Remove specified vertex element at index from structure.
/// @param graph Structure to remove from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
/// @param destroy_edge Function pointer to destroy single edge element associated wtih removed vertex.
/// @return Index of last swapped element in vertex array or 'IGRAPH_NIL', if removed element was last.
/// @note The last vertex in elements array (including its edges) gets swapped with the removed index vertex.
size_t remove_vertex_imatrix_graph(imatrix_graph_s * graph, const size_t index, void * buffer, const destroy_fn destroy_edge);

/// @brief Gets the vertex element at index in structure.
/// @param graph Structure to get from.
/// @param index Index location of vertex.
/// @param buffer Element buffer to save vertex.
void get_vertex_imatrix_graph(const imatrix_graph_s * graph, const size_t index, void * buffer);

/// @brief Inserts a single edge element into the structure.
/// @param graph Structure to insert into.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param edge Element buffer to insert.
/// @return Index of element in vertex array.
void insert_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, const void * edge);

/// @brief Remove specified edge element at vertex indices from structure.
/// @param graph Structure to remove from.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @param buffer Element buffer to insert.
void remove_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer);

/// @brief Checks if edge exists between two vertives.
/// @param graph Structure to check.
/// @param index_one Index location of first vertex.
/// @param index_two Index location of second vertex.
/// @return 'true' if an edge exists, 'false' otherwise.
/// @note Existance is dependent on 'none' structure parameter.
bool contains_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two);

/// @brief Gets the edge element at vertex indices in structure.
/// @param graph Structure to get from.
/// @param index_one First vertex index of edge.
/// @param index_two Second vertex index of edge.
/// @param buffer Element buffer to save edge.
void get_edge_imatrix_graph(const imatrix_graph_s * graph, const size_t index_one, const size_t index_two, void * buffer);

/// @brief Traverses the vertices of the specified structure using breadth first search.
/// @param graph Structure to traverse.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_GRAPH_NIL' if all vertex shortest path.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element for table.
/// @return Breadth first search lookup table with subgraph from start node to all other nodes.
imatrix_table_s breadth_first_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn copy_edge);

/// @brief Traverses the vertices of the specified structure using depth first search.
/// @param graph Structure to traverse.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_GRAPH_NIL' if all vertex shortest path.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element for table.
/// @return Depth first search lookup table with subgraph from start node to all other nodes.
imatrix_table_s depth_first_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn copy_edge);

/// @brief Generate a Dijkstra lookup array table with nodes' edge sums and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index, or 'IMATRIX_GRAPH_NIL' if all vertex shortest path.
/// @param add_edge Function pointer to add two edges' weights into destination edge.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @return Dijkstra lookup table with subgraph of shortest paths from start node to all other nodes.
imatrix_table_s dijkstra_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const copy_fn add_edge, const destroy_fn destroy_edge);

/// @brief Generate a Bellman-Ford lookup array table with nodes' edge sums and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param add_edge Function pointer to add two edges' weights into destination edge.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @return Bellman-Ford lookup table with subgraph of shortest paths from start node to all other nodes.
imatrix_table_s bellman_ford_imatrix_table(const imatrix_graph_s * graph, const size_t start, const copy_fn add_edge, const destroy_fn destroy_edge);

/// @brief Generate a Prim lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element for table.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @return Prim lookup table with subgraph of minimum spanning tree from start node to all other nodes.
imatrix_table_s prim_imatrix_table(const imatrix_graph_s * graph, const size_t start, const copy_fn copy_edge, const destroy_fn destroy_edge);

/// @brief Generate a Kruskal lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param sort Function pointer to sort special array of edges and vertices - 'imatrix_graph_edge', for Kruskal's algorithm.
/// @param sort_args Arguments for sorting function.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element for table.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @return Kruskal lookup table with subgraph of minimum spanning tree from start node to all other nodes.
imatrix_table_s kruskal_imatrix_table(const imatrix_graph_s * graph, const process_fn sort, void * sort_args, const copy_fn copy_edge, const destroy_fn destroy_edge);

/// @brief Generate an A* lookup array table with nodes' edges and previous indexes.
/// @param graph Structure to generate from.
/// @param start Starting vertex index.
/// @param end Last vertex index.
/// @param add_edge Function pointer to add two edges' weights into destination edge.
/// @param destroy_edge Function pointer to destroy a single edge element.
/// @param heuristic Function pointer to determine heuristic 'edge weight' based on two vectices.
/// @return A* lookup table with subgraph of shortest paths from start to end node.
imatrix_table_s a_star_imatrix_table(const imatrix_graph_s * graph, const size_t start, const size_t end, const operate_fn add_edge, const operate_fn heuristic, const destroy_fn destroy_edge);

/// @brief Traverses the vertices of the specified structure using a generated table.
/// @param graph Structure to traverse.
/// @param table Structure to reference.
/// @param start Starting vertex index.
/// @param operate Operate function pointer to operate on each vertex.
/// @param arguments Arguments for operate function pointer.
void search_imatrix_table(const imatrix_graph_s * graph, const imatrix_table_s * table, const size_t start, const handle_fn operate, void * arguments);

/// @brief Destroys a structure, and its elements and makes it unusable.
/// @param graph Structure to reference.
/// @param table Structure to destroy.
/// @param destroy_edge Function pointer to destroy a single edge element.
void destroy_imatrix_table(const imatrix_graph_s * graph, imatrix_table_s * table, const destroy_fn destroy_edge);

/// @brief Creates a subgraph copy of specified graph based on its table.
/// @param graph Structure to copy.
/// @param table Structure to reference.
/// @param copy_vertex Function pointer to create a deep/shallow copy of a single vertex element.
/// @param copy_edge Function pointer to create a deep/shallow copy of a single edge element.
/// @return Matrix graph structure.
imatrix_graph_s subgraph_imatrix_table(const imatrix_graph_s * graph, const imatrix_table_s * table, const copy_fn copy_vertex, const copy_fn copy_edge);

/// @brief Iterates over each vertex element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Arguments for operate function pointer.
void map_vertex_imatrix_graph(const imatrix_graph_s * graph, const handle_fn handle, void * arguments);

/// @brief Iterates over each edge element in structure starting from the beginning.
/// @param graph Structure to iterate.
/// @param handle Function pointer to handle each element reference using generic arguments.
/// @param arguments Arguments for operate function pointer.
void map_edge_imatrix_graph(const imatrix_graph_s * graph, const handle_fn handle, void * arguments);

#endif // IMATRIX_GRAPH_H
