#ifndef DODAC_H
#define DODAC_H

#include <stddef.h>
#include <stdbool.h>

#ifndef DODAC_SEED
#    define DODAC_SEED 0x12345678
#endif

#define CHAR_FORMAT "%c"
#define UCHAR_FORMAT "%c"
#define INT_FORMAT "%d "
#define UINT_FORMAT "%u "
#define FLOAT_FORMAT "%f "
#define FLOAT_FORMAT "%f "
#define DOUBLE_FORMAT "%lf "
#define SIZE_FORMAT "%zu "
#define STRING_FORMAT "%s "

void voiddst(void * const element, void * arg);

void chardst(void * const element, void * arg);

void * charcpy(void * const destination, void const * const source, void * arg);

void * charsum(void * const sum, void const * const addent, void * arg);

size_t charhshsame(void const * const element, void * arg);

int charcmp(void const * const a, void const * const b, void * arg);

int charrcmp(void const * const a, void const * const b, void * arg);

bool charprint(void * const element, void * const format);

bool chartolower(void * const element, void * const null);

bool chartoupper(void * const element, void * const null);

void charqsort(void * const array, size_t const lenght, void * const null);

void charrqsort(void * const array, size_t const lenght, void * const null);

bool charfileven(void const * const element, void * arg);

bool charfilodd(void const * const element, void * arg);

bool charfilalnum(void const * const element, void * arg);

bool charfilalpha(void const * const element, void * arg);

bool charfilcntrl(void const * const element, void * arg);

bool charfildigit(void const * const element, void * arg);

bool charfillower(void const * const element, void * arg);

bool charfilupper(void const * const element, void * arg);

bool charfilpunct(void const * const element, void * arg);

bool charfilspace(void const * const element, void * arg);

bool charfilxdigit(void const * const element, void * arg);

bool charfilprime(void const * const element, void * arg);

void uchardst(void * const element, void * arg);

void * ucharcpy(void * const destination, void const * const source, void * arg);

void * ucharsum(void * const sum, void const * const addent, void * arg);

size_t ucharhshsame(void const * const element, void * arg);

int ucharcmp(void const * const a, void const * const b, void * arg);

int ucharrcmp(void const * const a, void const * const b, void * arg);

bool ucharprint(void * const element, void * const format);

bool uchartolower(void * const element, void * const null);

bool uchartoupper(void * const element, void * const null);

void ucharqsort(void * const array, size_t const lenght, void * const null);

void ucharrqsort(void * const array, size_t const lenght, void * const null);

bool ucharfileven(void const * const element, void * arg);

bool ucharfilodd(void const * const element, void * arg);

bool ucharfilalnum(void const * const element, void * arg);

bool ucharfilalpha(void const * const element, void * arg);

bool ucharfilcntrl(void const * const element, void * arg);

bool ucharfildigit(void const * const element, void * arg);

bool ucharfillower(void const * const element, void * arg);

bool ucharfilupper(void const * const element, void * arg);

bool ucharfilpunct(void const * const element, void * arg);

bool ucharfilspace(void const * const element, void * arg);

bool ucharfilxdigit(void const * const element, void * arg);

bool ucharfilprime(void const * const element, void * arg);

void intdst(void * const element, void * arg);

void * intcpy(void * const destination, void const * const source, void * arg);

void * intsum(void * const sum, void const * const addent, void * arg);

size_t inthshsame(void const * const element, void * arg);

size_t inthshmurmur(void const * const element, void * arg);

int intcmp(void const * const a, void const * const b, void * arg);

int intrcmp(void const * const a, void const * const b, void * arg);

bool intprint(void * const element, void * const format);

bool intincrement(void * const element, void * const value);

bool intdecrement(void * const element, void * const value);

void intqsort(void * const array, size_t const lenght, void * const null);

void intrqsort(void * const array, size_t const lenght, void * const null);

bool intfileven(void const * const element, void * arg);

bool intfilodd(void const * const element, void * arg);

bool intfilprime(void const * const element, void * arg);

void uintdst(void * const element, void * arg);

void * uintcpy(void * const destination, void const * const source, void * arg);

void * uintsum(void * const sum, void const * const addent, void * arg);

size_t uinthshsame(void const * const element, void * arg);

int uintcmp(void const * const a, void const * const b, void * arg);

int uintrcmp(void const * const a, void const * const b, void * arg);

bool uintprint(void * const element, void * const format);

void uintqsort(void * const array, size_t const lenght, void * const null);

void uintrqsort(void * const array, size_t const lenght, void * const null);

bool uintfileven(void const * const element, void * arg);

bool uintfilodd(void const * const element, void * arg);

bool uintfilprime(void const * const element, void * arg);

void floatdst(void * const element, void * arg);

void * floatcpy(void * const destination, void const * const source, void * arg);

void * floatsum(void * const sum, void const * const addent, void * arg);

size_t floathshsame(void const * const element, void * arg);

int floatcmp(void const * const a, void const * const b, void * arg);

int floatrcmp(void const * const a, void const * const b, void * arg);

bool floatprint(void * const element, void * const format);

void floatqsort(void * const array, size_t const lenght, void * const null);

void floatrqsort(void * const array, size_t const lenght, void * const null);

void doubledst(void * const element, void * arg);

void * doublecpy(void * const destination, void const * const source, void * arg);

void * doublesum(void * const sum, void const * const addent, void * arg);

size_t doublehshsame(void const * const element, void * arg);

int doublecmp(void const * const a, void const * const b, void * arg);

int doublercmp(void const * const a, void const * const b, void * arg);

bool doubleprint(void * const element, void * const format);

void doubleqsort(void * const array, size_t const lenght, void * const null);

void doublerqsort(void * const array, size_t const lenght, void * const null);

void sizedst(void * const element, void * arg);

void * sizecpy(void * const destination, void const * const source, void * arg);

void * sizesum(void * const sum, void const * const addent, void * arg);

size_t sizehshsame(void const * const element, void * arg);

int sizecmp(void const * const a, void const * const b, void * arg);

int sizercmp(void const * const a, void const * const b, void * arg);

bool sizeprint(void * const element, void * const format);

void sizeqsort(void * const array, size_t const lenght, void * const null);

void sizerqsort(void * const array, size_t const lenght, void * const null);

bool sizefileven(void const * const element, void * arg);

bool sizefilodd(void const * const element, void * arg);

bool sizefilprime(void const * const element, void * arg);

size_t stringhshmurmur(void const * const element, void * arg);

int stringcmp(void const * const a, void const * const b, void * arg);

int stringrcmp(void const * const a, void const * const b, void * arg);

bool stringprint(void * const element, void * const format);

void stringqsort(void * const array, size_t const lenght, void * const null);

void stringrqsort(void * const array, size_t const lenght, void * const null);

#endif // DODAC_H
