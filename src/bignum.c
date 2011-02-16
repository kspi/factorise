#include <stdlib.h>
#include "bignum.h"

static bignum_chunk *chunk_new(bignum_chunk x) {
  bignum_chunk *result = malloc(sizeof *result);
  *result = x;
  return result;
}

bignum *bignum_from_chunk(bignum_chunk x) {
  bignum *n = malloc(sizeof *n);
  list_push(chunk_new(x), &n->chunks);
  n->size = 1;
  return n;
}

bool bignum_eq(bignum *a, bignum *b) {
  if (a->size != b->size) {
    return false;
  } else {
    list *p = a->chunks;
    list *q = b->chunks;
    
  loop:
    if (LIST_EMPTY(p) && LIST_EMPTY(q)) {
      return true;
    } else if (LIST_EMPTY(p) || LIST_EMPTY(q)) {
      return false;
    } else if (LIST_HEAD(p, bignum_chunk) != LIST_HEAD(q, bignum_chunk)) {
      return false;
    } else {
      LIST_NEXT(p);
      LIST_NEXT(q);
      goto loop;
    }
  }
}

void bignum_divmod(bignum *a, bignum *d, bignum **quot, bignum **rem) {
  
}

char *bignum_to_str(const bignum *n) {
  
}

