#ifndef INCLUDED_test_h
#define INCLUDED_test_h

#include <stdio.h>
#include <stdbool.h>

#define BEGIN_TEST                              \
  int main() {                                  \
  int __tests_result = 0;                       \
  printf(__FILE__ ":\n");

#define END_TEST                                \
  if (__tests_result == 0) {                    \
    printf("All tests passed.\n");              \
  } else {                                      \
    printf("SOME TESTS FAILED.\n");             \
  }                                             \
  return __tests_result;                        \
  }

#define SHOW(...)                               \
  printf("show: " #__VA_ARGS__ "\n");           \
  __VA_ARGS__;

#define TEST(condition)                         \
  if (condition) {                              \
    printf("pass: " #condition "\n");           \
  } else {                                      \
    printf("FAIL: " #condition "\n");           \
    ++__tests_result;                           \
  }

#endif
