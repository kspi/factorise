#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bignum.h"

static bignum *bignum_empty() {
  bignum *result = malloc(sizeof *result);
  result->digits = NULL;
  result->size = 0;
  result->allocated = 0;
  return result;
}

void bignum_add_digit(bignum *n, bignum_digit x) {
  n->size++;
  if (n->size > n->allocated) {
    n->allocated = (n->allocated * 2) || 1;
    n->digits = realloc(n->digits, n->allocated * (sizeof *n->digits));
  }
  n->digits[n->size - 1] = x;
}

void bignum_normalise(bignum *n) {
  int i = n->size - 1;
  while ((i >= 0) && !n->digits[i]) {
    --n->size;
    --i;
  }

  n->allocated = n->size;
  n->digits = realloc(n->digits, n->allocated);
}

bignum *bignum_from_digit(bignum_digit x) {
  bignum *n = bignum_empty();
  if (x) {
    bignum_add_digit(n, x);
    bignum_normalise(n);
  }
  return n;
}

void bignum_free(bignum *n) {
  free(n->digits);
  free(n);
}

bignum *bignum_copy(const bignum *n) {
  assert(n);
  bignum *new = bignum_empty();
  new->size = n->size;
  new->allocated = n->size;
  if (n->digits) {
    new->digits = malloc(new->allocated * (sizeof *new->digits));
    memcpy(new->digits, n->digits, new->allocated * (sizeof *new->digits));
  }
  return new;
}

bool bignum_eq(const bignum *a, const bignum *b) {
  if (a->size != b->size) {
    return false;
  } else {
    for (int i = 0; i < a->size; ++i) {
      if (a->digits[i] != b->digits[i]) return false;
    }
    return true;
  }
}

bool bignum_lt(const bignum *a, const bignum *b) {
  if (a->size < b->size) {
    return true;
  } else if ((a->size > b->size) || ((a->size == 0) && (b->size == 0))) {
    return false;
  } else {
    for (int i = a->size - 1; i >= 0; --i) {
      if (a->digits[i] >= b->digits[i]) return false;
    }    
    return true;
  }
}

bool bignum_gt(const bignum *a, const bignum *b) {
  if (a->size > b->size) {
    return true;
  } else if ((a->size < b->size) || ((a->size == 0) && (b->size == 0))) {
    return false;
  } else {
    for (int i = a->size - 1; i >= 0; --i) {
      if (a->digits[i] <= b->digits[i]) return false;
    }    
    return true;
  }
}

bool bignum_lteq(const bignum *a, const bignum *b) {
  return !bignum_gt(a, b);
}

bool bignum_gteq(const bignum *a, const bignum *b) {
  return !bignum_lt(a, b);
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

bignum *bignum_add(const bignum *a, const bignum *b) {
  bignum *result = bignum_empty();
  bignum_digit carry = 0;
  int largest_size = a->size > b->size ? a->size : b->size;
  for (int i = 0; i < largest_size + 1; ++i) {
    bignum_overflow sum = carry;
    if (i < a->size) sum += a->digits[i];
    if (i < b->size) sum += b->digits[i];
    bignum_add_digit(result, sum % BIGNUM_BASE);
    carry = sum / BIGNUM_BASE;
  }

  bignum_normalise(result);
  return result;
}

static void bignum_mini_divmod(bignum *n,
                               bignum_digit divisor,
                               bignum_digit *remainder)
{
  assert(divisor != 0);
  
  if (n->size == 0) {
    *remainder = 0;
  } else {
    *remainder = n->digits[0] % divisor;

    for (int i = 0; i < n->size; i++) {
      if (i > 0) {
        n->digits[i - 1] += (n->digits[i] % divisor) * BIGNUM_BASE / divisor;
      }
      n->digits[i] /= divisor;
    }
    
    bignum_normalise(n);
  }
}


char *bignum_to_str(const bignum *n) {
  unsigned int strsize = 32;
  char *str = malloc(strsize * sizeof *str);

  static const bignum_digit base = 10;
  static const char *digits = "0123456789";
  bignum *zero = bignum_from_digit(0);
  
  bignum *m = bignum_copy(n);
  
  unsigned int i = 0;
  while (bignum_gt(m, zero)) {
    bignum_digit rem;
    bignum_mini_divmod(m, base, &rem);

    if (i > strsize) {
      strsize *= 2;
      str = realloc(str, strsize * sizeof *str);
    }

    str[i] = digits[rem];
    ++i;
  }

  bignum_free(zero);
  bignum_free(m);

  str[i] = '\0';
  /* strsize = i + 1; */
  /* str = realloc(str, strsize * sizeof *str); */

  /* Reverse the number string. */
  for (unsigned int j = 0; j < i / 2; j++) {
    char t = str[j];
    str[j] = str[i - 1 - j];
    str[i - 1 - j] = t;
  }

  printf("%s\n", str);
  
  return str;
}
