#include <cerpec.h>

#include <stdlib.h>
#include <assert.h>

void * _standard_alloc(const size_t size, void * arg);
void _standard_dealloc(void * pointer, void * arg);
void * _standard_realloc(void * pointer, const size_t size, void * arg);

const memory_s standard = {
    .alloc = _standard_alloc,
    .free = _standard_dealloc,
    .realloc = _standard_realloc,
    .arg = NULL,
};

void * _standard_alloc(const size_t size, void * arg) {
    (void)(arg);
    return malloc(size);
}

void _standard_dealloc(void * pointer, void * arg) {
    (void)(arg);
    free(pointer);
}

void * _standard_realloc(void * pointer, const size_t size, void * arg) {
    (void)(arg);
    return realloc(pointer, size);
}

memory_s compose_memory(const alloc_fn alloc, const realloc_fn realloc, const free_fn free, void * arg) {
    error(alloc && "Paremeter can't be NULL.");
    error(realloc && "Paremeter can't be NULL.");
    error(free && "Paremeter can't be NULL.");

    return (memory_s) { .alloc = alloc, .free = free, .realloc = realloc, .arg = arg, };
}
