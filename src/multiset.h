#ifndef INCLUDED_multiset_h
#define INCLUDED_multiset_h

#include <stdbool.h>
#include "number.h"

/*
 * multiset * — number tipo reikšmių multiaibė, įgyvendinta, kaip
 * dvejetainis paieškos medis.
 */

typedef struct multiset {
  number value;
  number count;
  struct multiset *left, *right;
} multiset;

void multiset_insert(multiset **s, number value);


/* Funkcija multiset_foreach kviečia funkciją f kiekvienai reikšmei
   multiaibėje, perduodama tai funkcijai reikšmę, tų reikšmių kiekį
   multiaibėje ir f_data, kurio ji pati nekeičia.

   f_data galima naudoti perduoti savo duomenims į funkciją f.

   Reikšmės aplankomos didėjimo tvarka. */
typedef void (*multiset_foreach_fn)(number value, number count, void *data);
void multiset_foreach(multiset *s, multiset_foreach_fn f, void *f_data);

#endif
