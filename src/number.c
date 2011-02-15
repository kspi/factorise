#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "number.h"

number strtonum(const char *x) {
#if UNSIGNED
  while (*x == ' ') ++x;        /* skip spaces */
  if (*x == '-') {
    fprintf(stderr, "Number out of range [" NUMFMT ", " NUMFMT "]: %s\n",
            NUMBER_MIN, NUMBER_MAX, x);
    exit(1);
  }
#endif
  
  errno = 0;
  char *endptr;
  number ret = strtoull(x, &endptr, 0);
  if ((errno == EINVAL) || (*x && *endptr)) {
    fprintf(stderr, "Invalid number: %s\n", x);
    exit(1);
  } else if (errno == ERANGE) {
    fprintf(stderr, "Number out of range [" NUMFMT ", " NUMFMT "]: %s\n",
            NUMBER_MIN, NUMBER_MAX, x);
    exit(1);
  }

  return ret;
}

number *new_num(number n) {
  number *ret = malloc(sizeof *ret);
  *ret = n;
  return ret;
}

bool num_less_eq(void *a, void *b) {
  return *(number *)a <= *(number *)b;
}
