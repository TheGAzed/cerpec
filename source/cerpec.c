#include <cerpec.h>

#include <stdlib.h>

void * standard_alloc(const size_t size, void * arguments) {
    (void)(arguments);
    return malloc(size);
}

void standard_dealloc(void * pointer, void * arguments) {
    (void)(arguments);
    free(pointer);
}

void * standard_realloc(void * pointer, const size_t size, void * arguments) {
    (void)(arguments);
    return realloc(pointer, size);
}

memory_s standard_memory() {
    return (memory_s) {
        .alloc = standard_alloc,
        .free = standard_dealloc,
        .realloc = standard_realloc,

        .arguments = NULL,
    };
}

memory_s custom_memory(const alloc_fn alloc, const realloc_fn realloc, const free_fn free, void * arguments) {
    return (memory_s) {
        .alloc = alloc,
        .free = free,
        .realloc = realloc,

        .arguments = arguments,
    };
}
