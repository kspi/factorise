#include "test.h"
#include "list.h"

BEGIN

SHOW(struct list *list = NULL; int x = 42);
TEST(LIST_EMPTY(list) && x == 42);
SHOW(list_push(&x, &list));
SHOW(list_push(&x, &list));
TEST(list_pop(&list) == &x);
TEST(*(int *)list_pop(&list) == 42);
TEST(LIST_EMPTY(list));

END
