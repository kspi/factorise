#ifndef INCLUDED_list_h
#define INCLUDED_list_h

#include <stdbool.h>
#include "number.h"

/*
 * struct list* — singly linked list of numbers.
 */

struct list {
  void *value;
  struct list *tail;
};

#define LIST_EMPTY(list) (!(list))
#define LIST_NEXT(list) (list) = (list)->tail
#define LIST_HEAD(l, elemtype) (*(elemtype *)(l)->value)
#define LIST_FOREACH(cell, l) \
  for (struct list *cell = (l); !LIST_EMPTY(cell); LIST_NEXT(cell))

void list_push(void *value, struct list **list);
void *list_pop(struct list **list);

typedef bool (*sort_compare_fn)(void *, void *);
void list_sort(struct list **list, sort_compare_fn less_eq);

#endif
