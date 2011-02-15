#include <stdio.h>
#include "factorise.h"

/*
 * Verbose factoring mode
 */

bool verbose_factoring = false;
#define VERBOSE_PRINT(...) do {                          \
    if (verbose_factoring) fprintf(stderr, __VA_ARGS__); \
  } while (0);


/*
 * Numeric functions.
 */

number square(number x) {
  return x * x;
}

number ipow(number base, number degree) {
  /* Raises the integer base to the degree power. O(log n) time. */
  
  number result = 1;
  while (degree > 0) {
    if (degree % 2) {           /* If degree is odd, just multiply. */
      result *= base;
      --degree;
    } else {                    /* If degree is even, b^n = (b^2)^(n/2). */
      base *= base;
      degree >>= 1;
    }
  } 
  return result;
}

number isqrt(number n) {
  number root = 0;
  
  /* Start guessing with the second to middle bit, since it is the
     largest bit that can be a square.

     For example, with 32 bit integers, sizeof(int32_t)*4-1 would be 15
     and 2^15 squared would be 2^30, but 2^16 squared would be 2^32,
     which is larger than 2^31-1, the maximum value of 32 bit
     integers.
  */
  number delta = ipow(2, sizeof(number) * 4 - 1);

  while (delta != 0) {
    if (square(root | delta) <= n) { /* If we can increase root by delta … */
      root |= delta;                 /* … do it. */
    }
    /* Also, + is equivalent to | here, because delta always has one
       bit set, and that bit is never set in root before the
       operation. */

    /* Try next bit: */
    delta >>= 1;
  }
  
  return root;
}

bool is_square(number x) {
  return x == square(isqrt(x));
}

void get_fermat_factors(number n, struct list **out, bool *prime) {
  /* The function accepts a natural number.

     If the number is composite, it put two of its (not necessarily
     prime, but >1) factors to *out and sets *prime to false (it prime
     is not NULL).
     
     Otherwise it sets *prime to true (even though 1 isn't prime).
  */

  VERBOSE_PRINT("Factoring " NUMFMT ": ", n);
  
  if (n <= 3) {
    /* No factors. */
    if (prime) *prime = true;
    VERBOSE_PRINT("no factors.\n");
  } else if ((n % 2) == 0) {
    /* Even number. */
    if (prime) *prime = false;
    push(2, out);
    push(n / 2, out);
    VERBOSE_PRINT(NUMFMT " and " NUMFMT ".\n", (number) 2, n / 2);
  } else {
    /* Search for factors. */
    number a = isqrt(n);
    number bsqr;

    unsigned int iterations = 0;
  loop:
    bsqr = square(a) - n;
    if (a > ((n - 3) / 2)) {
      /* End of iteration reached and no factors found. */
      if (prime) *prime = true;
      VERBOSE_PRINT("no factors (%u iterations).\n", iterations);
    } else if (is_square(bsqr)) {
      /* Factors found. */
      number b = isqrt(bsqr);
      if (prime) *prime = false;
      push(a + b, out);
      push(a - b, out);
      VERBOSE_PRINT(NUMFMT " and " NUMFMT ".\n", a + b, a - b);
    } else {
      /* Try the next pair of numbers. */
      ++a;
      ++iterations;
      goto loop;
    }
  }
}

struct list *factorise(number n) {
  struct list *prime_factors = NULL;
  struct list *factors = NULL;

#if UNSIGNED == 0
  if (n < -1) {
    push(-1, &prime_factors);
    n *= -1;
  }
#endif

  bool prime;
  get_fermat_factors(n, &factors, &prime);
  if (prime) {
    push(n, &prime_factors);
  } else {
    while (!LIST_EMPTY(factors)) {
      number x = pop(&factors);
      get_fermat_factors(x, &factors, &prime);
      if (prime) {
        push(x, &prime_factors);
      }
    }
  }

  return prime_factors;
}
