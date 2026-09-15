#ifndef ZYRX_STRING_H
#define ZYRX_STRING_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *data;
  uint64_t length;
  uint64_t capacity;
} string_t;

typedef struct {
  string_t *ptr;
  uint64_t offset;
  uint64_t length;
} string_view_t;

void string_set_capacity(string_t *list, uint64_t capacity);
void string_push(string_t *list, char item);
void string_free(string_t *list);
void string_pop(string_t *list);

static inline void string_view_chop_left(string_view_t *view, uint64_t amount) {
  assert(view->length >= amount);
  view->offset += amount;
  view->length -= amount;
}

static inline void string_view_chop_right(string_view_t *view,
                                          uint64_t amount) {
  assert(view->length >= amount);
  view->length -= amount;
}

static inline string_view_t string_view_make(string_t *list) {
  assert(list->data != NULL && list->length > 0);
  return (string_view_t){
      .ptr = list,
      .offset = 0,
      .length = list->length,
  };
}

static inline char *string_view_at(string_view_t *view, uint64_t index) {
  assert(index < view->length);
  return &view->ptr->data[view->offset + index];
}
static inline char *string_at(string_t *list, uint64_t index) {
  ((index < list->length)
       ? (void)(0)
       : __assert_fail("index < list->length",
                       "/home/ivory/Projects/Zyrx/src/main.c", 8,
                       __extension__ __PRETTY_FUNCTION__));
  return &list->data[index];
}

#endif