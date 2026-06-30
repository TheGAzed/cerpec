#include <misc/ibinary_heap.h>

#include <assert.h>
#include <string.h>

/// @brief Resizes structure to new size.
/// @param heap Strcuture to resize.
/// @param size New size to be used.
void _ibinary_heap_resize(ibinary_heap_s * const heap, size_t const size);

/// @brief Performs heapify up operation on heap.
/// @param heap Heap to heapify.
/// @param index Start index to perform heapify.
void _ibinary_heapify_up(ibinary_heap_s const * const heap, size_t const index);

/// @brief Performs heapify down operation on heap.
/// @param heap Heap to heapify.
/// @param index Start index to perform heapify.
void _ibinary_heapify_down(ibinary_heap_s const * const heap, size_t const index);

/// @brief Swaps two elements based on their size.
/// @param a First element to swap.
/// @param b second element to swap
/// @param size Size of singular element.
void _ibinary_heap_swap(void * const a, void * const b, size_t const size);

ibinary_heap_s create_ibinary_heap(size_t const size, compare_fn const compare, void * const ac) {
    assert(compare && "Parameter can't be NULL.");
    assert(size && "Parameter can't be zero.");

    return (ibinary_heap_s) {
        .size = size, .compare = compare, .allocator = &standard, .ac = ac,
    };
}

ibinary_heap_s make_ibinary_heap(size_t const size, compare_fn const compare, void * const ac, memory_s const * const allocator) {
    assert(compare && "Parameter can't be NULL.");
    assert(size && "Parameter can't be zero.");
    assert(allocator && "Parameter can't be NULL.");

    return (ibinary_heap_s) {
        .size = size, .compare = compare, .allocator = allocator, .ac = ac,
    };
}

void destroy_ibinary_heap(ibinary_heap_s * const heap, set_fn const destroy, void * const ad) {
    assert(heap && "Parameter can't be NULL.");
    assert(destroy && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    // for each element in heap array call destroy function
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size), ad);
    }
    heap->allocator->free(heap->elements, heap->allocator->arg);

    // make structure invalid
    memset(heap, 0, sizeof(ibinary_heap_s));
}

void clear_ibinary_heap(ibinary_heap_s * const heap, set_fn const destroy, void * const ad) {
    assert(heap && "Parameter can't be NULL.");
    assert(destroy && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    // for each element in heap array call destroy function
    for (size_t i = 0; i < heap->length; ++i) {
        destroy(heap->elements + (i * heap->size), ad);
    }
    heap->allocator->free(heap->elements, heap->allocator->arg);

    // only clear structure
    heap->length = heap->capacity = 0;
    heap->elements = NULL;
}

ibinary_heap_s copy_ibinary_heap(ibinary_heap_s const * const heap, copy_fn const copy, void * const ac) {
    assert(heap && "Parameter can't be NULL.");
    assert(copy && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    // initialize replica structure
    ibinary_heap_s const replica = {
        .capacity = heap->capacity, .compare = heap->compare, .size = heap->size, .length = heap->length,
        .elements = heap->allocator->alloc(heap->capacity * heap->size, heap->allocator->arg),
        .allocator = heap->allocator, .ac = heap->ac,
    };
    assert((!replica.capacity || replica.elements) && "Memory allocation failed.");

    // copy each array element into replica
    for (size_t i = 0; i < heap->length; ++i) {
        copy(replica.elements + (i * replica.size), heap->elements + (i * heap->size), ac);
    }

    return replica;
}

bool is_empty_ibinary_heap(ibinary_heap_s const * const heap) {
    assert(heap && "Parameter can't be NULL.");
    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    return !(heap->length);
}

void push_ibinary_heap(ibinary_heap_s * const heap, void const * const element) {
    assert(heap && "Parameter can't be NULL.");
    assert(element && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    if (heap->length == heap->capacity) {
        size_t const capacity = heap->length ? heap->length * CERPEC_FACTOR : IBINARY_HEAP_CHUNK;
        _ibinary_heap_resize(heap, capacity);
    }

    // append element to the end of the structure
    memcpy(heap->elements + (heap->length * heap->size), element, heap->size);

    _ibinary_heapify_up(heap, heap->length);
    heap->length++;
}

void pop_ibinary_heap(ibinary_heap_s * const heap, void * const buffer) {
    assert(heap && "Parameter can't be NULL.");
    assert(buffer && "Parameter can't be NULL.");
    assert(heap->length && "Length can't be zero.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    // save root element
    memcpy(buffer, heap->elements, heap->size);
    heap->length--;

    // put last element in first's place (memory may overlap)
    memmove(heap->elements, heap->elements + (heap->length * heap->size), heap->size);

    _ibinary_heapify_down(heap, 0);

    if (heap->length <= heap->capacity / CERPEC_FACTOR && (heap->length > IBINARY_HEAP_CHUNK || !heap->length)) {
        _ibinary_heap_resize(heap, heap->length);
    }
}

void peep_ibinary_heap(ibinary_heap_s const * const heap, void * const buffer) {
    assert(heap && "Parameter can't be NULL.");
    assert(heap->length && "Length can't be zero.");
    assert(buffer && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    memcpy(buffer, heap->elements, heap->size);
}

void replace_ibinary_heap(ibinary_heap_s const * const heap, size_t const index, void const * const element, void * const buffer) {
    assert(heap && "Parameter can't be NULL.");
    assert(buffer && "Parameter can't be NULL.");
    assert(element && "Parameter can't be NULL.");
    assert(index < heap->length && "Parameter can't be more than length.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    // copy element to replace into buffer
    memcpy(buffer, heap->elements + (index * heap->size), heap->size);
    // place parameter element into removed element's place
    memcpy(heap->elements + (index * heap->size), element, heap->size);

    // perform heapify up or down based on comparison (ignore comparison equal to 'zero' as heap doesn't change)
    int const comparison = heap->compare(buffer, element, heap->ac);
    if (comparison > 0) {
        _ibinary_heapify_up(heap, index);
    } else if (comparison < 0) {
        _ibinary_heapify_down(heap, index);
    }
}

void meld_ibinary_heap(ibinary_heap_s * const destination, ibinary_heap_s * const source) {
    assert(destination && "Parameter can't be NULL.");
    assert(source && "Parameter can't be NULL.");
    assert(source != destination && "Parameter can't be equal.");

    assert(destination->compare && "Parameter can't be NULL.");
    assert(destination->size && "Parameter can't be zero.");
    assert(destination->allocator && "Paremeter can't be NULL.");

    assert(source->compare && "Parameter can't be NULL.");
    assert(source->size && "Parameter can't be zero.");
    assert(source->allocator && "Paremeter can't be NULL.");

    // calculate new destination length
    size_t const sum = destination->length + source->length;
    size_t const max = destination->capacity > source->capacity ? destination->capacity : source->capacity;
    size_t const capacity = sum > max ? CERPEC_FACTOR * max : max;
    _ibinary_heap_resize(destination, capacity);

    // copy source elements into destination array
    memcpy(destination->elements + (destination->length * destination->size), source->elements, source->length * source->size);
    destination->length += source->length;

    // clear (NOT DESTROY) source elements array
    source->allocator->free(source->elements, source->allocator->arg);
    source->length = source->capacity = 0;
    source->elements = NULL;

    // for each element at half index call
    size_t const start = (destination->length / 2) - 1; // value may underflow when length is less than two
    for (size_t i = 0; destination->length > 1 && i <= start; ++i) {
        size_t const reverse = start - i;
        _ibinary_heapify_down(destination, reverse);
    }
}

void each_ibinary_heap(ibinary_heap_s const * const heap, handle_fn const handle, void * const ah) {
    assert(heap && "Parameter can't be NULL.");
    assert(handle && "Parameter can't be NULL.");

    assert(heap->compare && "Parameter can't be NULL.");
    assert(heap->size && "Parameter can't be zero.");
    assert(heap->allocator && "Paremeter can't be NULL.");

    for (size_t i = 0; i < heap->length && handle(heap->elements + (i * heap->size), ah); ++i) {}
}

void _ibinary_heap_resize(ibinary_heap_s * const heap, size_t const size) {
    heap->capacity = size;

    heap->elements = heap->allocator->realloc(heap->elements, size * heap->size, heap->allocator->arg);
    assert((!size || heap->elements) && "Memory allocation failed.");
}

void _ibinary_heapify_up(ibinary_heap_s const * const heap, size_t const index) {
    // while current top child index is not zero and current element is greater than its parent
    size_t c = index;
    size_t p = (c - 1) / 2;

    void * child = heap->elements + (c * heap->size);
    void * parent = heap->elements + (p * heap->size);
    while (c && heap->compare(child, parent, heap->ac) < 0) {
        // swap current child element with parent
        _ibinary_heap_swap(child, parent, heap->size);

        // change current child to parent and parent to grand-parent
        c = p;
        p = (c - 1) / 2;
        child = heap->elements + (c * heap->size);
        parent = heap->elements + (p * heap->size);
    }
}

void _ibinary_heapify_down(ibinary_heap_s const * const heap, size_t const index) {
    for (size_t p = index, c = (2 * p) + 1; c < heap->length; p = c, c = (2 * p) + 1) {
        size_t const s = c + 1;

        void const * sibling = heap->elements + (s * heap->size);
        void const * juvenile = heap->elements + (c * heap->size);
        // if right child is a valid index and it is smaller than left child change left child to right one
        if (s < heap->length && heap->compare(juvenile, sibling, heap->ac) > 0) {
            c = s;
        }

        void * const child = heap->elements + (c * heap->size);
        void * const parent = heap->elements + (p * heap->size);
        // if child is greater, then parent is properly set and thus break from loop
        if (heap->compare(child, parent, heap->ac) > 0) {
            break;
        }

        // swap current parent element with greatest child
        _ibinary_heap_swap(child, parent, heap->size);
    }
}

void _ibinary_heap_swap(void * const a, void * const b, size_t const size) {
    char * const _a = a, * const _b = b;
    for (size_t i = 0; i < size; ++i) {
        char const temp = _a[i];
        _a[i] = _b[i];
        _b[i] = temp;
    }
}
