#include <priority/ibinary_heap.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void _ibinary_heap_resize(ibinary_heap_s * heap, const size_t size);

ibinary_heap_s create_ibinary_heap(const size_t size, const compare_fn compare) {
    assert(compare && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    return (ibinary_heap_s) { .size = size, .compare = compare, };
}

void destroy_ibinary_heap(ibinary_heap_s * heap, const destroy_fn destroy) {
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size));
    }
    free(heap->elements);

    memset(heap, 0, sizeof((*heap)));
}

void clear_ibinary_heap(ibinary_heap_s * heap, const destroy_fn destroy) {
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size));
    }
    free(heap->elements);

    heap->length = heap->capacity = 0;
    heap->elements = NULL;
}

ibinary_heap_s copy_ibinary_heap(const ibinary_heap_s heap, const copy_fn copy) {
    const ibinary_heap_s replica = {
        .capacity = heap.capacity, .compare = heap.compare, .size = heap.size, .length = heap.length,
        .elements = malloc(heap.capacity * heap.size),
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");

    for (size_t i = 0; i < heap.length; ++i) {
        copy(replica.elements + (i * replica.size), heap.elements + (i * heap.size));
    }

    return replica;
}

bool is_empty_ibinary_heap(const ibinary_heap_s heap) {
    return !(heap.length);
}

void push_ibinary_heap(ibinary_heap_s * heap, const void * element) {
    if (heap->length == heap->capacity) {
        _ibinary_heap_resize(heap, heap->capacity + IBINARY_HEAP_CHUNK);
    }

    memcpy(heap->elements + (heap->length * heap->size), element, heap->size);

    // create temporary element holder outside of loop for swaps in heap fixup
    char * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    // while current top child index is not zero and current element is greater than its parent
    size_t c = heap->length, p = (c - 1) / 2;
    while (c && heap->compare(heap->elements + (c * heap->size), heap->elements + (p * heap->size)) < 0) {
        // swap current child element with parent
        memcpy(temporary, heap->elements + (c * heap->size), heap->size);
        memcpy(heap->elements + (c * heap->size), heap->elements + (p * heap->size), heap->size);
        memcpy(heap->elements + (p * heap->size), temporary, heap->size);

        // change current child to parent and parent to grand-parent
        c = p;
        p = (c - 1) / 2;
    }
    free(temporary);

    heap->length++;
}

void pop_ibinary_heap(ibinary_heap_s * heap, void * buffer) {
    assert(heap->length && "[ERROR] Length can't be zero.");

    // save root element
    memcpy(buffer, heap->elements, heap->size);
    heap->length--;

    if (heap->length == heap->capacity - IBINARY_HEAP_CHUNK) {
        _ibinary_heap_resize(heap, heap->length);
    }

    // put last element in first's place (memory may overlap)
    memmove(heap->elements, heap->elements + (heap->length * heap->size), heap->size);

    // create temporary element holder outside of loop for swaps in heap fixup
    char * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    for (size_t p = 0, c = (2 * p) + 1; c < heap->length; p = c, c = (2 * p) + 1) {
        const size_t s = c + 1;
        // if right child is a valid index and it is smaller than left child change left child to right one
        if (s < heap->length && heap->compare(heap->elements + (c * heap->size), heap->elements + (s * heap->size)) > 0) {
            c = s;
        }

        // if child is greater, then parent is properly set and thus break from loop
        if (heap->compare(heap->elements + (c * heap->size), heap->elements + (p * heap->size)) > 0) {
            break;
        }

        // swap current parent element with greatest child
        memcpy(temporary, heap->elements + (c * heap->size), heap->size);
        memcpy(heap->elements + (c * heap->size), heap->elements + (p * heap->size), heap->size);
        memcpy(heap->elements + (p * heap->size), temporary, heap->size);
    }
    free(temporary);
}

void peep_ibinary_heap(const ibinary_heap_s heap, void * buffer) {
    assert(heap.length && "[ERROR] Length can't be zero.");

    memcpy(buffer, heap.elements, heap.size);
}

void replace_ibinary_heap(const ibinary_heap_s heap, const size_t index, const void * element, void * buffer) {
    memcpy(buffer, heap.elements + (index * heap.size), heap.size);
    memcpy(heap.elements + (index * heap.size), element, heap.size);

    // create temporary element holder outside of loop for swaps in heap fixup
    char * temporary = malloc(heap.size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    if (heap.compare(buffer, element) > 0) {
        // while current top child index is not zero and current element is greater than its parent
        size_t c = index, p = (c - 1 / 2);
        while (c && heap.compare(heap.elements + (c * heap.size), heap.elements + (p * heap.size)) < 0) {
            // swap current child element with parent
            memcpy(temporary, heap.elements + (c * heap.size), heap.size);
            memcpy(heap.elements + (c * heap.size), heap.elements + (p * heap.size), heap.size);
            memcpy(heap.elements + (p * heap.size), temporary, heap.size);

            // change current child to parent and parent to grand-parent
            c = p;
            p = (c - 1) / 2;
        }
    } else {
        for (size_t p = 0, c = (2 * p) + 1; c < heap.length; p = c, c = (2 * p) + 1) {
            const size_t s = c + 1;
            // if right child is a valid index and it is smaller than left child change left child to right one
            if (s < heap.length && heap.compare(heap.elements + (c * heap.size), heap.elements + (s * heap.size)) > 0) {
                c = s;
            }

            // if child is greater, then parent is properly set and thus break from loop
            if (heap.compare(heap.elements + (c * heap.size), heap.elements + (p * heap.size)) > 0) {
                break;
            }

            // swap current parent element with greatest child
            memcpy(temporary, heap.elements + (c * heap.size), heap.size);
            memcpy(heap.elements + (c * heap.size), heap.elements + (p * heap.size), heap.size);
            memcpy(heap.elements + (p * heap.size), temporary, heap.size);
        }
    }
    free(temporary);
}

void meld_ibinary_heap(ibinary_heap_s * destination, ibinary_heap_s * source) {
    // calculate new destination length
    const size_t sum = destination->length + source->length;
    const size_t mod = sum % IBINARY_HEAP_CHUNK;
    _ibinary_heap_resize(destination, mod ? sum - mod + IBINARY_HEAP_CHUNK : sum);

    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    destination->length += source->length;

    free(source->elements);
    source->length = source->capacity = 0;
    source->elements = NULL;

    // create temporary element holder outside of loop for swaps in heap fixup
    char * temporary = malloc(destination->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    const size_t start = (destination->size / 2) - 1;
    for (size_t i = 0; i <= start; ++i) {
        const size_t reverse = start - i;
        for (size_t p = reverse, c = (2 * p) + 1; c < destination->size; p = c, c = (2 * p) + 1) {
            const size_t s = c + 1;
            // if right child is a valid index and it is smaller than left child change left child to right one
            if (s < destination->length && destination->compare(destination->elements + (c * destination->size), destination->elements + (s * destination->size)) > 0) {
                c = s;
            }

            // if child is greater, then parent is properly set and thus break from loop
            if (destination->compare(destination->elements + (c * destination->size), destination->elements + (p * destination->size)) > 0) {
                break;
            }

            // swap current parent element with greatest child
            memcpy(temporary, destination->elements + (c * destination->size), destination->size);
            memcpy(destination->elements + (c * destination->size), destination->elements + (p * destination->size), destination->size);
            memcpy(destination->elements + (p * destination->size), temporary, destination->size);
        }
    }
    free(temporary);
}

void patch_ibinary_heap(const ibinary_heap_s heap) {
    // create temporary element holder outside of loop for swaps in heap fixup
    char * temporary = malloc(heap.size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    const size_t start = (heap.size / 2) - 1;
    for (size_t i = 0; i <= start; ++i) {
        const size_t reverse = start - i;
        for (size_t p = reverse, c = (2 * p) + 1; c < heap.size; p = c, c = (2 * p) + 1) {
            const size_t s = c + 1;
            // if right child is a valid index and it is smaller than left child change left child to right one
            if (s < heap.length && heap.compare(heap.elements + (c * heap.size), heap.elements + (s * heap.size)) > 0) {
                c = s;
            }

            // if child is greater, then parent is properly set and thus break from loop
            if (heap.compare(heap.elements + (c * heap.size), heap.elements + (p * heap.size)) > 0) {
                break;
            }

            // swap current parent element with greatest child
            memcpy(temporary, heap.elements + (c * heap.size), heap.size);
            memcpy(heap.elements + (c * heap.size), heap.elements + (p * heap.size), heap.size);
            memcpy(heap.elements + (p * heap.size), temporary, heap.size);
        }
    }
    free(temporary);
}

void foreach_istack(const ibinary_heap_s heap, const operate_fn operate, void * arguments) {
    for (size_t i = 0; i < heap.length && operate(heap.elements + (i * heap.size), arguments); ++i) {}
}

void _ibinary_heap_resize(ibinary_heap_s * heap, const size_t size) {
    heap->capacity = size;

    heap->elements = realloc(heap->elements, heap->capacity * heap->size);
    assert((!heap->capacity || heap->elements) && "[ERROR] Memory allocation failed.");
}
