#include "test.h"
#include "multiset.h"

BEGIN_TEST

SHOW(multiset *m = NULL);
SHOW(multiset_insert(&m, 5));
SHOW(multiset_insert(&m, 3));
SHOW(multiset_insert(&m, 7));
SHOW(multiset_insert(&m, 3));
SHOW(multiset_insert(&m, 5));
TEST(m->value == 5);
TEST(m->count == 2);
TEST(m->left->value == 3);
TEST(m->left->count == 2);
TEST(m->right->value == 7);
TEST(m->right->count == 1);


END_TEST
