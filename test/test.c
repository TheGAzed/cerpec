#include <test.h>

#include <stdio.h>
#include <stdlib.h>

void destroy(void * const element) {
    (void)(element);
}

void * copy(void * destination, void const * source) {
    (*(int*)destination) = (*(int*)source);
    return destination;
}

void * add(void * a, const void * b) {
    int * ai = a;
    const int * bi = b;

    (*ai) += (*bi);

    return ai;
}

int compare(void const * const a, void const * const b) {
    return (*(int*)(a)) - (*(int*)(b));
}

int compare_reverse(void const * const a, void const * const b) {
    return (*(int*)(b)) - (*(int*)(a));
}

bool increment(void * const element, void * const value) {
    int * e = element, * v = value;
    (*e) += (*v);

    return true;
}

bool print(void * const element, void * const format) {
    printf(format, (*(int*)element));
    (void)(element); (void)(format);

    return true;
}

bool nothing(void * const element, void * const argument) {
    (void)(element);
    (void)(argument);

    return true;
}

void sort(void * const array, size_t const length, void * const compare) {
    struct compare const * cmp = compare;
    qsort(array, length, sizeof(int), cmp->compare_element);
}

bool odd(void const * const element, void * const nil) {
    (void)(nil);
    return (*(int*)element) % 2;
}

bool even(void const * const element, void * const nil) {
    (void)(nil);
    return !((*(int*)element) % 2);
}

bool prime(void const * const element, void * const nil) {
    (void)(nil);

    const int number = (*(int*)element);
    for (int i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

size_t hash(void const * const element) {
    return (size_t)(*(int*)(element));
}
