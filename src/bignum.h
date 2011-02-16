#ifndef INCLUDED_bignum_h
#define INCLUDED_bignum_h

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

/*
 * struct bignum — arbitrary size non-negative integer type.
 */

#define BIGNUM_CHUNK_SIZE_BITS 32
typedef uint32_t bignum_chunk;
typedef uint64_t bignum_larger_chunk;
#define BIGNUM_CHUNK_MODULO (((bignum_larger_chunk)UINT32_MAX)+1)

typedef struct {
  list *chunks;
  unsigned int size;            /* in chunks */
} bignum;

bignum *bignum_from_chunk(bignum_chunk x);
void bignum_free(bignum *n);
bignum *bignum_copy(bignum *n);

bool bignum_eq(bignum *a, bignum *b);
bool bignum_lt(bignum *a, bignum *b);
bool bignum_gt(bignum *a, bignum *b);
bool bignum_lteq(bignum *a, bignum *b);
bool bignum_gteq(bignum *a, bignum *b);

void bignum_add(bignum **a, bignum *b);

char *bignum_to_str(const bignum *n);

#endif
