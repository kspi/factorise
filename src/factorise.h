#ifndef INCLUDED_factorise_h
#define INCLUDED_factorise_h

#include <stdbool.h>
#include "number.h"
#include "multiset.h"

extern bool verbose_factoring;
multiset *factorise(number n);

#endif
