#ifndef INCLUDED_list_h
#define INCLUDED_list_h

#include <stdbool.h>
#include "number.h"

/*
 * list * — singly linked list of numbers.
 */

typedef struct list {
  void *value;
  struct list *tail;
} list;

#define LIST_EMPTY(l) (!(l))
#define LIST_NEXT(l) (l) = (l)->tail
#define LIST_HEAD(l, elemtype) (*(elemtype *)(l)->value)
#define LIST_FOREACH(cell, l) \
  for (list *cell = (l); !LIST_EMPTY(cell); LIST_NEXT(cell))

void list_push(void *value, list **l);
void *list_pop(list **l);

typedef bool sort_compare_fn(void *, void *);
void list_sort(list **l, sort_compare_fn *less_eq);

typedef void free_fn(void *);
void list_free(list *l, free_fn *free_value);

#endif
