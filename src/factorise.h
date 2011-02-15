#ifndef INCLUDED_factorise_h
#define INCLUDED_factorise_h

#include <stdbool.h>
#include "list.h"
#include "number.h"

extern bool verbose_factoring;
struct list *factorise(number n);

#endif
