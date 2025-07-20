#include <test.h>

#include <stdio.h>
#include <stdlib.h>

void destroy(void * element) {
    (void)(element);
}

void * copy(void * destination, const void * source) {
    (*(int*)destination) = (*(int*)source);
    return destination;
}

int compare(const void * a, const void * b) {
    return (*(int*)(a)) - (*(int*)(b));
}

int compare_reverse(const void * a, const void * b) {
    return (*(int*)(b)) - (*(int*)(a));
}

bool increment(void * element, void * value) {
    int * e = element, * v = value;
    (*e) += (*v);

    return true;
}

bool print(void * element, void * format) {
    printf(format, (*(int*)element));
    (void)(element); (void)(format);

    return true;
}

void sort(void * array, const size_t length, void * compare) {
    struct compare const * cmp = compare;
    qsort(array, length, sizeof(int), cmp->compare_element);
}

bool odd(const void * element, void * nil) {
    (void)(nil);
    return (*(int*)element) % 2;
}

bool even(const void * element, void * nil) {
    (void)(nil);
    return !((*(int*)element) % 2);
}

bool prime(const void * element, void * nil) {
    (void)(nil);

    const int number = (*(int*)element);
    for (int i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}
