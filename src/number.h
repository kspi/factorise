#ifndef INCLUDED_number_h
#define INCLUDED_number_h

#include <stdbool.h>

typedef unsigned long long number;
#define NUMFMT "%llu"
#define NUMBER_MIN ((number)0)
#define NUMBER_MAX ULLONG_MAX
#define UNSIGNED 1

number strtonum(const char *x);

/* POW2 pakelia 2 x-uoju laipsniu. */
#define POW2(x) ((number)1 << (x))

number square(number x);
number ipow(number base, number degree);
number isqrt(number n);
bool is_square(number x);

#endif
