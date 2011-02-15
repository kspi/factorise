#ifndef INCLUDED_test_h
#define INCLUDED_test_h

#include <stdio.h>
#include <stdbool.h>

#define BEGIN                                   \
  int main() {                                  \
  int __tests_result = 0;                       \
  printf(__FILE__ ":\n");

#define END return __tests_result; }

#define SHOW(statement)                         \
  printf("Show: " #statement "\n");             \
  statement;

#define TEST(condition)                         \
  if (condition) {                              \
    printf("Pass: " #condition "\n");           \
  } else {                                      \
    printf("FAIL: " #condition "\n");           \
    ++__tests_result;                           \
  }

#endif
