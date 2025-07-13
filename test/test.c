#include <test.h>

#include <stdio.h>
#include <stdlib.h>

void destroy(void * element, const size_t size) {
    (void)(element);
    (void)(size);
}

int compare(const void * a, const void * b) {
    return (*(test_t*)(a)) - (*(test_t*)(b));
}

int compare_reverse(const void * a, const void * b) {
    return (*(test_t*)(b)) - (*(test_t*)(a));
}

bool increment(void * element, const size_t size, void * value) {
    (void)(size);

    test_t * e = element, * v = value;
    (*e) += (*v);

    return true;
}

bool print(void * element, const size_t size, void * format) {
    //printf(format, (*(test_t*)element)); fflush(stdout);
    (void)(element); (void)(format); (void)(size);

    return true;
}

void sort(void * array, const size_t length, const size_t size, void * compare) {
    struct compare const * cmp = compare;
    qsort(array, length, size, cmp->compare_element);
}
