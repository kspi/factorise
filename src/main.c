#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "factorise.h"

/*
 * Interface functions.
 */

void usage(char *program) {
  fprintf(stderr,
          "%s [-v] SKAIČIUS\n"
          "\n"
          "Parametras -v parodo skaidymo procesą.\n",
          program);
  exit(1);
}

void sanity_check(number n, list *factors) {
  number product = 1;

  LIST_FOREACH(cell, factors) {
    product *= LIST_HEAD(cell, number);
  }
    
  assert(product == n);
}

int main(int argc, char **argv) {
  if (argc <= 1 || strncmp(argv[1], "-h", 3) == 0) {
    usage(argv[0]);
  } else {
    unsigned int numidx = 1;
    bool options_done = false;
    while (!options_done) {
      if (strncmp(argv[numidx], "-v", 3) == 0) {
        ++numidx;
        verbose_factoring = true;
      } else {
        options_done = true;
      }
    }
    
    number n = strtonum(argv[numidx]);
    list *factors = factorise(n);
    list_sort(&factors, num_less_eq);
    LIST_FOREACH(cell, factors) {
      printf(NUMFMT, LIST_HEAD(cell, number));
      if (!LIST_EMPTY(cell->tail)) {
        printf(" ");
      } else {
        printf("\n");
      }
    }
    sanity_check(n, factors);
  }
  return 0;
}
