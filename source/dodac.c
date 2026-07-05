#include <dodac.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void voiddst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

int cmpchar(void const * const a, void const * const b) {
    return (int)((*(char*)a) - (*(char*)b));
}

int rcmpchar(void const * const a, void const * const b) {
    return (int)((*(char*)b) - (*(char*)a));
}

int cmpuchar(void const * const a, void const * const b) {
    int const a_t = (int)(*(unsigned char*)a);
    int const b_t = (int)(*(unsigned char*)b);

    return a_t - b_t;
}

int rcmpuchar(void const * const a, void const * const b) {
    int const a_t = (int)(*(unsigned char*)a);
    int const b_t = (int)(*(unsigned char*)b);

    return b_t - a_t;
}

int cmpint(void const * const a, void const * const b) {
    return (*(int*)a) - (*(int*)b);
}

int rcmpint(void const * const a, void const * const b) {
    return (*(int*)b) - (*(int*)a);
}

int cmpuint(void const * const a, void const * const b) {
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int rcmpuint(void const * const a, void const * const b) {
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

int cmpfloat(void const * const a, void const * const b) {
    float const a_temp = (*(float*)a);
    float const b_temp = (*(float*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int rcmpfloat(void const * const a, void const * const b) {
    float const a_temp = (*(float*)a);
    float const b_temp = (*(float*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

int cmpdouble(void const * const a, void const * const b) {
    double const a_temp = (*(double*)a);
    double const b_temp = (*(double*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int rcmpdouble(void const * const a, void const * const b) {
    double const a_temp = (*(double*)a);
    double const b_temp = (*(double*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

int cmpsize(void const * const a, void const * const b) {
    size_t const a_temp = (*(size_t*)a);
    size_t const b_temp = (*(size_t*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int rcmpsize(void const * const a, void const * const b) {
    size_t const a_temp = (*(size_t*)a);
    size_t const b_temp = (*(size_t*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

int cmpstring(void const * const a, void const * const b) {
    return strcmp(*((char**)(a)), *((char**)(b)));
}

int rcmpstring(void const * const a, void const * const b) {
    return strcmp(*((char**)(b)), *((char**)(a)));
}

void chardst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * charcpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    *(char*)destination = *(char*)source;
    return destination;
}

void * charsum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    *(char*)sum += *(char*)addent;
    return sum;
}

size_t charhshsame(void const * const element, void * arg) {
    (void)(arg);
    return (size_t)(*(char*)element);
}

int charcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return (int)((*(char*)a) - (*(char*)b));
}

int charrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return (int)((*(char*)b) - (*(char*)a));
}

bool charprint(void * const element, void * const format) {
    printf(format ? format : CHAR_FORMAT, *(char*)element);
    return true;
}

bool chartolower(void * const element, void * const null) {
    (void)(null);

    char const temp = (char)tolower(*(char*)element);
    *(char*) element = temp;

    return true;
}

bool chartoupper(void * const element, void * const null) {
    (void)(null);

    char const temp = (char)toupper(*(char*)element);
    *(char*) element = temp;

    return true;
}

void charqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(char), cmpchar);
}

void charrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(char), rcmpchar);
}

bool charfileven(void const * const element, void * arg) {
    (void)(arg);
    return !((*(char*)element) % 2);
}

bool charfilodd(void const * const element, void * arg) {
    (void)(arg);
    return ((*(char*)element) % 2);
}

bool charfilalnum(void const * const element, void * arg) {
    (void)(arg);
    return isalnum(*(char*)element);
}

bool charfilalpha(void const * const element, void * arg) {
    (void)(arg);
    return isalpha(*(char*)element);
}

bool charfilcntrl(void const * const element, void * arg) {
    (void)(arg);
    return iscntrl(*(char*)element);
}

bool charfildigit(void const * const element, void * arg) {
    (void)(arg);
    return isdigit(*(char*)element);
}

bool charfillower(void const * const element, void * arg) {
    (void)(arg);
    return islower(*(char*)element);
}

bool charfilupper(void const * const element, void * arg) {
    (void)(arg);
    return isupper(*(char*)element);
}

bool charfilpunct(void const * const element, void * arg) {
    (void)(arg);
    return ispunct(*(char*)element);
}

bool charfilspace(void const * const element, void * arg) {
    (void)(arg);
    return isspace(*(char*)element);
}

bool charfilxdigit(void const * const element, void * arg) {
    (void)(arg);
    return isxdigit(*(char*)element);
}

bool charfilprime(void const * const element, void * arg) {
    (void)(arg);
    const char number = (*(char*)element);
    for (char i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void uchardst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * ucharcpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    *(unsigned char*)destination = *(unsigned char*)source;
    return destination;
}

void * ucharsum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    *(unsigned char*)sum += *(unsigned char*)addent;
    return sum;
}

size_t ucharhshsame(void const * const element, void * arg) {
    (void)(arg);
    return (size_t)(*(unsigned char*)element);
}

int ucharcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    int const a_t = (int)(*(unsigned char*)a);
    int const b_t = (int)(*(unsigned char*)b);

    return a_t - b_t;
}

int ucharrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    int const a_t = (int)(*(unsigned char*)a);
    int const b_t = (int)(*(unsigned char*)b);

    return b_t - a_t;
}

bool ucharprint(void * const element, void * const format) {
    printf(format ? format : UCHAR_FORMAT, *(char*)element);
    return true;
}

bool uchartolower(void * const element, void * const null) {
    (void)(null);

    char const temp = (unsigned char)tolower(*(char*)element);
    *(char*) element = temp;

    return true;
}

bool uchartoupper(void * const element, void * const null) {
    (void)(null);

    char const temp = (unsigned char)toupper(*(char*)element);
    *(char*) element = temp;

    return true;
}

void ucharqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned char), cmpchar);
}

void ucharrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned char), rcmpchar);
}

bool ucharfileven(void const * const element, void * arg) {
    (void)(arg);
    return !((*(unsigned char*)element) % 2);
}

bool ucharfilodd(void const * const element, void * arg) {
    (void)(arg);
    return ((*(unsigned char*)element) % 2);
}

bool ucharfilalnum(void const * const element, void * arg) {
    (void)(arg);
    return isalnum(*(unsigned char*)element);
}

bool ucharfilalpha(void const * const element, void * arg) {
    (void)(arg);
    return isalpha(*(unsigned char*)element);
}

bool ucharfilcntrl(void const * const element, void * arg) {
    (void)(arg);
    return iscntrl(*(unsigned char*)element);
}

bool ucharfildigit(void const * const element, void * arg) {
    (void)(arg);
    return isdigit(*(unsigned char*)element);
}

bool ucharfillower(void const * const element, void * arg) {
    (void)(arg);
    return islower(*(unsigned char*)element);
}

bool ucharfilupper(void const * const element, void * arg) {
    (void)(arg);
    return isupper(*(unsigned char*)element);
}

bool ucharfilpunct(void const * const element, void * arg) {
    (void)(arg);
    return ispunct(*(unsigned char*)element);
}

bool ucharfilspace(void const * const element, void * arg) {
    (void)(arg);
    return isspace(*(unsigned char*)element);
}

bool ucharfilxdigit(void const * const element, void * arg) {
    (void)(arg);
    return isxdigit(*(unsigned char*)element);
}

bool ucharfilprime(void const * const element, void * arg) {
    (void)(arg);
    const unsigned char number = (*(unsigned char*)element);
    for (unsigned char i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void intdst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * intcpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    (*(int*)destination) = (*(int*)source);
    return destination;
}

void * intsum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    (*(int*)sum) += (*(int*)addent);
    return sum;
}

size_t inthshsame(void const * const element, void * arg) {
    (void)(arg);
    return (size_t)(*(int*)element);
}

size_t inthshmurmur(void const * const element, void * arg) {
    (void)(arg);
    // Fast-hash, as described by https://jonkagstrom.com/bit-mixer-construction and
    // https://code.google.com/archive/p/fast-hash.
    // In testing, this hash function provided slightly better performance than the Murmur3 mixer.
    // code copied from https://github.com/JacksonAllan/Verstable/blob/main/verstable.h
    size_t key = *((int*)element);

    key ^= key >> 23;
    key *= 0x2127599bf4325c37ull;
    key ^= key >> 47;

    return key;
}

int intcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return (*(int*)a) - (*(int*)b);
}

int intrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return (*(int*)b) - (*(int*)a);
}

bool intprint(void * const element, void * const format) {
    printf(format ? format : INT_FORMAT, (*(int*)element));
    return true;
}

bool intincrement(void * const element, void * const value) {
    (*(int*)element) += (*(int*)value);
    return true;
}

bool intdecrement(void * const element, void * const value) {
    (*(int*)element) -= (*(int*)value);
    return true;
}

void intqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(int), cmpint);
}

void intrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(int), rcmpint);
}

bool intfileven(void const * const element, void * arg) {
    (void)(arg);
    return !((*(int*)element) % 2);
}

bool intfilodd(void const * const element, void * arg) {
    (void)(arg);
    return (*(int*)element) % 2;
}

bool intfilprime(void const * const element, void * arg) {
    (void)(arg);
    const int number = (*(int*)element);
    for (int i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void uintdst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * uintcpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    (*(unsigned*)destination) = (*(unsigned*)source);
    return destination;
}

void * uintsum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    (*(unsigned*)sum) += (*(unsigned*)addent);
    return sum;
}

size_t uinthshsame(void const * const element, void * arg) {
    (void)(arg);
    return (size_t)(*(unsigned*)element);
}

int uintcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int uintrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

bool uintprint(void * const element, void * const format) {
    printf(format ? format : UINT_FORMAT, (*(unsigned*)element));
    return true;
}

void uintqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned), cmpuint);
}

void uintrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned), rcmpuint);
}

bool uintfileven(void const * const element, void * arg) {
    (void)(arg);
    return !((*(unsigned*)element) % 2);
}

bool uintfilodd(void const * const element, void * arg) {
    (void)(arg);
    return !((*(unsigned*)element) % 2);
}

bool uintfilprime(void const * const element, void * arg) {
    (void)(arg);
    const unsigned number = (*(unsigned*)element);
    for (unsigned i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void floatdst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * floatcpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    (*(float*)destination) = (*(float*)source);
    return destination;
}

void * floatsum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    (*(float*)sum) += (*(float*)addent);
    return sum;
}

size_t floathshsame(void const * const element, void * arg) {
    (void)(arg);
    size_t size = sizeof(float) > sizeof(size_t) ? sizeof(size_t) : sizeof(float);
    memcpy(&size, element, size);
    return size;
}

int floatcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    float const a_temp = (*(float*)a);
    float const b_temp = (*(float*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int floatrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    float const a_temp = (*(float*)a);
    float const b_temp = (*(float*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

bool floatprint(void * const element, void * const format) {
    printf(format ? format : FLOAT_FORMAT, (*(float*)element));
    return true;
}

void floatqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(float), cmpfloat);
}

void floatrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(float), rcmpfloat);
}

void doubledst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * doublecpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    (*(double*)destination) = (*(double*)source);
    return destination;
}

void * doublesum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    (*(double*)sum) += (*(double*)addent);
    return sum;
}

size_t doublehshsame(void const * const element, void * arg) {
    (void)(arg);
    size_t size = sizeof(double) > sizeof(size_t) ? sizeof(size_t) : sizeof(double);
    memcpy(&size, element, size);
    return size;
}

int doublecmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    double const a_temp = (*(double*)a);
    double const b_temp = (*(double*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int doublercmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    double const a_temp = (*(double*)a);
    double const b_temp = (*(double*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

bool doubleprint(void * const element, void * const format) {
    printf(format ? format : DOUBLE_FORMAT, (*(double*)element));
    return true;
}

void doubleqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(double), cmpdouble);
}

void doublerqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(double), rcmpdouble);
}

void sizedst(void * const element, void * arg) {
    (void)(element);
    (void)(arg);
}

void * sizecpy(void * const destination, void const * const source, void * arg) {
    (void)(arg);
    (*(size_t*)destination) = (*(size_t*)source);
    return destination;
}

void * sizesum(void * const sum, void const * const addent, void * arg) {
    (void)(arg);
    (*(size_t*)sum) += (*(size_t*)addent);
    return sum;
}

size_t sizehshsame(void const * const element, void * arg) {
    (void)(arg);
    return (*(size_t*)element);
}

int sizecmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    size_t const a_temp = (*(size_t*)a);
    size_t const b_temp = (*(size_t*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int sizercmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    size_t const a_temp = (*(size_t*)a);
    size_t const b_temp = (*(size_t*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

bool sizeprint(void * const element, void * const format) {
    printf(format ? format : SIZE_FORMAT, (*(size_t*)element));
    return true;
}

void sizeqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(size_t), cmpsize);
}

void sizerqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(size_t), rcmpsize);
}

bool sizefileven(void const * const element, void * arg) {
    (void)(arg);
    return !((*(size_t*)element) % 2);
}

bool sizefilodd(void const * const element, void * arg) {
    (void)(arg);
    return ((*(size_t*)element) % 2);
}

bool sizefilprime(void const * const element, void * arg) {
    (void)(arg);
    const size_t number = (*(size_t*)element);
    for (size_t i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

size_t stringhshmurmur(void const * const element, void * arg) {
    (void)(arg);
    char const * string = *((char**)element);
    size_t hash = DODAC_SEED;

    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    while(*(string++)) {
        hash ^= (*string);
        hash *= 0x5bd1e995;
        hash ^= hash >> 15;
    }

    return hash;
}

int stringcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return strcmp(*((char**)(a)), *((char**)(b)));
}

int stringrcmp(void const * const a, void const * const b, void * arg) {
    (void)(arg);
    return strcmp(*((char**)(b)), *((char**)(a)));
}

bool stringprint(void * const element, void * const format) {
    printf(format ? format : STRING_FORMAT, *(char**)element);
    return true;
}

void stringqsort(void * const array, size_t const lenght, void * const null) {
    (void)null;
    qsort(array, lenght, sizeof(char*), cmpstring);
}

void stringrqsort(void * const array, size_t const lenght, void * const null) {
    (void)null;
    qsort(array, lenght, sizeof(char*), cmpstring);
}
