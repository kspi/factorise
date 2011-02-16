#include <stdlib.h>
#include "bignum.h"

static bignum_chunk *chunk_new(bignum_chunk x) {
  bignum_chunk *result = malloc(sizeof *result);
  *result = x;
  return result;
}

static bignum_chunk *chunk_copy(bignum_chunk *x) {
  return chunk_new(*x);
}

bignum *bignum_from_chunk(bignum_chunk x) {
  bignum *n = malloc(sizeof *n);
  list_push(chunk_new(x), &n->chunks);
  n->size = 1;
  return n;
}

void bignum_free(bignum *n) {
  list_free(n->chunks, free);
  free(n);
}

bignum *bignum_copy(bignum *n) {
  bignum *new = malloc(sizeof *new);
  new->size = n->size;
  new->chunks = list_copy(n->chunks, (copy_fn *)chunk_copy);
  return new;
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

bool bignum_lt(bignum *a, bignum *b) {
  if (a->size > b->size) {
    return false;
  } else {
    list *p = a->chunks;
    list *q = b->chunks;
    
  loop:
    if (LIST_EMPTY(p)) {
      return true;
    } else if (LIST_EMPTY(q)) {
      return false;
    } else if (LIST_HEAD(p, bignum_chunk) >= LIST_HEAD(q, bignum_chunk)) {
      return false;
    } else {
      LIST_NEXT(p);
      LIST_NEXT(q);
      goto loop;
    }
  }
}

bool bignum_gt(bignum *a, bignum *b) {
  if (a->size < b->size) {
    return false;
  } else {
    list *p = a->chunks;
    list *q = b->chunks;
    
  loop:
    if (LIST_EMPTY(q)) {
      return true;
    } else if (LIST_EMPTY(p)) {
      return false;
    } else if (LIST_HEAD(p, bignum_chunk) <= LIST_HEAD(q, bignum_chunk)) {
      return false;
    } else {
      LIST_NEXT(p);
      LIST_NEXT(q);
      goto loop;
    }
  }
}

bool bignum_lteq(bignum *a, bignum *b) {
  return !bignum_gt(a, b);
}

bool bignum_gteq(bignum *a, bignum *b) {
  return !bignum_lt(a, b);
}



void bignum_divmod(bignum *a, bignum *d, bignum **quot, bignum **rem) {

}

char *bignum_to_str(const bignum *n) {

}
