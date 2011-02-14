#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "factorise.h"

/*
 * Interface functions.
 */

void usage(char *program) {
  fprintf(stderr,
          "Usage: %s [-v] NUMBER\n"
          "  -v shows the factoring process.\n",
          program);
  exit(1);
}

void sanity_check(number n, struct list *factors) {
  number product = 1;

  LIST_FOREACH(cell, factors) {
    product *= cell->value;
  }
    
  if (product != n) {
    fprintf(stderr, "Logic error: product of factors is " NUMFMT "\n",
            product);
    exit(1);
  }
}

int main(int argc, char **argv) {
  if (argc <= 1 || strncmp(argv[1], "-h", 3) == 0) {
    usage(argv[0]);
  } else {
    unsigned int numidx = 1;
    bool options_done = false;
    while (!options_done) {
      if (strncmp(argv[1], "-v", 3) == 0) {
        ++numidx;
        verbose_factoring = true;
      } else if (strncmp(argv[1], "-t", 3) == 0) {
        ++numidx;
        /* tex_output = true; */
      } else {
        options_done = true;
      }
    }
    
    number n = strtonum(argv[numidx]);
    struct multiset *factor_set = factorise(n);
    struct list *factors = NULL;
    multiset_to_list(factor_set, &factors);
    multiset_free(factor_set);
    sanity_check(n, factors);
    list_print(factors);
  }
  return 0;
}
