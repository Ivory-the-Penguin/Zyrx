#include "build.h"
#include "core/list.h"
#include <stdio.h>

DEFINE_ARRAY(int)
IMPL_ARRAY(int)

int main() {
  list_int_t list = {0};

  for (int i = 1; i <= 10; i++) {
    list_int_push(&list, i);
  }

  list_int_pop(&list);

  for (uint64_t i = 0; i < list.length; i++) {
    printf("%d ", *list_int_at(&list, i));
  }

  printf("\nCapacity: %lu, Length: %lu", list.capacity, list.length);
}