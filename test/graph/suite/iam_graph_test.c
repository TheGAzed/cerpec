#include <limits.h>

#include <graph/iam_graph.h>

#include <suite.h>

#define DIJKSTRA_01_SIZE 7
#define DIJKSTRA_02_SIZE 8
#define DIJKSTRA_03_SIZE 9

TEST CREATE_01(void) {
    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    ASSERT_NEQ(NULL, graph.allocator);
    ASSERT_NEQ(NULL, graph.compare);
    ASSERT_EQ(0, graph.capacity);
    ASSERT_NEQ(0, graph.edge_size);
    ASSERT_NEQ(0, graph.vertex_size);
    ASSERT_EQ(NULL, graph.edges);
    ASSERT_EQ(NULL, graph.vertices);
    ASSERT_NEQ(NULL, graph.none);

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST DESTROY_01(void) {
    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    ASSERT_EQ(NULL, graph.allocator);
    ASSERT_EQ(NULL, graph.compare);
    ASSERT_EQ(0, graph.capacity);
    ASSERT_EQ(0, graph.edge_size);
    ASSERT_EQ(0, graph.vertex_size);
    ASSERT_EQ(NULL, graph.edges);
    ASSERT_EQ(NULL, graph.vertices);
    ASSERT_EQ(NULL, graph.none);

    PASS();
}

TEST CLEAR_01(void) {
    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    clear_iam_graph(&graph, intdst, NULL, intdst, NULL);

    ASSERT_NEQ(NULL, graph.allocator);
    ASSERT_NEQ(NULL, graph.compare);
    ASSERT_EQ(0, graph.capacity);
    ASSERT_NEQ(0, graph.edge_size);
    ASSERT_NEQ(0, graph.vertex_size);
    ASSERT_EQ(NULL, graph.edges);
    ASSERT_EQ(NULL, graph.vertices);
    ASSERT_NEQ(NULL, graph.none);

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST COPY_01(void) {
    int  none = 0;
    int const one = 1;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < IAM_GRAPH_CHUNK - 1; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    for (int i = 0; i < IAM_GRAPH_CHUNK - 1; ++i) {
        for (int j = i + 1; j < IAM_GRAPH_CHUNK - 1; ++j) {
            insert_edge_iam_graph(&graph, (size_t)i, (size_t)j, &one);
        }
    }

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST COPY_02(void) {
    int  none = 0;
    int const one = 1;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < IAM_GRAPH_CHUNK; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    for (int i = 0; i < IAM_GRAPH_CHUNK; ++i) {
        for (int j = i + 1; j < IAM_GRAPH_CHUNK; ++j) {
            insert_edge_iam_graph(&graph, (size_t)i, (size_t)j, &one);
        }
    }

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST COPY_03(void) {
    int  none = 0;
    int const one = 1;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < IAM_GRAPH_CHUNK + 1; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    for (int i = 0; i < IAM_GRAPH_CHUNK + 1; ++i) {
        for (int j = i + 1; j < IAM_GRAPH_CHUNK + 1; ++j) {
            insert_edge_iam_graph(&graph, (size_t)i, (size_t)j, &one);
        }
    }

    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST DIJKSTRA_01(void) {
    // SOURCE: https://www.youtube.com/watch?v=CmIQ29cUGiE

    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < DIJKSTRA_01_SIZE; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    int const edges[] = { 2, 5, 3, 7, 1, 4, 3, 4, 1, 1, 3, };
    insert_edge_iam_graph(&graph, 0, 1, edges +  0);
    insert_edge_iam_graph(&graph, 0, 3, edges +  1);
    insert_edge_iam_graph(&graph, 0, 5, edges +  2);

    insert_edge_iam_graph(&graph, 1, 2, edges +  3);
    insert_edge_iam_graph(&graph, 1, 4, edges +  4);
    insert_edge_iam_graph(&graph, 1, 5, edges +  5);

    insert_edge_iam_graph(&graph, 2, 4, edges +  6);
    insert_edge_iam_graph(&graph, 2, 6, edges +  7);

    insert_edge_iam_graph(&graph, 3, 4, edges +  8);
    insert_edge_iam_graph(&graph, 3, 6, edges +  9);

    insert_edge_iam_graph(&graph, 4, 6, edges + 10);

    int zero = 0, inf = INT_MAX;
    const iam_cost_s cost = compose_iam_cost(sizeof(int), intcmp, NULL, intcpy, NULL, intsum, NULL, &zero, &inf);

    iam_table_s table = dijkstra_iam_graph(&graph, &cost, 0, IAM_SPECIAL);

    int const costs[DIJKSTRA_01_SIZE] = { 0, 2, 6, 4, 3, 3, 5 };
    ASSERT_MEM_EQ(costs, table.costs, DIJKSTRA_01_SIZE * sizeof(int));

    size_t const previous[DIJKSTRA_01_SIZE] = { IAM_SPECIAL, 0, 4, 4, 1, 0, 3 };
    ASSERT_MEM_EQ(previous, table.previous, DIJKSTRA_01_SIZE * sizeof(size_t));

    destroy_iam_table(&table);
    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST DIJKSTRA_02(void) {
    // SOURCE: https://www.youtube.com/watch?v=5GT5hYzjNoo

    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < DIJKSTRA_02_SIZE; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    int const edges[] = { 8, 2, 5, 2, 13, 2, 5, 1, 6, 3, 1, 2, 3, 6 };
    insert_edge_iam_graph(&graph, 0, 1, edges +  0);
    insert_edge_iam_graph(&graph, 0, 2, edges +  1);
    insert_edge_iam_graph(&graph, 0, 3, edges +  2);

    insert_edge_iam_graph(&graph, 1, 3, edges +  3);
    insert_edge_iam_graph(&graph, 1, 5, edges +  4);

    insert_edge_iam_graph(&graph, 2, 3, edges +  5);
    insert_edge_iam_graph(&graph, 2, 4, edges +  6);

    insert_edge_iam_graph(&graph, 3, 4, edges +  7);
    insert_edge_iam_graph(&graph, 3, 5, edges +  8);
    insert_edge_iam_graph(&graph, 3, 6, edges +  9);

    insert_edge_iam_graph(&graph, 4, 6, edges + 10);

    insert_edge_iam_graph(&graph, 5, 6, edges + 11);
    insert_edge_iam_graph(&graph, 5, 7, edges + 12);

    insert_edge_iam_graph(&graph, 6, 7, edges + 13);

    int zero = 0, inf = INT_MAX;
    const iam_cost_s cost = compose_iam_cost(sizeof(int), intcmp, NULL, intcpy, NULL, intsum, NULL, &zero, &inf);

    iam_table_s table = dijkstra_iam_graph(&graph, &cost, 0, IAM_SPECIAL);

    int const costs[DIJKSTRA_02_SIZE] = { 0, 6, 2, 4, 5, 8, 6, 11, };
    ASSERT_MEM_EQ(costs, table.costs, DIJKSTRA_02_SIZE * sizeof(int));

    size_t const previous[DIJKSTRA_02_SIZE] = { IAM_SPECIAL, 3, 0, 2, 3, 6, 4, 5, };
    ASSERT_MEM_EQ(previous, table.previous, DIJKSTRA_02_SIZE * sizeof(size_t));

    destroy_iam_table(&table);
    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

TEST DIJKSTRA_03(void) {
    // SOURCE: https://www.youtube.com/watch?v=5GT5hYzjNoo

    int none = 0;
    iam_graph_s graph = create_iam_graph(sizeof(int), sizeof(int), intcmp, NULL, &none);

    for (int i = 0; i < DIJKSTRA_03_SIZE; ++i) {
        insert_vertex_iam_graph(&graph, &i);
    }

    int const edges[] = { 4, 8, 8, 11, 7, 4, 2, 9, 14, 10, 2, 1, 6, 7 };
    insert_edge_iam_graph(&graph, 0, 1, edges +  0);
    insert_edge_iam_graph(&graph, 0, 7, edges +  1);

    insert_edge_iam_graph(&graph, 1, 2, edges +  2);
    insert_edge_iam_graph(&graph, 1, 7, edges +  3);

    insert_edge_iam_graph(&graph, 2, 3, edges +  4);
    insert_edge_iam_graph(&graph, 2, 5, edges +  5);
    insert_edge_iam_graph(&graph, 2, 8, edges +  6);

    insert_edge_iam_graph(&graph, 3, 4, edges +  7);
    insert_edge_iam_graph(&graph, 3, 5, edges +  8);

    insert_edge_iam_graph(&graph, 4, 5, edges +  9);

    insert_edge_iam_graph(&graph, 5, 6, edges + 10);

    insert_edge_iam_graph(&graph, 6, 7, edges + 11);
    insert_edge_iam_graph(&graph, 6, 8, edges + 12);

    insert_edge_iam_graph(&graph, 7, 8, edges + 13);

    int zero = 0, inf = INT_MAX;
    const iam_cost_s cost = compose_iam_cost(sizeof(int), intcmp, NULL, intcpy, NULL, intsum, NULL, &zero, &inf);

    iam_table_s table = dijkstra_iam_graph(&graph, &cost, 0, IAM_SPECIAL);

    int const costs[DIJKSTRA_03_SIZE] = { 0, 4, 12, 19, 21, 11, 9, 8, 14, };
    ASSERT_MEM_EQ(costs, table.costs, DIJKSTRA_03_SIZE * sizeof(int));

    size_t const previous[DIJKSTRA_03_SIZE] = { IAM_SPECIAL, 0, 1, 2, 5, 6, 7, 0, 2, };
    ASSERT_MEM_EQ(previous, table.previous, DIJKSTRA_03_SIZE * sizeof(size_t));

    destroy_iam_table(&table);
    destroy_iam_graph(&graph, intdst, NULL, intdst, NULL);

    PASS();
}

SUITE (iam_graph_test) {
    RUN_TEST(CREATE_01); RUN_TEST(DESTROY_01); RUN_TEST(CLEAR_01);
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03);
    RUN_TEST(DIJKSTRA_01); RUN_TEST(DIJKSTRA_02); RUN_TEST(DIJKSTRA_03);
}
