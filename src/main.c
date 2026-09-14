#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int *data;
  uint64_t length;
  uint64_t capacity;
} list_int_t;

void list_int_set_capacity(list_int_t *list, uint64_t capacity) {
  assert(list->length <= capacity &&
         "Capacity is smaller than the list length!");

  list->data = (int *)realloc(list->data, capacity * sizeof(int));
  assert(list->data != nullptr && capacity != 0 && "Allocation failed!");

  list->capacity = capacity;

  memset(list->data + list->length, 0, (capacity - list->length) * sizeof(int));
}

void list_int_push(list_int_t *list, int item) {
  if (list->capacity == 0) {
    list_int_set_capacity(list, 1);
  } else if (list->length == list->capacity) {
    list_int_set_capacity(list, list->capacity * 2);
  }

  list->data[list->length++] = item;
}

void list_int_free(list_int_t *list) {
  assert(list->data != nullptr);

  free((void *)list->data);
  list->length = 0;
  list->capacity = 0;
}

void list_int_pop(list_int_t *list) {
  assert(list->length > 0);

  list->data[--list->length] = 0;

  if (list->length < list->capacity / 4 && list->length > 4) {
    list_int_set_capacity(list, list->length * 2);
  }
}

static inline int *list_int_at(list_int_t *list, uint64_t index) {
  assert(index < list->length);
  return &list->data[index];
}

int main() {
  list_int_t list = {};

  for (int i = 1; i <= 10; i++) {
    list_int_push(&list, i);
  }

  list_int_pop(&list);

  for (uint64_t i = 0; i < list.length; i++) {
    printf("%d ", *list_int_at(&list, i));
  }

  printf("\nCapacity: %lu, Length: %lu", list.capacity, list.length);
}