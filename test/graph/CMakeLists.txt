add_executable(graph_test main.c
        suite/iam_graph_test.c
)

target_include_directories(graph_test PUBLIC suite)
target_link_libraries(graph_test PRIVATE greatest ${PROJECT_NAME})
add_test(NAME GRAPH_TEST COMMAND graph_test)
