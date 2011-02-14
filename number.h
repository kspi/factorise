#ifndef INCLUDED_number_h
#define INCLUDED_number_h

/*
 * number — Abstract integer type.
 */

typedef unsigned long long number;
#define NUMFMT "%llu"
#define NUMBER_MIN ((number)0)
#define NUMBER_MAX ULLONG_MAX
#define UNSIGNED 1

number strtonum(const char *x);

#endif
