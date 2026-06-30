add_executable(set_test main.c
        suite/isc_hash_set_test.c
        suite/ibitwise_set_test.c
        suite/irb_set_test.c
        suite/fbitwise_set_test.c
        suite/fsc_hash_set_test.c
        suite/frb_set_test.c
)

target_include_directories(set_test PUBLIC suite)
target_link_libraries(set_test PRIVATE greatest ${PROJECT_NAME})
add_test(NAME SET_TEST COMMAND set_test)
