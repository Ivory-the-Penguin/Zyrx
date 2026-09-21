#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"
#include "core/assert.h"
#include "core/list.h"

#define HASHMAP_SMALL_PRIME 53ull
#define HASHMAP_MINIMUM_CAPACITY 16

static inline uint64_t hashmap_make_hash(const char* string) {
  uint64_t hash_value = 0, small_prime_pow = 1;
  uint64_t size = strlen(string);

  for (uint64_t i = 0; i < size; i++) {
    hash_value = hash_value + string[i] * small_prime_pow;
    small_prime_pow = HASHMAP_SMALL_PRIME * small_prime_pow;
  }

  return hash_value;
}

typedef struct {
  const char* key;
  int value;
  uint64_t hash;
} hashmap_int_slot_t;

typedef struct {
  hashmap_int_slot_t* slots;
  uint64_t capacity;
} hashmap_int_t;

static inline hashmap_int_t hashmap_int_make() {
  return (hashmap_int_t){
      .slots = calloc(HASHMAP_MINIMUM_CAPACITY, sizeof(hashmap_int_slot_t)),
      .capacity = HASHMAP_MINIMUM_CAPACITY,
  };
}

static inline void hashmap_int_set(hashmap_int_t* hashmap, const char* key,
                                   int value) {
  uint64_t hash = hashmap_make_hash(key), position = hash % hashmap->capacity;

  if (hashmap->slots[position].key == NULL) {
    hashmap->slots[position] = (hashmap_int_slot_t){
        .key = key,
        .value = value,
        .hash = hash,
    };
    return;
  } else {
    ZYRX_ASSERT(0, "Hashmap collision!");
  }
}

int main() {
  hashmap_int_t hm = hashmap_int_make();
  hashmap_int_set(&hm, "SomeKey", 25);
}
