#ifndef TEST_H
#define TEST_H

#include <stddef.h>
#include <stdbool.h>

typedef int test_t;

struct compare {
    int (*compare_element)(const void *, const void *);
};

void destroy(void * element, const size_t size);
bool increment(void * element, const size_t size, void * value);
bool print(void * element, const size_t size, void * format);
void sort(void * array, const size_t length, const size_t size, void * compare);

int  compare(const void * a, const void * b);
int  compare_reverse(const void * a, const void * b);

#endif // TEST_H
