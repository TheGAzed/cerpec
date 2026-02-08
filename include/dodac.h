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

void chardst(void * const element);

void * charcpy(void * const destination, void const * const source);

void * charsum(void * const sum, void const * const addent);

size_t charhshsame(void const * const element);

int charcmp(void const * const a, void const * const b);

int charrcmp(void const * const a, void const * const b);

bool charprint(void * const element, void * const format);

bool chartolower(void * const element, void * const null);

bool chartoupper(void * const element, void * const null);

void charqsort(void * const array, size_t const lenght, void * const null);

void charrqsort(void * const array, size_t const lenght, void * const null);

bool chareven(void const * const element, void * const null);

bool charodd(void const * const element, void * const null);

bool charalnum(void const * const element, void * const null);

bool charalpha(void const * const element, void * const null);

bool charcntrl(void const * const element, void * const null);

bool chardigit(void const * const element, void * const null);

bool charlower(void const * const element, void * const null);

bool charupper(void const * const element, void * const null);

bool charpunct(void const * const element, void * const null);

bool charspace(void const * const element, void * const null);

bool charxdigit(void const * const element, void * const null);

bool charprime(void const * const element, void * const null);

void uchardst(void * const element);

void * ucharcpy(void * const destination, void const * const source);

void * ucharsum(void * const sum, void const * const addent);

size_t ucharhshsame(void const * const element);

int ucharcmp(void const * const a, void const * const b);

int ucharrcmp(void const * const a, void const * const b);

bool ucharprint(void * const element, void * const format);

bool uchartolower(void * const element, void * const null);

bool uchartoupper(void * const element, void * const null);

void ucharqsort(void * const array, size_t const lenght, void * const null);

void ucharrqsort(void * const array, size_t const lenght, void * const null);

bool uchareven(void const * const element, void * const null);

bool ucharodd(void const * const element, void * const null);

bool ucharalnum(void const * const element, void * const null);

bool ucharalpha(void const * const element, void * const null);

bool ucharcntrl(void const * const element, void * const null);

bool uchardigit(void const * const element, void * const null);

bool ucharlower(void const * const element, void * const null);

bool ucharupper(void const * const element, void * const null);

bool ucharpunct(void const * const element, void * const null);

bool ucharspace(void const * const element, void * const null);

bool ucharxdigit(void const * const element, void * const null);

bool ucharprime(void const * const element, void * const null);

void intdst(void * const element);

void * intcpy(void * const destination, void const * const source);

void * intsum(void * const sum, void const * const addent);

size_t inthshsame(void const * const element);

size_t inthshmurmur(void const * const element);

int intcmp(void const * const a, void const * const b);

int intrcmp(void const * const a, void const * const b);

bool intprint(void * const element, void * const format);

bool intincrement(void * const element, void * const value);

bool intdecrement(void * const element, void * const value);

void intqsort(void * const array, size_t const lenght, void * const null);

void intrqsort(void * const array, size_t const lenght, void * const null);

bool inteven(void const * const element, void * const null);

bool intodd(void const * const element, void * const null);

bool intprime(void const * const element, void * const null);

void uintdst(void * const element);

void * uintcpy(void * const destination, void const * const source);

void * uintsum(void * const sum, void const * const addent);

size_t uinthshsame(void const * const element);

int uintcmp(void const * const a, void const * const b);

int uintrcmp(void const * const a, void const * const b);

bool uintprint(void * const element, void * const format);

void uintqsort(void * const array, size_t const lenght, void * const null);

void uintrqsort(void * const array, size_t const lenght, void * const null);

bool uinteven(void const * const element, void * const null);

bool uintodd(void const * const element, void * const null);

bool uintprime(void const * const element, void * const null);

void floatdst(void * const element);

void * floatcpy(void * const destination, void const * const source);

void * floatsum(void * const sum, void const * const addent);

size_t floathshsame(void const * const element);

int floatcmp(void const * const a, void const * const b);

int floatrcmp(void const * const a, void const * const b);

bool floatprint(void * const element, void * const format);

void floatqsort(void * const array, size_t const lenght, void * const null);

void floatrqsort(void * const array, size_t const lenght, void * const null);

void doubledst(void * const element);

void * doublecpy(void * const destination, void const * const source);

void * doublesum(void * const sum, void const * const addent);

size_t doublehshsame(void const * const element);

int doublecmp(void const * const a, void const * const b);

int doublercmp(void const * const a, void const * const b);

bool doubleprint(void * const element, void * const format);

void doubleqsort(void * const array, size_t const lenght, void * const null);

void doublerqsort(void * const array, size_t const lenght, void * const null);

void sizedst(void * const element);

void * sizecpy(void * const destination, void const * const source);

void * sizesum(void * const sum, void const * const addent);

size_t sizehshsame(void const * const element);

int sizecmp(void const * const a, void const * const b);

int sizercmp(void const * const a, void const * const b);

bool sizeprint(void * const element, void * const format);

void sizeqsort(void * const array, size_t const lenght, void * const null);

void sizerqsort(void * const array, size_t const lenght, void * const null);

bool sizeeven(void const * const element, void * const null);

bool sizeodd(void const * const element, void * const null);

bool sizeprime(void const * const element, void * const null);

size_t stringhshmurmur(void const * const element);

int stringcmp(void const * const a, void const * const b);

int stringrcmp(void const * const a, void const * const b);

bool stringprint(void * const element, void * const format);

void stringqsort(void * const array, size_t const lenght, void * const null);

void stringrqsort(void * const array, size_t const lenght, void * const null);

#endif // DODAC_H
