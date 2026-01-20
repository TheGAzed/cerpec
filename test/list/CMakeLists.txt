add_executable(list_test main.c
        suite/istraight_list_test.c
        suite/icircular_list_test.c
        suite/idouble_list_test.c

        suite/fstraight_list_test.c
        suite/fcircular_list_test.c
        suite/fdouble_list_test.c
)

target_include_directories(list_test PUBLIC suite)
target_link_libraries(list_test PRIVATE GREATEST TEST CERPEC)
add_test(NAME LIST_TEST COMMAND list_test)
