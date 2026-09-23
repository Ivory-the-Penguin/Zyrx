#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"
#include "core/hashmap.h"
#include "core/list.h"

DEFINE_HASHMAP(int32_t, int)

int main() {
  hashmap_int_t hm = hashmap_int_make();
  hashmap_int_set(&hm, "SomeKey", 25);
  hashmap_int_set(&hm, "Another key", 124);
  hashmap_int_set(&hm, "uknown value?", -1);
  hashmap_int_set(&hm, "Might resize", -1);

  hashmap_int_remove(&hm, "Another key");

  printf("%d", hashmap_int_get(&hm, "SomeKey"));
}
