#include "test.h"
#include "list.h"

bool test_list_sorted(list *l) {
  return LIST_EMPTY(l)
    || LIST_EMPTY(l->tail)
    || (LIST_HEAD(l, int) <= LIST_HEAD(l->tail, int)
        && test_list_sorted(l->tail));
}

bool int_less_eq(void *a, void *b) {
  return *(int *)a <= *(int *)b;
}

BEGIN_TEST

SHOW(list *l = NULL; int x = 1, y = 42, z = 91);
TEST(LIST_EMPTY(l));
SHOW(list_push(&z, &l); list_push(&y, &l); list_push(&x, &l));
TEST(test_list_sorted(l));
TEST(list_pop(&l) == &x);
TEST(*(int *)list_pop(&l) == 42);
TEST(!LIST_EMPTY(l));
SHOW(list_push(&y, &l); list_push(&z, &l));
SHOW(list_sort(&l, int_less_eq));
TEST(test_list_sorted(l));
TEST(*(int *)list_pop(&l) == 42);
TEST(list_pop(&l) == list_pop(&l));
TEST(LIST_EMPTY(l));
SHOW(list_push(&y, &l); list_push(&z, &l); list_free(l, NULL));

END_TEST
