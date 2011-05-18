#include <stdio.h>
#include <stdlib.h>
#include "factorise.h"

/*
 * Verbose factoring mode
 */

bool verbose_factoring = false;
#define VERBOSE_PRINT(...) do {                          \
    if (verbose_factoring) fprintf(stderr, __VA_ARGS__); \
  } while (0);


/*
 * Funkcijos darbui su skaičiais.
 */

number square(number x) {
  return x * x;
}

/* POW2 pakelia 2 x-uoju laipsniu. */
#define POW2(x) ((number)1 << (x))

number isqrt(number n) {
  number root = 0;
  
  /* Jeigu number sudaro k bitų, tai pradedame spelioti nuo 2^((k/2) -
     1). Tai skaičius, kur nustatytas vienas bitas, mažiau reikšmingas
     už vidurinįjį. Tai didžiausias skaičius, kurio kvadratas telpa į
     number.
  */
  number delta = POW2(sizeof delta * 4 - 1);

  while (delta != 0) {
    if (square(root | delta) <= n) { /* Rezultate yra delta bitas … */
      root |= delta;                 /* … tai nustatome jį root. */
    }
    /* Beje, delta visada turi vieną bitą, taigi galime vietoje +
       naudoti |. */

    /* Bandome sekantį bitą: */
    delta >>= 1;
  }
  
  return root;
}

bool is_square(number x) {
  return x == square(isqrt(x));
}

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
    list_push(new_num(2), out);
    list_push(new_num(n / 2), out);
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
      list_push(new_num(a + b), out);
      list_push(new_num(a - b), out);
      VERBOSE_PRINT(NUMFMT " ir " NUMFMT ".\n", a + b, a - b);
    } else {
      /* Bandyti kitą skaičių porą. */
      ++a;
      ++iterations;
      goto loop;
    }
  }
}

list *factorise(number n) {
  list *prime_factors = NULL;
  list *factors = NULL;

#if UNSIGNED == 0
  if (n < -1) {
    push(-1, &prime_factors);
    n *= -1;
  }
#endif

  bool prime;
  get_fermat_factors(n, &factors, &prime);
  if (prime) {
    list_push(new_num(n), &prime_factors);
  } else {
    while (!LIST_EMPTY(factors)) {
      number *x = list_pop(&factors);
      get_fermat_factors(*x, &factors, &prime);
      if (prime) {
        list_push(x, &prime_factors);
      } else {
        free(x);
      }
    }
  }

  return prime_factors;
}
