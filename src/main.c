#include <stdbool.h>
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

static char* hashmap_tombstone = "tombstone";

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
} hashmap_slot_int_t;

typedef struct {
  hashmap_slot_int_t* slots;
  uint64_t capacity;
  uint64_t items_count;
} hashmap_int_t;

static inline hashmap_int_t hashmap_int_make() {
  return (hashmap_int_t){
      .slots = calloc(HASHMAP_MINIMUM_CAPACITY, sizeof(hashmap_slot_int_t)),
      .capacity = HASHMAP_MINIMUM_CAPACITY,
      .items_count = 0,
  };
}

static inline void hashmap_int_set_raw(hashmap_int_t* hashmap, const char* key,
                                       int value, uint64_t hash) {
  uint64_t position = hash % hashmap->capacity;

  uint64_t i = position;
  do {
    if (hashmap->slots[i].key == NULL ||
        hashmap->slots[i].key == hashmap_tombstone) {
      hashmap->slots[i] = (hashmap_slot_int_t){
          .key = key,
          .value = value,
          .hash = hash,
      };
      hashmap->items_count++;
      return;
    } else if (hashmap->slots[i].hash == hash) {
      hashmap->slots[i].value = value;
      return;
    }

    i = (i + 1 >= hashmap->capacity ? 0 : i + 1);
  } while (i != position);

  ZYRX_ASSERT(0, "Hashmap is out of space");
}

static inline void hashmap_int_set(hashmap_int_t* hashmap, const char* key,
                                   int value) {
  uint64_t hash = hashmap_make_hash(key);

  hashmap_int_set_raw(hashmap, key, value, hash);
}

static inline hashmap_slot_int_t* hashmap_int_get_raw(hashmap_int_t* hashmap,
                                                      uint64_t hash) {
  uint64_t position = hash % hashmap->capacity;

  uint64_t i = position;
  do {
    if (hashmap->slots[i].key == NULL) {
      break;
    }

    if (hashmap->slots[i].hash == hash &&
        hashmap->slots[i].key != hashmap_tombstone) {
      return &hashmap->slots[i];
    }

    i = (i + 1 >= hashmap->capacity ? 0 : i + 1);
  } while (i != position);

  ZYRX_ASSERT(0, "Value is not found in the hashmap");

  return (hashmap_slot_int_t*)NULL;
}

static inline int hashmap_int_get(hashmap_int_t* hashmap, const char* key) {
  uint64_t hash = hashmap_make_hash(key);

  return hashmap_int_get_raw(hashmap, hash)->value;
}

static inline void hashmap_int_resize(hashmap_int_t* hashmap, uint64_t size) {
  ZYRX_ASSERT(size >= hashmap->items_count, "Capacity is too small!");

  hashmap_slot_int_t* old_array = hashmap->slots;
  uint64_t old_capacity = hashmap->capacity;

  hashmap->slots =
      (hashmap_slot_int_t*)calloc(size, sizeof(hashmap_slot_int_t));
  hashmap->capacity = size;
  hashmap->items_count = 0;

  for (uint64_t i = 0; i < old_capacity; i++) {
    if (old_array[i].key != NULL && old_array[i].key != hashmap_tombstone) {
      hashmap_int_set_raw(hashmap, old_array[i].key, old_array[i].value,
                          old_array[i].hash);
    }
  }

  free(old_array);
}

static inline void hashmap_int_remove(hashmap_int_t* hashmap, const char* key) {
  uint64_t hash = hashmap_make_hash(key);

  *hashmap_int_get_raw(hashmap, hash) = (hashmap_slot_int_t){
      .key = hashmap_tombstone,
      .value = 0,
      .hash = 0,
  };
  hashmap->items_count--;
}

int main() {
  hashmap_int_t hm = hashmap_int_make();
  hashmap_int_set(&hm, "SomeKey", 25);
  hashmap_int_set(&hm, "Another key", 124);
  hashmap_int_set(&hm, "uknown value?", -1);

  hashmap_int_remove(&hm, "Another key");

  printf("%d", hashmap_int_get(&hm, "SomeKey"));
}
