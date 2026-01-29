add_executable(tree_test main.c
        suite/ibsearch_tree_test.c
        suite/iavl_tree_test.c
        suite/irb_tree_test.c
        suite/fbsearch_tree_test.c
)

target_include_directories(tree_test PUBLIC suite)
target_link_libraries(tree_test PRIVATE GREATEST TEST CERPEC)
add_test(NAME TREE_TEST COMMAND tree_test)
