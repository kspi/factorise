#include <stdlib.h>
#include <stdio.h>
#include "bignum.h"

static bignum_digit *digit_new(bignum_digit x) {
  bignum_digit *result = malloc(sizeof *result);
  *result = x;
  return result;
}

static bignum_digit *digit_copy(bignum_digit *x) {
  return digit_new(*x);
}

static bignum *bignum_empty() {
  bignum *result = malloc(sizeof *result);
  result->digits = NULL;
  result->size = 0;
  return result;
}

bignum *bignum_from_digit(bignum_digit x) {
  bignum *n = bignum_empty();
  list_push(digit_new(x), &n->digits);
  n->size = 1;
  return n;
}

void bignum_free(bignum *n) {
  list_free(n->digits, free);
  free(n);
}

bignum *bignum_copy(bignum *n) {
  bignum *new = bignum_empty();
  new->size = n->size;
  new->digits = list_copy(n->digits, (copy_fn *)digit_copy);
  return new;
}

bool bignum_eq(bignum *a, bignum *b) {
  if (a->size != b->size) {
    return false;
  } else {
    list *p = a->digits;
    list *q = b->digits;
    
  loop:
    if (LIST_EMPTY(p) && LIST_EMPTY(q)) {
      return true;
    } else if (LIST_EMPTY(p) || LIST_EMPTY(q)) {
      return false;
    } else if (LIST_HEAD(p, bignum_digit) != LIST_HEAD(q, bignum_digit)) {
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
    list *p = a->digits;
    list *q = b->digits;
    
  loop:
    if (LIST_EMPTY(p)) {
      return true;
    } else if (LIST_EMPTY(q)) {
      return false;
    } else if (LIST_HEAD(p, bignum_digit) >= LIST_HEAD(q, bignum_digit)) {
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
    list *p = a->digits;
    list *q = b->digits;
    
  loop:
    if (LIST_EMPTY(q)) {
      return true;
    } else if (LIST_EMPTY(p)) {
      return false;
    } else if (LIST_HEAD(p, bignum_digit) <= LIST_HEAD(q, bignum_digit)) {
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
  list **p = &((*a)->digits);
  list **q = &(b->digits);
  
  bignum_digit carry = 0;

 loop:
  if (LIST_EMPTY(*p) && LIST_EMPTY(*q)) {
    /* Both lists are empty, so we just create a new digit for the
       carry, if there is any. */
    if (carry) {
      list *cell = malloc(sizeof *cell);
      cell->value = digit_new(carry);
      cell->tail = NULL;
      *p = cell;
      ++(*a)->size;
    }
  } else if (LIST_EMPTY(*p)) {
    /* Just p is empty, so we create a new digit for it. */
    bignum_overflow sum =
      ((bignum_overflow)carry) +
      ((bignum_overflow)LIST_HEAD(*q, bignum_digit));
    carry = (bignum_digit)(sum / BIGNUM_BASE);
    list *cell = malloc(sizeof *cell);
    cell->value = digit_new((bignum_digit)(sum % BIGNUM_BASE));
    cell->tail = NULL;
    *p = cell;
    ++(*a)->size;
    p = &((*p)->tail);
    goto loop;
  } else if (LIST_EMPTY(*q)) {
    /* Just q is empty, so we replace p's value with the new one. */
    bignum_overflow sum =
      ((bignum_overflow)carry) +
      ((bignum_overflow)LIST_HEAD(*p, bignum_digit));
    carry = (bignum_digit)(sum / BIGNUM_BASE);
    free((*p)->value);
    (*p)->value = digit_new((bignum_digit)(sum % BIGNUM_BASE));
    p = &((*p)->tail);
    goto loop;
  } else {
    bignum_overflow sum =
      ((bignum_overflow)carry) +
      ((bignum_overflow)LIST_HEAD(*p, bignum_digit)) +
      ((bignum_overflow)LIST_HEAD(*q, bignum_digit));
    carry = (bignum_digit)(sum / BIGNUM_BASE);
    free((*p)->value);
    (*p)->value = digit_new((bignum_digit)(sum % BIGNUM_BASE));
    p = &((*p)->tail);
    q = &((*q)->tail);
    goto loop;
  }
}

static void bignum_mini_divmod(bignum **n, bignum_digit divisor, bignum_digit *remainder) {
  
}

char *bignum_to_str(const bignum *n) {

}
