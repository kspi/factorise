#ifndef INCLUDED_bignum_h
#define INCLUDED_bignum_h

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

/*
 * struct bignum — arbitrary size non-negative integer type.
 */

#define BIGNUM_DIGIT_SIZE_BITS 32
typedef uint32_t bignum_digit;
typedef int64_t bignum_overflow; /* must be able to hold values at
                                    least twice as large as digit */
#define BIGNUM_BASE (((bignum_overflow)UINT32_MAX)+1)

typedef struct {
  list *digits;
  unsigned int size;            /* in digits */
} bignum;

bignum *bignum_from_digit(bignum_digit x);
void bignum_free(bignum *n);
bignum *bignum_copy(const bignum *n);

bool bignum_eq(bignum *a, bignum *b);
bool bignum_lt(bignum *a, bignum *b);
bool bignum_gt(bignum *a, bignum *b);
bool bignum_lteq(bignum *a, bignum *b);
bool bignum_gteq(bignum *a, bignum *b);

void bignum_add(bignum **a, bignum *b);

char *bignum_to_str(const bignum *n);

#endif
