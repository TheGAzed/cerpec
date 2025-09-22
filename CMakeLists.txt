cmake_minimum_required(VERSION 3.22.1)

project(CERPEC)
add_library(CERPEC)

add_subdirectory(include)
add_subdirectory(source)

option(CERPEC_INCLUDE_TEST "Include cerpec unit tests." ON)

if (CERPEC_INCLUDE_TEST)
    add_subdirectory(external)

    include(CTest)
    enable_testing()
    add_subdirectory(test)

    if (EXISTS "${CMAKE_SOURCE_DIR}/experiment")
        add_subdirectory(experiment)
    endif ()
endif ()