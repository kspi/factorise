#ifndef INCLUDED_multiset_h
#define INCLUDED_multiset_h

#include <stdbool.h>
#include "number.h"

/*
 * multiset * — number tipo reikšmių multiaibė, įgyvendinta, kaip
 * dvejetainis paieškos medis
 */

typedef struct multiset {
  number value;
  number count;
  struct multiset *left, *right;
} multiset;

void multiset_insert(multiset **s, number value);

typedef void (*multiset_foreach_fn)(number value, number count, void *data);
void multiset_foreach(multiset *s, multiset_foreach_fn f, void *f_data);

#endif
