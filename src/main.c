#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "vec/vec.h"

int main() {
  vec_int_t test;
  vec_init(&test);

  vec_push(&test, 100);
  vec_push(&test, 300);
  vec_push(&test, 600);

  printf("%d\n", test.data[0]);

  vec_deinit(&test);
}
