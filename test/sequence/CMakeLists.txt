add_executable(sequence_test main.c
        suite/stack_test.c
        suite/queue_test.c
        suite/ideque_test.c
)

target_include_directories(sequence_test PUBLIC suite)
target_link_libraries(sequence_test PRIVATE GREATEST TEST CERPEC)
add_test(NAME SEQUENCE_TEST COMMAND sequence_test)
