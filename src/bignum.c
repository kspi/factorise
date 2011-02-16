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

static bignum *bignum_empty() {
  bignum *result = malloc(sizeof *result);
  result->chunks = NULL;
  result->size = 0;
  return result;
}

bignum *bignum_from_chunk(bignum_chunk x) {
  bignum *n = bignum_empty();
  list_push(chunk_new(x), &n->chunks);
  n->size = 1;
  return n;
}

void bignum_free(bignum *n) {
  list_free(n->chunks, free);
  free(n);
}

bignum *bignum_copy(bignum *n) {
  bignum *new = bignum_empty();
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

void bignum_add(bignum **a, bignum *b) {
  list **p = &((*a)->chunks);
  list **q = &(b->chunks);
  
  bignum_chunk carry = 0;

 loop:
  if (LIST_EMPTY(*p) && LIST_EMPTY(*q)) {
    /* Both lists are empty, so we just create a new chunk for the
       carry, if there is any. */
    if (carry) {
      list *chunk = malloc(sizeof *chunk);
      chunk->value = chunk_new(carry);
      chunk->tail = NULL;
      *p = chunk;
      ++(*a)->size;
    }
  } else if (LIST_EMPTY(*p)) {
    /* Just p is empty, so we create a new chunk for it. */
    bignum_larger_chunk sum =
      ((bignum_larger_chunk)carry) +
      ((bignum_larger_chunk)LIST_HEAD(*q, bignum_chunk));
    carry = (bignum_chunk)(sum / BIGNUM_CHUNK_MODULO);
    list *chunk = malloc(sizeof *chunk);
    chunk->value = chunk_new((bignum_chunk)(sum % BIGNUM_CHUNK_MODULO));
    chunk->tail = NULL;
    *p = chunk;
    ++(*a)->size;
    p = &((*p)->tail);
    goto loop;
  } else if (LIST_EMPTY(*q)) {
    /* Just q is empty, so we replace p's value with the new one. */
    bignum_larger_chunk sum =
      ((bignum_larger_chunk)carry) +
      ((bignum_larger_chunk)LIST_HEAD(*p, bignum_chunk));
    carry = (bignum_chunk)(sum / BIGNUM_CHUNK_MODULO);
    free((*p)->value);
    (*p)->value = chunk_new((bignum_chunk)(sum % BIGNUM_CHUNK_MODULO));
    p = &((*p)->tail);
    goto loop;
  } else {
    bignum_larger_chunk sum =
      ((bignum_larger_chunk)carry) +
      ((bignum_larger_chunk)LIST_HEAD(*p, bignum_chunk)) +
      ((bignum_larger_chunk)LIST_HEAD(*q, bignum_chunk));
    carry = (bignum_chunk)(sum / BIGNUM_CHUNK_MODULO);
    free((*p)->value);
    (*p)->value = chunk_new((bignum_chunk)(sum % BIGNUM_CHUNK_MODULO));
    p = &((*p)->tail);
    q = &((*q)->tail);
    goto loop;
  }
}

void bignum_divmod(bignum *a, bignum *d, bignum **quot, bignum **rem) {

}

char *bignum_to_str(const bignum *n) {

}
