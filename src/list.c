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
  /* Paslenka *list per n elementų į priekį arba iki sąrašo galo.

     Grąžina, per kiek elementų paslinko. */

  unsigned int skipped = 0;
  while (n && *list) {
    *list = (*list)->tail;
    --n;
    ++skipped;
  }
  return skipped;
}

void list_sort(list **l, sort_compare_fn *less_eq) {
  /* Merge sort algoritmu surūšiuoja sąrašus vietoje. Naudoja O(1)
     atminties ir O(n log n) operacijų. */
  
  unsigned int merges;          /* Kiek sujungimų (merge) padaryta
                                   paskutiniame cikle. */
  unsigned int sublist_size = 1;
    
  do {
    /* sublist_size dydžio sąrašo dalis jungti į *l: */
    
    list *p, *q;                /* Kairys ir dešinys posąrašis */
    unsigned int psize, qsize;  /* ir jų dydžiai. */
    
    p = *l;                     /* Pradedame nuo *l pradžios. */

    list *tail = NULL;          /* Sąrašo, kurį dabar statome, paskutinis elementas. */

    merges = 0;
    while (!LIST_EMPTY(p)) {    /* Jungiame, kol kairiojo posąrašio
                                   rodyklė pasieka galą. */
      ++merges;

      q = p;
      psize = list_skip(&q, sublist_size); /* p tęsiasi iki q */
      qsize = sublist_size;                /* q tęsiasi iki galo arba iki
                                              sublist_size */

      while ((psize > 0) || ((qsize > 0) && (!LIST_EMPTY(q)))) {
        /* Kol bent viename posąrašyje yra elementų. */
        
        list *elem;

        /* Išrenkame mažesnįjį … */
        if (psize == 0) {       /* p tuščias, išrenkam iš q */
          elem = q;
          list_skip(&q, 1);
          --qsize;
        } else if (qsize == 0 || LIST_EMPTY(q)) { /* q tuščias */
          elem = p;
          list_skip(&p, 1);
          --psize;          
        } else if (less_eq(p->value, q->value)) {
          /* p mažesnis arba lygus, išrenkam p (kad rūšiavimas būtų stabilus) */
          elem = p;
          list_skip(&p, 1);
          --psize;          
        } else {                /* q mažesnis */
          elem = q;
          list_skip(&q, 1);
          --qsize;
        }

        /* … ir prikabiname jį prie naujojo sąrašo galo. */
        if (LIST_EMPTY(tail)) {
          *l = elem;
        } else {
          tail->tail = elem;
        }
        tail = elem;            /* Jis tampa naujuoju galu, žinoma. */
      }

      /* Dabar q rodo arba į pirmąjį kito posąrašio elementą arba į
         sąrašo galą. Vienaip ar kitaip nustatome p į ten. */
      p = q;
    }
    tail->tail = NULL;
    sublist_size *= 2;
  } while (merges > 1);
}

void list_free(list *l, free_fn *free_value) {
  /* Atlaisvina sąrašui išskirta atmintį. Jei free_value ne nulis, tai
     naudoja tą funkciją atlaisvinti sąrašo reikšmėms. */

  while (!LIST_EMPTY(l)) {
    if (free_value) free_value(l->value);
    list *old_cell = l;
    LIST_NEXT(l);
    free(old_cell);
  }
}

list *list_copy(list *l, copy_fn *copy_value) {
  list *new_list = NULL;
  list *new_list_tail;
  LIST_FOREACH(cell, l) {
    if (LIST_EMPTY(new_list)) {
      new_list = malloc(sizeof *new_list);
      if (copy_value) {
        new_list->value = copy_value(cell->value);
      } else {
        new_list->value = cell->value;
      }
      new_list_tail = new_list;
    } else {
      new_list_tail->tail = malloc(sizeof *new_list_tail);
      LIST_NEXT(new_list_tail);
      if (copy_value) {
        new_list_tail->value = copy_value(cell->value);
      } else {
        new_list_tail->value = cell->value;
      }
    }
  }
  new_list_tail->tail = NULL;
  return new_list;
}
