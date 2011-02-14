#ifndef INCLUDED_factorise_h
#define INCLUDED_factorise_h

#include <stdbool.h>
#include "multiset.h"
#include "number.h"

extern bool verbose_factoring;
struct multiset *factorise(number n);

#endif
