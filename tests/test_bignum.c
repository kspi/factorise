#include "test.h"
#include "bignum.h"

BEGIN_TEST

SHOW(bignum *a = bignum_from_chunk(0));
SHOW(bignum *b = bignum_from_chunk(1));
TEST(bignum_eq(a, a));
TEST(!bignum_eq(a, b));

END_TEST
