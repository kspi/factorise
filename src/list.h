#ifndef INCLUDED_list_h
#define INCLUDED_list_h

#include <stdbool.h>
#include "number.h"

/*
 * list * — vienkryptis number sąrašas
 */

typedef struct list {
  number value;
  struct list *tail;
} list;

#define LIST_EMPTY(l) (!(l))
#define LIST_NEXT(l) (l) = (l)->tail
#define LIST_HEAD(l) ((l)->value)
#define LIST_FOREACH(cell, l) \
  for (list *cell = (l); !LIST_EMPTY(cell); LIST_NEXT(cell))

void list_push(number value, list **l);
number list_pop(list **l);

void list_sort(list **l);

void list_free(list *l);

#endif
