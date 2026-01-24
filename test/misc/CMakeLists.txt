add_executable(misc_test main.c
        suite/ibinary_heap_test.c
        suite/fbinary_heap_test.c
        suite/ihash_map_test.c
        suite/fhash_table_test.c
)

target_include_directories(misc_test PUBLIC suite)
target_link_libraries(misc_test PRIVATE GREATEST TEST CERPEC)
add_test(NAME MISC_TEST COMMAND misc_test)
