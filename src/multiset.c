#include <stdlib.h>

#include "multiset.h"

void multiset_insert(multiset **s, number value) {
 loop:
  if (*s == NULL) {
    *s = malloc(sizeof **s);
    (*s)->value = value;
    (*s)->count = 1;
    (*s)->left = NULL;
    (*s)->right = NULL;
  } else if ((*s)->value == value) {
    ++(*s)->count;
  } else {
    if (value < (*s)->value) {
      s = &(*s)->left;
    } else {
      s = &(*s)->right;
    }
    goto loop;
  }
}

void multiset_foreach(multiset *s, multiset_foreach_fn f, void *f_data) {
  if (s) {
    multiset_foreach(s->left, f, f_data);
    f(s->value, s->count, f_data);
    multiset_foreach(s->right, f, f_data);
  }
}
