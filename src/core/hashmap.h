#ifndef ZYRX_HASHMAP_H
#define ZYRX_HASHMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "core/assert.h"

#define HASHMAP_OFFSET_BASIS 14695981039346656037ull
#define HASHMAP_PRIME 1099511628211ull
#define HASHMAP_MINIMUM_CAPACITY 16
#define HASHMAP_SIZE_INCREASE_THRESHOLD 0.7f
#define HASHMAP_SIZE_DECREASE_THRESHOLD 0.15f

static char* hashmap_tombstone = "tombstone";

#define DEFINE_HASHMAP(TYPE, TYPE_NAME)                                        \
  static inline uint64_t hashmap_make_hash(const char* string) {               \
    uint64_t hash = HASHMAP_OFFSET_BASIS;                                      \
    uint8_t* i = (uint8_t*)string;                                             \
                                                                               \
    while (*i != '\0') {                                                       \
      hash = hash ^ *i;                                                        \
      hash = hash * HASHMAP_PRIME;                                             \
      i++;                                                                     \
    }                                                                          \
                                                                               \
    return hash;                                                               \
  }                                                                            \
                                                                               \
  typedef struct {                                                             \
    const char* key;                                                           \
    TYPE value;                                                                \
    uint64_t hash;                                                             \
  } hashmap_##TYPE_NAME##_slot_t;                                              \
                                                                               \
  typedef struct {                                                             \
    hashmap_##TYPE_NAME##_slot_t* slots;                                       \
    uint64_t capacity;                                                         \
    uint64_t items_count;                                                      \
  } hashmap_##TYPE_NAME##_t;                                                   \
                                                                               \
  static inline hashmap_##TYPE_NAME##_t hashmap_##TYPE_NAME##_make() {         \
    return (hashmap_##TYPE_NAME##_t){                                          \
        .slots = calloc(HASHMAP_MINIMUM_CAPACITY,                              \
                        sizeof(hashmap_##TYPE_NAME##_slot_t)),                 \
        .capacity = HASHMAP_MINIMUM_CAPACITY,                                  \
        .items_count = 0,                                                      \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline void hashmap_##TYPE_NAME##_set_raw(                            \
      hashmap_##TYPE_NAME##_t* hashmap, const char* key, TYPE value,           \
      uint64_t hash) {                                                         \
    uint64_t position = hash % hashmap->capacity;                              \
                                                                               \
    uint64_t i = position, first_tombstone = 0;                                \
    bool found_tombstone = false;                                              \
    while (hashmap->slots[i].key != NULL) {                                    \
      if (hashmap->slots[i].key == hashmap_tombstone) {                        \
        if (!found_tombstone) {                                                \
          first_tombstone = i;                                                 \
          found_tombstone = true;                                              \
        }                                                                      \
      } else if (hashmap->slots[i].hash == hash) {                             \
        hashmap->slots[i].value = value;                                       \
        return;                                                                \
      }                                                                        \
                                                                               \
      i = (i + 1 >= hashmap->capacity ? 0 : i + 1);                            \
                                                                               \
      if (i == position) {                                                     \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (found_tombstone) {                                                     \
      hashmap->slots[first_tombstone] = (hashmap_##TYPE_NAME##_slot_t){        \
          .key = key,                                                          \
          .value = value,                                                      \
          .hash = hash,                                                        \
      };                                                                       \
      hashmap->items_count++;                                                  \
      return;                                                                  \
    }                                                                          \
                                                                               \
    if (hashmap->slots[i].key == NULL) {                                       \
      hashmap->slots[i] = (hashmap_##TYPE_NAME##_slot_t){                      \
          .key = key,                                                          \
          .value = value,                                                      \
          .hash = hash,                                                        \
      };                                                                       \
      hashmap->items_count++;                                                  \
      return;                                                                  \
    }                                                                          \
                                                                               \
    ZYRX_ASSERT(0, "Hashmap is out of space");                                 \
  }                                                                            \
                                                                               \
  static inline void hashmap_##TYPE_NAME##_resize(                             \
      hashmap_##TYPE_NAME##_t* hashmap, uint64_t size) {                       \
    ZYRX_ASSERT(size >= hashmap->items_count, "Capacity is too small!");       \
                                                                               \
    hashmap_##TYPE_NAME##_slot_t* old_array = hashmap->slots;                  \
    uint64_t old_capacity = hashmap->capacity;                                 \
                                                                               \
    hashmap->slots = (hashmap_##TYPE_NAME##_slot_t*)calloc(                    \
        size, sizeof(hashmap_##TYPE_NAME##_slot_t));                           \
    hashmap->capacity = size;                                                  \
    hashmap->items_count = 0;                                                  \
                                                                               \
    for (uint64_t i = 0; i < old_capacity; i++) {                              \
      if (old_array[i].key != NULL && old_array[i].key != hashmap_tombstone) { \
        hashmap_##TYPE_NAME##_set_raw(hashmap, old_array[i].key,               \
                                      old_array[i].value, old_array[i].hash);  \
      }                                                                        \
    }                                                                          \
                                                                               \
    free(old_array);                                                           \
  }                                                                            \
                                                                               \
  static inline void hashmap_##TYPE_NAME##_set(                                \
      hashmap_##TYPE_NAME##_t* hashmap, const char* key, TYPE value) {         \
    hashmap_##TYPE_NAME##_set_raw(hashmap, key, value,                         \
                                  hashmap_make_hash(key));                     \
                                                                               \
    if (((float)hashmap->items_count / hashmap->capacity) >                    \
        HASHMAP_SIZE_INCREASE_THRESHOLD) {                                     \
      hashmap_##TYPE_NAME##_resize(hashmap, hashmap->capacity * 2);            \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline hashmap_##TYPE_NAME##_slot_t* hashmap_##TYPE_NAME##_get_raw(   \
      hashmap_##TYPE_NAME##_t* hashmap, uint64_t hash) {                       \
    uint64_t position = hash % hashmap->capacity;                              \
                                                                               \
    uint64_t i = position;                                                     \
    do {                                                                       \
      if (hashmap->slots[i].key == NULL) {                                     \
        break;                                                                 \
      }                                                                        \
                                                                               \
      if (hashmap->slots[i].hash == hash &&                                    \
          hashmap->slots[i].key != hashmap_tombstone) {                        \
        return &hashmap->slots[i];                                             \
      }                                                                        \
                                                                               \
      i = (i + 1 >= hashmap->capacity ? 0 : i + 1);                            \
    } while (i != position);                                                   \
                                                                               \
    return (hashmap_##TYPE_NAME##_slot_t*)NULL;                                \
  }                                                                            \
                                                                               \
  static inline TYPE hashmap_##TYPE_NAME##_get(                                \
      hashmap_##TYPE_NAME##_t* hashmap, const char* key) {                     \
    hashmap_##TYPE_NAME##_slot_t* found_slot =                                 \
        hashmap_##TYPE_NAME##_get_raw(hashmap, hashmap_make_hash(key));        \
                                                                               \
    if (found_slot == NULL) {                                                  \
      ZYRX_ASSERT(0, "Value is not found in the hashmap");                     \
    } else {                                                                   \
      return found_slot->value;                                                \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void hashmap_##TYPE_NAME##_remove(                             \
      hashmap_##TYPE_NAME##_t* hashmap, const char* key) {                     \
    hashmap_##TYPE_NAME##_slot_t* found_slot =                                 \
        hashmap_##TYPE_NAME##_get_raw(hashmap, hashmap_make_hash(key));        \
                                                                               \
    if (found_slot == NULL) {                                                  \
      ZYRX_ASSERT(0, "Value is not found in the hashmap");                     \
      return;                                                                  \
    }                                                                          \
                                                                               \
    *found_slot = (hashmap_##TYPE_NAME##_slot_t){                              \
        .key = hashmap_tombstone,                                              \
        .value = 0,                                                            \
        .hash = 0,                                                             \
    };                                                                         \
    hashmap->items_count--;                                                    \
                                                                               \
    if (((float)hashmap->items_count / hashmap->capacity) <                    \
            HASHMAP_SIZE_DECREASE_THRESHOLD &&                                 \
        hashmap->capacity / 2 >= HASHMAP_MINIMUM_CAPACITY) {                   \
      hashmap_##TYPE_NAME##_resize(hashmap, hashmap->capacity / 2);            \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline bool hashmap_##TYPE_NAME##_exists(                             \
      hashmap_##TYPE_NAME##_t* hashmap, const char* key) {                     \
    return (hashmap_##TYPE_NAME##_get_raw(hashmap, hashmap_make_hash(key)) ==  \
                    NULL                                                       \
                ? false                                                        \
                : true);                                                       \
  }

#endif