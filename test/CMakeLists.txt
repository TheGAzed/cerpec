if (MSVC)
    add_compile_options(/W2 /WX /Z7)
else ()
    add_compile_options(-std=c99 -O0 -g -Wall -Wextra -Wconversion -Werror -Wpedantic)
endif ()

add_library(TEST test.c)
target_include_directories(TEST PUBLIC .)

add_subdirectory(sequence)
