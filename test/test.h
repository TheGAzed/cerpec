#ifndef TEST_H
#define TEST_H

#include <stddef.h>
#include <stdbool.h>

struct compare {
    int (*compare_element)(void const * const, void const * const);
};

void destroy(void * element);
void * copy(void * destination, const void * source);
void * add(void * a, const void * b);
bool increment(void * element, void * value);
bool print(void * element, void * format);
bool nothing(void * element, void * argument);
void sort(void * array, const size_t length, void * compare);
bool odd(const void * element, void * nil);
bool even(const void * element, void * nil);
bool prime(const void * element, void * nil);
size_t hash(const void * element);

int compare(const void * a, const void * b);
int compare_reverse(const void * a, const void * b);

#endif // TEST_H
