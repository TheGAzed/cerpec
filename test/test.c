#include <test.h>

#include <stdio.h>
#include <stdlib.h>

void destroy(void * element) {
    (void)(element);
}

int compare(const void * a, const void * b) {
    return (*(test_t*)(a)) - (*(test_t*)(b));
}

int compare_reverse(const void * a, const void * b) {
    return (*(test_t*)(b)) - (*(test_t*)(a));
}

bool increment(void * element, void * value) {
    test_t * e = element, * v = value;
    (*e) += (*v);

    return true;
}

bool print(void * element, void * format) {
    //printf(format, (*(test_t*)element)); fflush(stdout);
    (void)(element); (void)(format);

    return true;
}

void sort(void * array, const size_t length, void * compare) {
    struct compare const * cmp = compare;
    qsort(array, length, sizeof(test_t), cmp->compare_element);
}
