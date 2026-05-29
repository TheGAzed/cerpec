add_executable(misc_test main.c
        suite/ibinary_heap_test.c
        suite/fbinary_heap_test.c
        suite/isc_hash_map_test.c
        suite/fsc_hash_map_test.c
)

target_include_directories(misc_test PUBLIC suite)
target_link_libraries(misc_test PRIVATE GREATEST CERPEC)
add_test(NAME MISC_TEST COMMAND misc_test)
