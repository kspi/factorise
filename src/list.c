#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void list_push(void *value, list **l) {
  list *new_list = malloc(sizeof *new_list);
  new_list->value = value;
  new_list->tail = *l;
  *l = new_list;
}

void *list_pop(list **l) {
  void *value = (*l)->value;
  list *old_list = *l;
  *l = (*l)->tail;
  free(old_list);
  return value;
}  

static unsigned int list_skip(list **list, unsigned int n) {
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

void list_sort(list **l, sort_compare_fn less_eq) {
  /* Merge sort list in place in O(n log n) time and O(1) space. */
  
  unsigned int merges;          /* Amount of merges done last
                                   cycle. */
  unsigned int sublist_size = 1;
    
  do {
    /* Repeatedly merge sublists of size sublist_size into *list: */
    
    list *p, *q;                /* Left and right sublist pointers */
    unsigned int psize, qsize;  /* and their sizes. */
    
    p = *l;                     /* Start at the beginning of list. */

    list *tail = NULL;          /* The current last element of the
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
        
        list *elem;

        /* Pick the smaller one … */
        if (psize == 0) {       /* p empty, pick q */
          elem = q;
          list_skip(&q, 1);
          --qsize;
        } else if (qsize == 0 || LIST_EMPTY(q)) { /* q empty */
          elem = p;
          list_skip(&p, 1);
          --psize;          
        } else if (less_eq(p->value, q->value)) {
          /* p smaller or equal, pick p */
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
          *l = elem;
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
