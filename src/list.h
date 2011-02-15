#ifndef INCLUDED_list_h
#define INCLUDED_list_h

#include "number.h"

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

void push(number x, struct list **list);
number pop(struct list **list);
void list_print(struct list *list);
void list_sort(struct list **list);

#endif
