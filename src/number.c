#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "number.h"

#define OUT_OF_RANGE_FORMAT ("Klaida: skaičius %s nepriklauso intervalui [" NUMFMT ", " NUMFMT "].\n")

number strtonum(const char *x) {
#if UNSIGNED
  while (*x == ' ') ++x;        /* Praleisti tarpus */
  if (*x == '-') {
    fprintf(stderr, OUT_OF_RANGE_FORMAT, x, NUMBER_MIN, NUMBER_MAX);
    exit(1);
  }
#endif
  
  errno = 0;
  char *endptr;
  number ret = strtoull(x, &endptr, 0);
  if ((errno == EINVAL) || (*x && *endptr)) {
    fprintf(stderr, "Netinkamas skaičius: %s\n", x);
    exit(1);
  } else if (errno == ERANGE) {
    fprintf(stderr, OUT_OF_RANGE_FORMAT, x, NUMBER_MIN, NUMBER_MAX);
    exit(1);
  }

  return ret;
}

number square(number x) {
  return x * x;
}

number ipow(number base, number degree) {
  /* Pakelia base laipsniu degree. O(log n) dauginimų. */
  
  number result = 1;
  while (degree > 0) {
    if (degree % 2) {
      result *= base;
      --degree;
    } else {
      base *= base;
      degree >>= 1;
    }
  } 
  return result;
}

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
