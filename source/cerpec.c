#include <cerpec.h>

#include <stdlib.h>

void * _standard_alloc(const size_t size, void * arguments);
void _standard_dealloc(void * pointer, void * arguments);
void * _standard_realloc(void * pointer, const size_t size, void * arguments);

const memory_s standard = {
    .alloc = _standard_alloc,
    .free = _standard_dealloc,
    .realloc = _standard_realloc,
    .arguments = NULL,
};

void * _standard_alloc(const size_t size, void * arguments) {
    (void)(arguments);
    return malloc(size);
}

void _standard_dealloc(void * pointer, void * arguments) {
    (void)(arguments);
    free(pointer);
}

void * _standard_realloc(void * pointer, const size_t size, void * arguments) {
    (void)(arguments);
    return realloc(pointer, size);
}

memory_s compose_memory(const alloc_fn alloc, const realloc_fn realloc, const free_fn free, void * arguments) {
    return (memory_s) {
        .alloc = alloc,
        .free = free,
        .realloc = realloc,

        .arguments = arguments,
    };
}
