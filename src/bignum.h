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

typedef struct {
  struct list *chunks;
  unsigned int size;            /* in chunks */
} bignum;

bignum *bignum_from_chunk(bignum_chunk x);
bool bignum_eq(bignum *a, bignum *b);
char *bignum_to_str(const bignum *n);

#endif
