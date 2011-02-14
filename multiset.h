#ifndef INCLUDED_multiset_h
#define INCLUDED_multiset_h

#include "number.h"
#include "list.h"

/*
 * struct multiset* — multiset of numbers, implemented as a binary
 * tree.
 */

struct multiset {
  number value;
  unsigned int count;
  struct multiset *left, *right;
};

void multiset_put(number n, struct multiset **multiset);
void multiset_to_list(struct multiset *multiset, struct list **out);
void multiset_free(struct multiset *multiset);

#endif
