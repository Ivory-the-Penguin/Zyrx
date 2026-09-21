#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "build.h"
#include "core/list.h"

#define HASHMAP_SMALL_PRIME 53ull

static inline uint64_t hashmap_make_hash(const char* string, uint64_t size) {
  uint64_t hash_value = 0, small_prime_pow = 1;

  for (uint64_t i = 0; i < size; i++) {
    hash_value = hash_value + string[i] * small_prime_pow;
    small_prime_pow = HASHMAP_SMALL_PRIME * small_prime_pow;
  }

  return hash_value;
}

int main() {}
