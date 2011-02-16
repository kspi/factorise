#include <string.h>
#include "test.h"
#include "bignum.h"

BEGIN_TEST

SHOW(bignum *a = bignum_from_digit(0));
SHOW(bignum *a2 = bignum_copy(a));
SHOW(bignum *b = bignum_from_digit(1));
TEST(strcmp(bignum_to_str(b), "1") == 0);
TEST(bignum_eq(a, a));
TEST(a != a2);
TEST(bignum_eq(a, a2));
TEST(!bignum_eq(a, b));
TEST(bignum_lt(a, b));
TEST(bignum_gt(b, a2));
TEST(bignum_lteq(a, a2));
SHOW(bignum_free(a); bignum_free(a2); bignum_free(b));
SHOW(bignum *c = bignum_from_digit(2147483648)); /* 2^31 */
TEST(strcmp(bignum_to_str(c), "2147483648") == 0);
SHOW(bignum_add(&c, c); bignum_add(&c, c));
/* 2^31 * 4 = 2^33 = 0 + 2 * 2^32 = 20 (base 32) */
TEST(c->size == 2);             /* two digits in base 32 */
TEST(LIST_HEAD(c->digits, bignum_digit) == 0);
TEST(LIST_HEAD(c->digits->tail, bignum_digit) == 2);

END_TEST
