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

static unsigned int list_skip(struct list **list, unsigned int n) {
  /* Advances list by n elements or until the end.

     Returns the actual number of elements skipped. */

  unsigned int skipped = 0;
  while (n && *list) {
    *list = (*list)->tail;
    --n;
    ++skipped;
  }
  return skipped;
}

void list_sort(struct list **list) {
  /* Merge sort list in place in O(n log n) time and O(1) space. */
  
  unsigned int merges;          /* Amount of merges done last
                                   cycle. */
  unsigned int sublist_size = 1;
    
  do {
    /* Repeatedly merge sublists of size sublist_size into *list: */
    
    struct list *p, *q;         /* Left and right sublist pointers */
    unsigned int psize, qsize;  /* and their sizes. */
    
    p = *list;                  /* Start at the beginning of list. */

    struct list *tail = NULL;   /* The current last element of the
                                   list that we are building up. */

    merges = 0;
    while (!LIST_EMPTY(p)) {    /* Merge until left pointer reaches end. */
      ++merges;

      q = p;
      psize = list_skip(&q, sublist_size); /* p continues until q */
      qsize = sublist_size;                /* q continues until end or
                                              sublist_size */

      while ((psize > 0) || ((qsize > 0) && (!LIST_EMPTY(q)))) {
        /* While at least one list has elements: */
        
        struct list *elem;

        /* Pick the smaller one … */
        if (psize == 0) {       /* p empty, pick q */
          elem = q;
          list_skip(&q, 1);
          --qsize;
        } else if (qsize == 0 || LIST_EMPTY(q)) { /* q empty */
          elem = p;
          list_skip(&p, 1);
          --psize;          
        } else if (p->value <= q->value) { /* p smaller or equal, pick p */
          elem = p;
          list_skip(&p, 1);
          --psize;          
        } else {                /* q smaller */
          elem = q;
          list_skip(&q, 1);
          --qsize;
        }

        /* … append it to the new list. */
        if (LIST_EMPTY(tail)) {
          *list = elem;
        } else {
          tail->tail = elem;
        }
        tail = elem;
      }

      /* q either points to the first element of the next sublist or
         the end of the list. Either way, advance p there: */
      p = q;
    }
    tail->tail = NULL;
    sublist_size *= 2;
  } while (merges > 1);
}
