#include "build.h"
#include "core/list.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

DEFINE_LIST(int32_t, int)

int main() {
  list_int_t list = list_int_make();
  list_int_t list2 = list_int_make();

  for (int i = 1; i <= 10; i++) {
    list_int_push(&list, i);
  }

  for (int i = -100; i < 0; i += 10) {
    list_int_push(&list2, i);
  }

  list_int_pop(&list);

  list_int_view_t view = list_int_view_make(&list);

  list_int_view_chop_left(&view, 1);
  list_int_view_chop_right(&view, 5);

  list_int_combine(&list, &list2);
  list_int_combine_view(&list, &view);

  view = list_int_view_make(&list);

  list_int_combine_view(&list, &view);

  // LIST_FOREACH(view, i) { printf("%d ", *list_int_view_at(&view, i)); }
  LIST_FOREACH(list, i) { printf("%d ", *list_int_at(&list, i)); }

  list_int_free(&list);
}