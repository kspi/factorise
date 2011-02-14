#include <stdlib.h>
#include "multiset.h"

void multiset_put(number n, struct multiset **multiset) {
 loop:
  if (*multiset) {
    if (n == (*multiset)->value) {
      (*multiset)->count++;
    } else if (n < (*multiset)->value) {
      multiset = &((*multiset)->left);
      goto loop;
    } else {
      multiset = &((*multiset)->right);
      goto loop;
    }      
  } else {
    *multiset = malloc(sizeof **multiset);
    (*multiset)->value = n;
    (*multiset)->count = 1;
    (*multiset)->left = NULL;
    (*multiset)->right = NULL;
  }
}

void multiset_to_list(struct multiset *multiset, struct list **out) {
  /* Returns a list of numbers in the multiset, in increasing
     order. */
  
  if (multiset) {
    multiset_to_list(multiset->right, out);
    push(multiset->value, out);
    multiset_to_list(multiset->left, out);
  }
}

void multiset_free(struct multiset *multiset) {
  if (multiset) {
    multiset_free(multiset->left);
    multiset_free(multiset->right);
    free(multiset);
  }
}
