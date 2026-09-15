#include "build.h"
#include "core/list.h"
#include <stdio.h>

DEFINE_ARRAY(int32_t, int)
IMPL_ARRAY(int32_t, int)

int main() {
  list_int_t *list = &(list_int_t){0};

  for (int i = 1; i <= 10; i++) {
    list_int_push(list, i);
  }

  list_int_pop(list);

  list_int_view_t view = list_int_view_make(list);

  list_int_view_chop_left(&view, 1);
  list_int_view_chop_right(&view, 5);

  LIST_FOREACH(*list, i) { printf("%d ", *list_int_at(list, i)); }

  printf("\nOffset: %lu, Length: %lu", view.offset, view.length);

  list_int_free(list);
}