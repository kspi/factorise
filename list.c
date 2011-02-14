#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void push(number x, struct list **list) {
  struct list *new_head = malloc(sizeof *new_head);
  new_head->value = x;
  new_head->tail = *list;
  *list = new_head;
}

number pop(struct list **list) {
  number value = (*list)->value;
  struct list *old_head = *list;
  *list = (*list)->tail;
  free(old_head);
  return value;
}  

void list_print(struct list *list) {
  LIST_FOREACH(cell, list) {
    printf(NUMFMT, cell->value);
    if (cell->tail) printf(" ");
  }
  printf("\n");
}
