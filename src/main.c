#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"
#include "core/assert.h"
#include "core/list.h"

#define HASHMAP_OFFSET_BASIS 14695981039346656037ull
#define HASHMAP_PRIME 1099511628211ull
#define HASHMAP_MINIMUM_CAPACITY 5

static inline uint64_t hashmap_make_hash(const char* string) {
  uint64_t hash = HASHMAP_OFFSET_BASIS;
  uint8_t* i = (uint8_t*)string;

  while (*i != '\0') {
    hash = hash ^ *i;
    hash = hash * HASHMAP_PRIME;
    i++;
  }

  return hash;
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

  ZYRX_ASSERT(hashmap->slots[position].hash != hash,
              "Can't have same strings as the key");

  if (hashmap->slots[position].key == NULL) {
    hashmap->slots[position] = (hashmap_int_slot_t){
        .key = key,
        .value = value,
        .hash = hash,
    };
    return;
  } else {
    for (uint64_t i = (position + 1) % hashmap->capacity; i != position;
         i = (i + 1 >= hashmap->capacity ? 0 : i + 1)) {
      ZYRX_ASSERT(hashmap->slots[i].hash != hash,
                  "Can't have same strings as the key");

      if (hashmap->slots[i].key == NULL) {
        hashmap->slots[i] = (hashmap_int_slot_t){
            .key = key,
            .value = value,
            .hash = hash,
        };
        return;
      }
    }

    ZYRX_ASSERT(0, "Hashmap is out of space");
  }
}

static inline int hashmap_int_get(hashmap_int_t* hashmap, const char* key) {
  uint64_t hash = hashmap_make_hash(key), position = hash % hashmap->capacity;

  if (hashmap->slots[position].key != NULL &&
      hashmap->slots[position].hash == hash) {
    return hashmap->slots[position].value;
  } else {
    for (uint64_t i = (position + 1) % hashmap->capacity; i != position;
         i = (i + 1 >= hashmap->capacity ? 0 : i + 1)) {
      if (hashmap->slots[i].key != NULL && hashmap->slots[i].hash == hash) {
        return hashmap->slots[i].value;
      }
    }

    ZYRX_ASSERT(0, "Value is not found in the hashmap");
  }
}

int main() {
  hashmap_int_t hm = hashmap_int_make();
  hashmap_int_set(&hm, "SomeKey", 25);
  hashmap_int_set(&hm, "Another key", 124);
  hashmap_int_set(&hm, "uknown value?", -1);

  printf("%d", hashmap_int_get(&hm, "Another key"));
}
