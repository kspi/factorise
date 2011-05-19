#include <stdio.h>
#include <stdlib.h>
#include "factorise.h"
#include "list.h"

/*
 * Detalaus skaidymo režimas
 */

bool verbose_factoring = false;
#define VERBOSE_PRINT(...) do {                          \
    if (verbose_factoring) fprintf(stderr, __VA_ARGS__); \
  } while (0);

void get_fermat_factors(number n, list **out, bool *prime) {
  /* Funkcija priima natūralų skaičių n.

     Jei skaičius sudėtinis, tai funkcija įdeda du jo daliklius
     (nebūtinai pirminius, bet >1) į number'ų sąrašą *out ir nustato
     *prime į false, jei prime ne nulis.

     Jei skaičius pirminis arba 1, nustato *prime į true.
  */

  VERBOSE_PRINT("Skaidomas " NUMFMT ": ", n);
  
  if (n <= 3) {
    /* Nėra daliklių. */
    if (prime) *prime = true;
    VERBOSE_PRINT("nėra daliklių.\n");
  } else if ((n % 2) == 0) {
    /* Lyginis skaičius. */
    if (prime) *prime = false;
    list_push(2, out);
    list_push(n / 2, out);
    VERBOSE_PRINT(NUMFMT " ir " NUMFMT ".\n", (number) 2, n / 2);
  } else {
    /* Fermat algoritmas. */
    number a = isqrt(n);
    number bsqr;

    unsigned int iterations = 0;
  loop:
    bsqr = square(a) - n;
    if (a > ((n - 3) / 2)) {
      /* Pasiektas maksimalus iteracijų skaičius ir nerasti
         dalikliai. */
      if (prime) *prime = true;
      VERBOSE_PRINT("nėra daliklių (%u iter.).\n", iterations);
    } else if (is_square(bsqr)) {
      /* Rasti dalikliai. */
      number b = isqrt(bsqr);
      if (prime) *prime = false;
      list_push(a + b, out);
      list_push(a - b, out);
      VERBOSE_PRINT(NUMFMT " ir " NUMFMT ".\n", a + b, a - b);
    } else {
      /* Bandyti kitą skaičių porą. */
      ++a;
      ++iterations;
      goto loop;
    }
  }
}

multiset *factorise(number n) {
  multiset *prime_factors = NULL;
  list *factors = NULL;

#if UNSIGNED == 0
  if (n < -1) {
    multiset_insert(&prime_factors, -1);
    n *= -1;
  }
#endif

  bool prime;
  get_fermat_factors(n, &factors, &prime);
  if (prime) {
    multiset_insert(&prime_factors, n);
  } else {
    while (!LIST_EMPTY(factors)) {
      number x = list_pop(&factors);
      get_fermat_factors(x, &factors, &prime);
      if (prime) {
        multiset_insert(&prime_factors, x);
      }
    }
  }

  return prime_factors;
}
