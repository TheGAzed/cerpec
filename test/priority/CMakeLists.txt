add_executable(priority_test main.c
        suite/ibinary_heap_test.c
)

target_include_directories(priority_test PUBLIC suite)
target_link_libraries(priority_test PRIVATE GREATEST TEST CERPEC)
add_test(NAME PRIORITY_TEST COMMAND priority_test)
