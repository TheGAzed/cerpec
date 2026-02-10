#include <cerpec.h>

#include <stdlib.h>
#include <assert.h>

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
    error(alloc && "Paremeter can't be NULL.");
    error(realloc && "Paremeter can't be NULL.");
    error(free && "Paremeter can't be NULL.");

    return (memory_s) { .alloc = alloc, .free = free, .realloc = realloc, .arguments = arguments, };
}

bool is_valid_uniter(uniter_s const * const iterator) {
    error(iterator && "Parameter can't be NULL.");
    error(iterator->meta != INVALID_ITERATOR && "Metadata can't be invalid.");

    return iterator->index != INVALID_ITERATOR;
}

bool is_valid_biter(biter_s const * const iterator) {
    error(iterator && "Parameter can't be NULL.");
    error(iterator->meta != INVALID_ITERATOR && "Metadata can't be invalid.");

    return iterator->index != INVALID_ITERATOR;
}
