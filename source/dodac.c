#include <dodac.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void chardst(void * const element) {
    *(char*)element = '\0';
}

void * charcpy(void * const destination, void const * const source) {
    *(char*)destination = *(char*)source;
    return destination;
}

void * charsum(void * const sum, void const * const addent) {
    *(char*)sum += *(char*)addent;
    return sum;
}

size_t charhshsame(void const * const element) {
    return (size_t)(*(char*)element);
}

int charcmp(void const * const a, void const * const b) {
    return (int)((*(char*)a) - (*(char*)b));
}

int charrcmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(char), charcmp);
}

void charrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(char), charrcmp);
}

bool chareven(void const * const element, void * const null) {
    (void)(null);
    return !((*(char*)element) % 2);
}

bool charodd(void const * const element, void * const null) {
    (void)(null);
    return ((*(char*)element) % 2);
}

bool charalnum(void const * const element, void * const null) {
    (void)(null);
    return isalnum(*(char*)element);
}

bool charalpha(void const * const element, void * const null) {
    (void)(null);
    return isalpha(*(char*)element);
}

bool charcntrl(void const * const element, void * const null) {
    (void)(null);
    return iscntrl(*(char*)element);
}

bool chardigit(void const * const element, void * const null) {
    (void)(null);
    return isdigit(*(char*)element);
}

bool charlower(void const * const element, void * const null) {
    (void)(null);
    return islower(*(char*)element);
}

bool charupper(void const * const element, void * const null) {
    (void)(null);
    return isupper(*(char*)element);
}

bool charpunct(void const * const element, void * const null) {
    (void)(null);
    return ispunct(*(char*)element);
}

bool charspace(void const * const element, void * const null) {
    (void)(null);
    return isspace(*(char*)element);
}

bool charxdigit(void const * const element, void * const null) {
    (void)(null);
    return isxdigit(*(char*)element);
}

bool charprime(void const * const element, void * const null) {
    (void)(null);

    const char number = (*(char*)element);
    for (char i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void uchardst(void * const element) {
    *(unsigned char*)element = '\0';
}

void * ucharcpy(void * const destination, void const * const source) {
    *(unsigned char*)destination = *(unsigned char*)source;
    return destination;
}

void * ucharsum(void * const sum, void const * const addent) {
    *(unsigned char*)sum += *(unsigned char*)addent;
    return sum;
}

size_t ucharhshsame(void const * const element) {
    return (size_t)(*(unsigned char*)element);
}

int ucharcmp(void const * const a, void const * const b) {
    int const a_t = (int)(*(unsigned char*)a);
    int const b_t = (int)(*(unsigned char*)b);

    return a_t - b_t;
}

int ucharrcmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(unsigned char), charcmp);
}

void ucharrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned char), charrcmp);
}

bool uchareven(void const * const element, void * const null) {
    (void)(null);
    return !((*(unsigned char*)element) % 2);
}

bool ucharodd(void const * const element, void * const null) {
    (void)(null);
    return ((*(unsigned char*)element) % 2);
}

bool ucharalnum(void const * const element, void * const null) {
    (void)(null);
    return isalnum(*(unsigned char*)element);
}

bool ucharalpha(void const * const element, void * const null) {
    (void)(null);
    return isalpha(*(unsigned char*)element);
}

bool ucharcntrl(void const * const element, void * const null) {
    (void)(null);
    return iscntrl(*(unsigned char*)element);
}

bool uchardigit(void const * const element, void * const null) {
    (void)(null);
    return isdigit(*(unsigned char*)element);
}

bool ucharlower(void const * const element, void * const null) {
    (void)(null);
    return islower(*(unsigned char*)element);
}

bool ucharupper(void const * const element, void * const null) {
    (void)(null);
    return isupper(*(unsigned char*)element);
}

bool ucharpunct(void const * const element, void * const null) {
    (void)(null);
    return ispunct(*(unsigned char*)element);
}

bool ucharspace(void const * const element, void * const null) {
    (void)(null);
    return isspace(*(unsigned char*)element);
}

bool ucharxdigit(void const * const element, void * const null) {
    (void)(null);
    return isxdigit(*(unsigned char*)element);
}

bool ucharprime(void const * const element, void * const null) {
    (void)(null);

    const unsigned char number = (*(unsigned char*)element);
    for (unsigned char i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void intdst(void * const element) {
    (*(int*)element) = 0;
}

void * intcpy(void * const destination, void const * const source) {
    (*(int*)destination) = (*(int*)source);
    return destination;
}

void * intsum(void * const sum, void const * const addent) {
    (*(int*)sum) += (*(int*)addent);
    return sum;
}

size_t inthshsame(void const * const element) {
    return (size_t)(*(int*)element);
}

size_t inthshmurmur(void const * const element) {
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

int intcmp(void const * const a, void const * const b) {
    return (*(int*)a) - (*(int*)b);
}

int intrcmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(int), intcmp);
}

void intrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(int), intrcmp);
}

bool inteven(void const * const element, void * const null) {
    (void)(null);
    return !((*(int*)element) % 2);
}

bool intodd(void const * const element, void * const null) {
    (void)(null);
    return (*(int*)element) % 2;
}

bool intprime(void const * const element, void * const null) {
    (void)(null);

    const int number = (*(int*)element);
    for (int i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void uintdst(void * const element) {
    (*(unsigned*)element) = 0U;
}

void * uintcpy(void * const destination, void const * const source) {
    (*(unsigned*)destination) = (*(unsigned*)source);
    return destination;
}

void * uintsum(void * const sum, void const * const addent) {
    (*(unsigned*)sum) += (*(unsigned*)addent);
    return sum;
}

size_t uinthshsame(void const * const element) {
    return (size_t)(*(unsigned*)element);
}

int uintcmp(void const * const a, void const * const b) {
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int uintrcmp(void const * const a, void const * const b) {
    unsigned const a_temp = (*(unsigned*)a);
    unsigned const b_temp = (*(unsigned*)b);

    return a_temp < b_temp ? 1 : a_temp > b_temp ? -1 : 0;
}

bool uintprint(void * const element, void * const format) {
    printf(format ? format : UINT_FORMAT, (*(unsigned*)element));
}

void uintqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned), uintcmp);
}

void uintrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(unsigned), uintrcmp);
}

bool uinteven(void const * const element, void * const null) {
    (void)(null);
    return !((*(unsigned*)element) % 2);
}

bool uintodd(void const * const element, void * const null) {
    (void)(null);
    return !((*(unsigned*)element) % 2);
}

bool uintprime(void const * const element, void * const null) {
    (void)(null);

    const unsigned number = (*(unsigned*)element);
    for (unsigned i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

void floatdst(void * const element) {
    (*(float*)element) = 0.0f;
}

void * floatcpy(void * const destination, void const * const source) {
    (*(float*)destination) = (*(float*)source);
    return destination;
}

void * floatsum(void * const sum, void const * const addent) {
    (*(float*)sum) += (*(float*)addent);
    return sum;
}

size_t floathshsame(void const * const element) {
    size_t size = sizeof(float) > sizeof(size_t) ? sizeof(size_t) : sizeof(float);
    memcpy(&size, element, size);
    return size;
}

int floatcmp(void const * const a, void const * const b) {
    float const a_temp = (*(float*)a);
    float const b_temp = (*(float*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int floatrcmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(float), floatcmp);
}

void floatrqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(float), floatrcmp);
}

void doubledst(void * const element) {
    (*(double*)element) = 0.0;
}

void * doublecpy(void * const destination, void const * const source) {
    (*(double*)destination) = (*(double*)source);
    return destination;
}

void * doublesum(void * const sum, void const * const addent) {
    (*(double*)sum) += (*(double*)addent);
    return sum;
}

size_t doublehshsame(void const * const element) {
    size_t size = sizeof(double) > sizeof(size_t) ? sizeof(size_t) : sizeof(double);
    memcpy(&size, element, size);
    return size;
}

int doublecmp(void const * const a, void const * const b) {
    double const a_temp = (*(double*)a);
    double const b_temp = (*(double*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int doublercmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(double), doublecmp);
}

void doublerqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(double), doublercmp);
}

void sizedst(void * const element) {
    (*(size_t*)element) = 0;
}

void * sizecpy(void * const destination, void const * const source) {
    (*(size_t*)destination) = (*(size_t*)source);
    return destination;
}

void * sizesum(void * const sum, void const * const addent) {
    (*(size_t*)sum) += (*(size_t*)addent);
    return sum;
}

size_t sizehshsame(void const * const element) {
    return (*(size_t*)element);
}

int sizecmp(void const * const a, void const * const b) {
    size_t const a_temp = (*(size_t*)a);
    size_t const b_temp = (*(size_t*)b);

    return a_temp < b_temp ? -1 : a_temp > b_temp ? 1 : 0;
}

int sizercmp(void const * const a, void const * const b) {
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
    qsort(array, lenght, sizeof(size_t), sizecmp);
}

void sizerqsort(void * const array, size_t const lenght, void * const null) {
    (void)(null);
    qsort(array, lenght, sizeof(size_t), sizercmp);
}

bool sizeeven(void const * const element, void * const null) {
    (void)(null);
    return !((*(size_t*)element) % 2);
}

bool sizeodd(void const * const element, void * const null) {
    (void)(null);
    return ((*(size_t*)element) % 2);
}

bool sizeprime(void const * const element, void * const null) {
    (void)(null);

    const size_t number = (*(size_t*)element);
    for (size_t i = 2; i < number; i++) {
        if (number % i == 0 && i != number) {
            return false;
        }
    }

    return number > 1;
}

size_t stringhshmurmur(void const * const element) {
    char const * string = element;
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

int stringcmp(void const * const a, void const * const b) {
    return strcmp(a, b);
}

int stringrcmp(void const * const a, void const * const b) {
    return strcmp(b, a);
}

bool stringprint(void * const element, void * const format) {
    printf(format ? format : STRING_FORMAT, (char*)format);
    return true;
}

void stringqsort(void * const array, size_t const lenght, void * const null) {
    (void)null;
    qsort(array, lenght, sizeof(char*), stringcmp);
}

void stringrqsort(void * const array, size_t const lenght, void * const null) {
    (void)null;
    qsort(array, lenght, sizeof(char*), stringcmp);
}
