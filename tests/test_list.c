#include "test.h"
#include "list.h"

BEGIN

SHOW(struct list *list = NULL);
TEST(LIST_EMPTY(list));
SHOW(push(42, &list));
TEST(pop(&list) == 42);
TEST(LIST_EMPTY(list));

END
