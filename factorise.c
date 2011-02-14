#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * number — Abstract integer type.
 */

typedef unsigned long long number;
#define NUMFMT "%llu"
#define NUMBER_MIN ((number)0)
#define NUMBER_MAX ULLONG_MAX
#define UNSIGNED 1

number strtonum(const char *x) {
#if UNSIGNED == 1
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


/*
 * struct list* — singly linked list of numbers.
 */

struct list {
  number value;
  struct list *tail;
};

#define LIST_EMPTY(list) (!(list))
#define LIST_NEXT(list) (list) = (list)->tail
#define LIST_FOREACH(cell, l) \
  for (struct list *cell = (l); !LIST_EMPTY(cell); LIST_NEXT(cell))

void push(number x, struct list **list) {
  struct list *new_head = malloc(sizeof *new_head);
  new_head->value = x;
  new_head->tail = *list;
  *list = new_head;
}

number pop(struct list **list) {
  number value = (*list)->value;
  struct list *old_head = *list;
  *list = (*list)->tail;
  free(old_head);
  return value;
}
  

void list_print(struct list *list) {
  LIST_FOREACH(cell, list) {
    printf(NUMFMT, cell->value);
    if (cell->tail) printf(" ");
  }
  printf("\n");
}

void list_reverse(struct list **list) {
  /* Reverses a list in place, O(n). */
  
  struct list *cur, *next, *prev;
  prev = NULL;
  cur = *list;
  while (!LIST_EMPTY(cur)) {
    next = cur->tail;
    cur->tail = prev;
    prev = cur;
    cur = next;
  }
  *list = prev;
}


/*
 * struct bag* — bag (multiset) of numbers, implemented as a binary
 * tree.
 */

struct bag {
  number value;
  unsigned int count;
  struct bag *left, *right;
};

void bag_put(number n, struct bag **bag) {
 loop:
  if (*bag) {
    if (n == (*bag)->value) {
      (*bag)->count++;
    } else if (n < (*bag)->value) {
      bag = &((*bag)->left);
      goto loop;
    } else {
      bag = &((*bag)->right);
      goto loop;
    }      
  } else {
    *bag = malloc(sizeof **bag);
    (*bag)->value = n;
    (*bag)->count = 1;
    (*bag)->left = NULL;
    (*bag)->right = NULL;
  }
}

void bag_print(struct bag *bag) {
  if (bag) {
    bag_print(bag->left);
    for (unsigned int i = 0; i < bag->count; i++) {
      printf(NUMFMT " ", bag->value);
    }
    bag_print(bag->right);
  }
}


/*
 * Verbose factoring mode
 */

static bool verbose_factoring = false;
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
    
  loop:
    bsqr = square(a) - n;
    if (a > ((n - 3) / 2)) {
      /* End of iteration reached and no factors found. */
      if (prime) *prime = true;
      VERBOSE_PRINT("no factors.\n");
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
      goto loop;
    }
  }
}

struct bag *factorise(number n) {
  struct bag *prime_factors = NULL;
  struct list *factors = NULL;

#if UNSIGNED == 0
  if (n < -1) {
    bag_put(-1, &prime_factors);
    n *= -1;
  }
#endif

  bool prime;
  get_fermat_factors(n, &factors, &prime);
  if (prime) {
    bag_put(n, &prime_factors);
  } else {
    while (!LIST_EMPTY(factors)) {
      number x = pop(&factors);
      get_fermat_factors(x, &factors, &prime);
      if (prime) {
        bag_put(x, &prime_factors);
      }
    }
  }

  return prime_factors;
}


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
    int numidx = 1;
    if (strncmp(argv[1], "-v", 3) == 0) {
      ++numidx;
      verbose_factoring = true;
    }
    number n = strtonum(argv[numidx]);
    struct bag *factors = factorise(n);
    bag_print(factors);
  }
  return 0;
}
