#ifndef TEST_H
#define TEST_H

#include <stddef.h>
#include <stdbool.h>

typedef int test_t;

struct compare {
    int (*compare_element)(const void *, const void *);
};

void destroy(void * element);
bool increment(void * element, void * value);
bool print(void * element, void * format);
void sort(void * array, const size_t length, void * compare);
bool odd(const void * element, void * nil);
bool even(const void * element, void * nil);
bool prime(const void * element, void * nil);

int compare(const void * a, const void * b);
int compare_reverse(const void * a, const void * b);

#endif // TEST_H
