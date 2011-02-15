#include "test.h"
#include "list.h"

bool test_list_sorted(struct list *list) {
  return LIST_EMPTY(list)
    || LIST_EMPTY(list->tail)
    || (LIST_HEAD(list, int) <= LIST_HEAD(list->tail, int)
        && test_list_sorted(list->tail));
}

bool int_less_eq(void *a, void *b) {
  return *(int *)a <= *(int *)b;
}

BEGIN_TEST

SHOW(struct list *list = NULL; int x = 1, y = 42, z = 91);
TEST(LIST_EMPTY(list));
SHOW(list_push(&z, &list); list_push(&y, &list); list_push(&x, &list));
TEST(test_list_sorted(list));
TEST(list_pop(&list) == &x);
TEST(*(int *)list_pop(&list) == 42);
TEST(!LIST_EMPTY(list));
SHOW(list_push(&y, &list); list_push(&z, &list));
SHOW(list_sort(&list, int_less_eq));
TEST(test_list_sorted(list));
TEST(*(int *)list_pop(&list) == 42);
TEST(list_pop(&list) == list_pop(&list));
TEST(LIST_EMPTY(list));

END_TEST
