#include <misc/ibinary_heap.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/// @brief Resizes structure to new size.
/// @param heap Strcuture to resize.
/// @param size New size to be used.
void _ibinary_heap_resize(ibinary_heap_s * heap, const size_t size);

/// @brief Performs heapify up operation on heap.
/// @param heap Heap to heapify.
/// @param index Start index to perform heapify.
/// @param temporary Temporary single size element used in swaps to avoid pointless memory allocation.
void _ibinary_heapify_up(const ibinary_heap_s * heap, const size_t index, void * temporary);

/// @brief Performs heapify down operation on heap.
/// @param heap Heap to heapify.
/// @param index Start index to perform heapify.
/// @param temporary Temporary single size element used in swaps to avoid pointless memory allocation.
void _ibinary_heapify_down(const ibinary_heap_s * heap, const size_t index, void * temporary);

ibinary_heap_s create_ibinary_heap(const size_t size, const compare_fn compare) {
    assert(compare && "[ERROR] Parameter can't be NULL.");
    assert(size && "[ERROR] Parameter can't be zero.");

    return (ibinary_heap_s) { .size = size, .compare = compare, };
}

void destroy_ibinary_heap(ibinary_heap_s * heap, const set_fn destroy) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // for each element in heap array call destroy function
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size));
    }
    free(heap->elements);

    // make structure invalid
    memset(heap, 0, sizeof((*heap)));
}

void clear_ibinary_heap(ibinary_heap_s * heap, const set_fn destroy) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(destroy && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // for each element in heap array call destroy function
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size));
    }
    free(heap->elements);

    // only clear structure
    heap->length = heap->capacity = 0;
    heap->elements = NULL;
}

ibinary_heap_s copy_ibinary_heap(const ibinary_heap_s * heap, const copy_fn copy) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(copy && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // initialize replica structure
    const ibinary_heap_s replica = {
        .capacity = heap->capacity, .compare = heap->compare, .size = heap->size, .length = heap->length,
        .elements = malloc(heap->capacity * heap->size),
    };
    assert((!replica.capacity || replica.elements) && "[ERROR] Memory allocation failed.");

    // copy each array element into replica
    for (size_t i = 0; i < heap->length; ++i) {
        copy(replica.elements + (i * replica.size), heap->elements + (i * heap->size));
    }

    return replica;
}

bool is_empty_ibinary_heap(const ibinary_heap_s * heap) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    return !(heap->length);
}

void push_ibinary_heap(ibinary_heap_s * heap, const void * element) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    if (heap->length == heap->capacity) {
        _ibinary_heap_resize(heap, heap->length + IBINARY_HEAP_CHUNK);
    }

    // append element to the end of the structure
    memcpy(heap->elements + (heap->length * heap->size), element, heap->size);

    // create temporary element holder outside of loop for swaps in heap fixup
    void * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    _ibinary_heapify_up(heap, heap->length, temporary);

    free(temporary);

    heap->length++;
}

void pop_ibinary_heap(ibinary_heap_s * heap, void * buffer) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(heap->length && "[ERROR] Length can't be zero.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // save root element
    memcpy(buffer, heap->elements, heap->size);
    heap->length--;

    // put last element in first's place (memory may overlap)
    memmove(heap->elements, heap->elements + (heap->length * heap->size), heap->size);

    // create temporary element holder outside of loop for swaps in heap fixup
    void * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    _ibinary_heapify_down(heap, 0, temporary);

    if (heap->length == heap->capacity - IBINARY_HEAP_CHUNK) {
        _ibinary_heap_resize(heap, heap->length);
    }

    free(temporary);
}

void peep_ibinary_heap(const ibinary_heap_s * heap, void * buffer) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(heap->length && "[ERROR] Length can't be zero.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    memcpy(buffer, heap->elements, heap->size);
}

void replace_ibinary_heap(const ibinary_heap_s * heap, const size_t index, const void * element, void * buffer) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(buffer && "[ERROR] Parameter can't be NULL.");
    assert(element && "[ERROR] Parameter can't be NULL.");
    assert(index < heap->length && "[ERROR] Parameter can't be more than length.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // copy element to replace into buffer
    memcpy(buffer, heap->elements + (index * heap->size), heap->size);
    // place parameter element into removed element's place
    memcpy(heap->elements + (index * heap->size), element, heap->size);

    // create temporary element holder outside of loop for swaps in heap fixup
    void * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    // perform heapify up or down based on comparison (ignore comparison equal to 'zero' as heap doesn't change)
    const int comparison = heap->compare(buffer, element);
    if (comparison > 0) {
        _ibinary_heapify_up(heap, index, temporary);
    } else if (comparison < 0) {
        _ibinary_heapify_down(heap, index, temporary);
    }

    free(temporary);
}

void meld_ibinary_heap(ibinary_heap_s * restrict destination, ibinary_heap_s * restrict source) {
    assert(destination && "[ERROR] Parameter can't be NULL.");
    assert(source && "[ERROR] Parameter can't be NULL.");
    assert(source != destination && "[ERROR] Parameter can't be equal.");

    assert(destination->compare && "[INVALID] Parameter can't be NULL.");
    assert(destination->size && "[INVALID] Parameter can't be zero.");

    assert(source->compare && "[INVALID] Parameter can't be NULL.");
    assert(source->size && "[INVALID] Parameter can't be zero.");

    // calculate new destination length
    const size_t sum = destination->length + source->length;
    const size_t mod = sum % IBINARY_HEAP_CHUNK;
    _ibinary_heap_resize(destination, mod ? sum - mod + IBINARY_HEAP_CHUNK : sum);

    // copy source elements into destination array
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    destination->length += source->length;

    // clear (NOT DESTROY) source elements array
    free(source->elements);
    source->length = source->capacity = 0;
    source->elements = NULL;

    // create temporary element holder outside of loop for swaps in heap fixup
    void * temporary = malloc(destination->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    // for each element at half index call
    const size_t start = (destination->length / 2) - 1; // value may underflow when length is less than two
    for (size_t i = 0; destination->length > 1 && i <= start; ++i) {
        const size_t reverse = start - i;
        _ibinary_heapify_down(destination, reverse, temporary);
    }

    free(temporary);
}

void patch_ibinary_heap(const ibinary_heap_s * heap) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    // create temporary element holder outside of loop for swaps in heap fixup
    void * temporary = malloc(heap->size);
    assert(temporary && "[ERROR] Memory allocation failed.");

    // for each element at half index
    const size_t start = (heap->length / 2) - 1; // value may underflow when length is less than two
    for (size_t i = 0; heap->length > 1 && i <= start; ++i) {
        const size_t reverse = start - i;
        _ibinary_heapify_down(heap, reverse, temporary);
    }

    free(temporary);
}

void map_ibinary_heap(const ibinary_heap_s * heap, const handle_fn handle, void * arguments) {
    assert(heap && "[ERROR] Parameter can't be NULL.");
    assert(handle && "[ERROR] Parameter can't be NULL.");

    assert(heap->compare && "[INVALID] Parameter can't be NULL.");
    assert(heap->size && "[INVALID] Parameter can't be zero.");

    for (size_t i = 0; i < heap->length && handle(heap->elements + (i * heap->size), arguments); ++i) {}
}

void _ibinary_heap_resize(ibinary_heap_s * heap, const size_t size) {
    heap->capacity = size;

    heap->elements = realloc(heap->elements, size * heap->size);
    assert((!size || heap->elements) && "[ERROR] Memory allocation failed.");
}

void _ibinary_heapify_up(const ibinary_heap_s * heap, const size_t index, void * temporary) {
    // while current top child index is not zero and current element is greater than its parent
    size_t c = index;
    size_t p = (c - 1) / 2;

    void * child = heap->elements + (c * heap->size);
    void * parent = heap->elements + (p * heap->size);
    while (c && heap->compare(child, parent) < 0) {
        // swap current child element with parent
        memcpy(temporary, child, heap->size);
        memmove(child, parent, heap->size);
        memcpy(parent, temporary, heap->size);

        // change current child to parent and parent to grand-parent
        c = p;
        p = (c - 1) / 2;
        child = heap->elements + (c * heap->size);
        parent = heap->elements + (p * heap->size);
    }
}

void _ibinary_heapify_down(const ibinary_heap_s * heap, const size_t index, void * temporary) {
    for (size_t p = index, c = (2 * p) + 1; c < heap->length; p = c, c = (2 * p) + 1) {
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
        memmove(heap->elements + (c * heap->size), heap->elements + (p * heap->size), heap->size);
        memcpy(heap->elements + (p * heap->size), temporary, heap->size);
    }
}
