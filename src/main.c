#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "factorise.h"

/*
 * Interface functions.
 */

void usage(char *program) {
  printf("%s [-v] [-t] SKAIČIUS\n"
         "    -v — parodo skaidymo procesą.\n"
         "    -t — rezultatus sutraukti į laipsnius, TeX formatu.\n",
         program);
  exit(1);
}

void multiply_by_factor(number n, number deg, void *product) {
  *(number *)product *= ipow(n, deg);
}

bool tex_format = false;

void print_factor(number n, number deg, void *unused) {
  if (tex_format) {
    printf(NUMFMT, n);
    if (deg > 1) printf("^" NUMFMT, deg);
    printf(" ");
  } else {
    while (deg) {
      printf(NUMFMT " ", n);
      --deg;
    }
  }
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
      } else if (strncmp(argv[numidx], "-t", 3) == 0) {
        ++numidx;
        tex_format = true;
      } else {
        options_done = true;
      }
    }
    
    number n = strtonum(argv[numidx]);
    multiset *factors = factorise(n);

    number product = 1;
    multiset_foreach(factors, multiply_by_factor, &product);
    assert(product == n);

    multiset_foreach(factors, print_factor, NULL);
    printf("\n");
  }
  return 0;
}
