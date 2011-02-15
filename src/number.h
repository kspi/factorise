#ifndef INCLUDED_number_h
#define INCLUDED_number_h

#include "list.h"

/*
 * number — Abstract integer type.
 */

typedef unsigned long long number;
#define NUMFMT "%llu"
#define NUMBER_MIN ((number)0)
#define NUMBER_MAX ULLONG_MAX
#define UNSIGNED 1

number strtonum(const char *x);
number *new_num(number n);
bool num_less_eq(void *a, void *b);

#endif
