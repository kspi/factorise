#include "test.h"
#include "bignum.h"

BEGIN_TEST

SHOW(bignum *a = bignum_from_chunk(0));
SHOW(bignum *a2 = bignum_copy(a));
SHOW(bignum *b = bignum_from_chunk(1));
TEST(bignum_eq(a, a));
TEST(a != a2);
TEST(bignum_eq(a, a2));
TEST(!bignum_eq(a, b));
TEST(bignum_lt(a, b));
TEST(bignum_gt(b, a2));
TEST(bignum_lteq(a, a2));
SHOW(bignum_free(a); bignum_free(a2); bignum_free(b));

END_TEST
