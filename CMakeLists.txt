cmake_minimum_required(VERSION 3.22.1)

project(cerpec C)

add_subdirectory(source)
add_subdirectory(include)

option(CERPEC_TEST "Include cerpec unit tests." ON)

if (CERPEC_TEST)
    add_subdirectory(external)

    include(CTest)
    enable_testing()
    add_subdirectory(test)

    if (EXISTS "${CMAKE_SOURCE_DIR}/experiment")
        add_subdirectory(experiment)
    endif ()
endif ()
