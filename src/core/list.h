#ifndef ZYRX_LIST_H
#define ZYRX_LIST_H

#include "core/assert.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LIST_FOREACH(list, i) for (uint64_t i = 0; i < (list).length; i++)

#define LIST_MINIMUM_CAPACITY 8

#define DEFINE_LIST(TYPE, TYPE_NAME)                                           \
  typedef struct {                                                             \
    TYPE *data;                                                                \
    uint64_t length;                                                           \
    uint64_t capacity;                                                         \
  } list_##TYPE_NAME##_t;                                                      \
                                                                               \
  typedef struct {                                                             \
    list_##TYPE_NAME##_t *ptr;                                                 \
    uint64_t offset;                                                           \
    uint64_t length;                                                           \
  } list_##TYPE_NAME##_view_t;                                                 \
                                                                               \
  static inline TYPE *list_##TYPE_NAME##_view_at(                              \
      const list_##TYPE_NAME##_view_t *view, uint64_t index) {                 \
    ZYRX_ASSERT(index < view->length, "Out of bounds error");                  \
    return &view->ptr->data[view->offset + index];                             \
  }                                                                            \
                                                                               \
  static inline TYPE *list_##TYPE_NAME##_at(const list_##TYPE_NAME##_t *list,  \
                                            uint64_t index) {                  \
    ZYRX_ASSERT(index < list->length, "Out of bounds error");                  \
    return &list->data[index];                                                 \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_set_capacity(                          \
      list_##TYPE_NAME##_t *list, uint64_t capacity) {                         \
    ZYRX_ASSERT(list->length <= capacity,                                      \
                "Capacity is smaller than the list length");                   \
    ZYRX_ASSERT(capacity > 0, "Can't set capacity to 0");                      \
                                                                               \
    list->data = (TYPE *)realloc(list->data, capacity * sizeof(TYPE));         \
    if (list->data == NULL) {                                                  \
      ZYRX_ASSERT(0, "Allocation failed");                                     \
    }                                                                          \
                                                                               \
    list->capacity = capacity;                                                 \
                                                                               \
    memset(list->data + list->length, 0,                                       \
           (capacity - list->length) * sizeof(TYPE));                          \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_push(list_##TYPE_NAME##_t *list,       \
                                             TYPE item) {                      \
    if (list->capacity == 0) {                                                 \
      list_##TYPE_NAME##_set_capacity(list, 4);                                \
    } else if (list->length == list->capacity) {                               \
      list_##TYPE_NAME##_set_capacity(list, list->capacity * 2);               \
    }                                                                          \
                                                                               \
    list->data[list->length++] = item;                                         \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_free(list_##TYPE_NAME##_t *list) {     \
    ZYRX_ASSERT(list->data != NULL, "Data can't be null");                     \
                                                                               \
    free((void *)list->data);                                                  \
    list->data = NULL;                                                         \
    list->length = 0;                                                          \
    list->capacity = 0;                                                        \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_pop(list_##TYPE_NAME##_t *list) {      \
    ZYRX_ASSERT(list->length > 0, "Popping when the list is empty ");          \
                                                                               \
    list->data[--list->length] = (TYPE){0};                                    \
                                                                               \
    if (list->length < list->capacity / 4 &&                                   \
        list->capacity > LIST_MINIMUM_CAPACITY) {                              \
      list_##TYPE_NAME##_set_capacity(list, list->capacity / 2);               \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_combine(                               \
      list_##TYPE_NAME##_t *dest, const list_##TYPE_NAME##_t *src) {           \
    ZYRX_ASSERT(dest->data != NULL && src->data != NULL, "Null data");         \
    ZYRX_ASSERT(dest->data != src->data, "Lists have to be different");        \
                                                                               \
    if (src->length == 0) {                                                    \
      return;                                                                  \
    }                                                                          \
                                                                               \
    list_##TYPE_NAME##_set_capacity(dest, dest->length + src->length);         \
    LIST_FOREACH(*src, i) {                                                    \
      list_##TYPE_NAME##_push(dest, *list_##TYPE_NAME##_at(src, i));           \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_view_chop_left(                        \
      list_##TYPE_NAME##_view_t *view, uint64_t amount) {                      \
    ZYRX_ASSERT(view->length >= amount, "String is too small to chop");        \
    view->offset += amount;                                                    \
    view->length -= amount;                                                    \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_view_chop_right(                       \
      list_##TYPE_NAME##_view_t *view, uint64_t amount) {                      \
    ZYRX_ASSERT(view->length >= amount, "String is too small to chop");        \
    view->length -= amount;                                                    \
  }                                                                            \
                                                                               \
  static inline list_##TYPE_NAME##_view_t list_##TYPE_NAME##_view_make(        \
      list_##TYPE_NAME##_t *list) {                                            \
    ZYRX_ASSERT(list->data != NULL,                                            \
                "Can't make a view from an unallocated list");                 \
    return (list_##TYPE_NAME##_view_t){                                        \
        .ptr = list,                                                           \
        .offset = 0,                                                           \
        .length = list->length,                                                \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline list_##TYPE_NAME##_t list_##TYPE_NAME##_make(void) {           \
    list_##TYPE_NAME##_t list = (list_##TYPE_NAME##_t){0};                     \
    list_##TYPE_NAME##_set_capacity(&list, LIST_MINIMUM_CAPACITY);             \
    return list;                                                               \
  }

#endif