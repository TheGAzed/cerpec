cmake_minimum_required(VERSION 3.22.1)

project(CERPEC)
add_library(${PROJECT_NAME})

add_subdirectory(include)
add_subdirectory(source)

if (NOT DEFINED IS_CERPEC_TESTING)
    set(IS_CERPEC_TESTING TRUE)
endif ()

if (IS_CERPEC_TESTING)
    add_subdirectory(external)

    include(CTest)
    enable_testing()
    add_subdirectory(test)

    if (EXISTS "${CMAKE_SOURCE_DIR}/experiment")
        add_subdirectory(experiment)
    endif ()
endif ()