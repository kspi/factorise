#include <stdlib.h>
#include "test.h"
#include "list.h"

bool test_list_sorted(list *l) {
  return LIST_EMPTY(l)
    || LIST_EMPTY(l->tail)
    || (LIST_HEAD(l) <= LIST_HEAD(l->tail)
        && test_list_sorted(l->tail));
}

bool int_less_eq(void *a, void *b) {
  return *(int *)a <= *(int *)b;
}

int *int_copy(int *x) {
  int *newint = malloc(sizeof *newint);
  *newint = *x;
  return newint;
}

BEGIN_TEST

SHOW(list *l = NULL);
TEST(LIST_EMPTY(l));
SHOW(list_push(3, &l); list_push(2, &l); list_push(1, &l));
TEST(test_list_sorted(l));
TEST(list_pop(&l) == 1);
TEST(list_pop(&l) == 2);
TEST(!LIST_EMPTY(l));
SHOW(list_push(5, &l); list_push(4, &l));
TEST(LIST_HEAD(l) == 4);
SHOW(list_sort(&l));
TEST(test_list_sorted(l));
TEST(list_pop(&l) == 3);
TEST(list_pop(&l) == 4);
TEST(list_pop(&l) == 5);
TEST(LIST_EMPTY(l));

END_TEST
