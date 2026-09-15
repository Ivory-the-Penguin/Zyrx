#ifndef ZYRX_LIST_H
#define ZYRX_LIST_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LIST_FOREACH(list, i) for (uint64_t i = 0; i < (list).length; i++)

#define DEFINE_ARRAY(TYPE, TYPE_NAME)                                          \
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
  static inline void list_##TYPE_NAME##_set_capacity(                          \
      list_##TYPE_NAME##_t *list, uint64_t capacity) {                         \
    assert(list->length <= capacity &&                                         \
           "Capacity is smaller than the list length!");                       \
    assert(capacity > 0 && "Can't set capacity to 0!");                        \
                                                                               \
    list->data = (TYPE *)realloc(list->data, capacity * sizeof(TYPE));         \
    if (list->data == NULL) {                                                  \
      assert(0 && "Allocation failed!");                                       \
      abort();                                                                 \
    }                                                                          \
                                                                               \
    list->capacity = capacity;                                                 \
                                                                               \
    memset(list->data + list->length, 0,                                       \
           (list->length > capacity ? 0 : (capacity - list->length)) *         \
               sizeof(TYPE));                                                  \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_push(list_##TYPE_NAME##_t *list,       \
                                             TYPE item) {                      \
    if (list->capacity == 0) {                                                 \
      list_##TYPE_NAME##_set_capacity(list, 1);                                \
    } else if (list->length == list->capacity) {                               \
      list_##TYPE_NAME##_set_capacity(list, list->capacity * 2);               \
    }                                                                          \
                                                                               \
    list->data[list->length++] = item;                                         \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_free(list_##TYPE_NAME##_t *list) {     \
    assert(list->data != NULL && "Data can't be null!");                       \
                                                                               \
    free((void *)list->data);                                                  \
    list->length = 0;                                                          \
    list->capacity = 0;                                                        \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_pop(list_##TYPE_NAME##_t *list) {      \
    assert(list->length > 0 && "Popping when the list is empty !");            \
                                                                               \
    list->data[--list->length] = (TYPE){0};                                    \
                                                                               \
    if (list->length < list->capacity / 4 && list->capacity > 8) {             \
      list_##TYPE_NAME##_set_capacity(list, list->capacity / 2);               \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_view_chop_left(                        \
      list_##TYPE_NAME##_view_t *view, uint64_t amount) {                      \
    assert(view->length >= amount);                                            \
    view->offset += amount;                                                    \
    view->length -= amount;                                                    \
  }                                                                            \
                                                                               \
  static inline void list_##TYPE_NAME##_view_chop_right(                       \
      list_##TYPE_NAME##_view_t *view, uint64_t amount) {                      \
    assert(view->length >= amount);                                            \
    view->length -= amount;                                                    \
  }                                                                            \
                                                                               \
  static inline list_##TYPE_NAME##_view_t list_##TYPE_NAME##_view_make(        \
      list_##TYPE_NAME##_t *list) {                                            \
    assert(list->data != NULL && list->length > 0);                            \
    return (list_##TYPE_NAME##_view_t){                                        \
        .ptr = list,                                                           \
        .offset = 0,                                                           \
        .length = list->length,                                                \
    };                                                                         \
  }                                                                            \
                                                                               \
  static inline TYPE *list_##TYPE_NAME##_view_at(                              \
      list_##TYPE_NAME##_view_t *view, uint64_t index) {                       \
    assert(index < view->length);                                              \
    return &view->ptr->data[view->offset + index];                             \
  }                                                                            \
                                                                               \
  static inline TYPE *list_##TYPE_NAME##_at(list_##TYPE_NAME##_t *list,        \
                                            uint64_t index) {                  \
    assert(index < list->length);                                              \
    return &list->data[index];                                                 \
  }

#endif