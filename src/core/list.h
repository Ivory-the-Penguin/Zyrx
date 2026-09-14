#include <assert.h>
#include <stdint.h>

#define DEFINE_ARRAY(TYPE)                                                     \
  typedef struct {                                                             \
    TYPE *data;                                                                \
    uint64_t length;                                                           \
    uint64_t capacity;                                                         \
  } list_##TYPE##_t;                                                           \
                                                                               \
  void list_##TYPE##_set_capacity(list_##TYPE##_t *list, uint64_t capacity);   \
  void list_##TYPE##_push(list_##TYPE##_t *list, TYPE item);                   \
  void list_##TYPE##_free(list_##TYPE##_t *list);                              \
  void list_##TYPE##_pop(list_##TYPE##_t *list);                               \
                                                                               \
  static inline TYPE *list_##TYPE##_at(list_##TYPE##_t *list,                  \
                                       uint64_t index) {                       \
    assert(index < list->length);                                              \
    return &list->data[index];                                                 \
  }

#define IMPL_ARRAY(TYPE)                                                       \
  void list_##TYPE##_set_capacity(list_##TYPE##_t *list, uint64_t capacity) {  \
    assert(list->length <= capacity &&                                         \
           "Capacity is smaller than the list length!");                       \
    assert(capacity > 0 && "Can't set capacity to 0!");                        \
                                                                               \
    list->data = (TYPE *)realloc(list->data, capacity * sizeof(TYPE));         \
    assert(list->data != nullptr && "Allocation failed!");                     \
                                                                               \
    list->capacity = capacity;                                                 \
                                                                               \
    memset(list->data + list->length, 0,                                       \
           (capacity - list->length) * sizeof(TYPE));                          \
  }                                                                            \
                                                                               \
  void list_##TYPE##_push(list_##TYPE##_t *list, TYPE item) {                  \
    if (list->capacity == 0) {                                                 \
      list_##TYPE##_set_capacity(list, 1);                                     \
    } else if (list->length == list->capacity) {                               \
      list_##TYPE##_set_capacity(list, list->capacity * 2);                    \
    }                                                                          \
                                                                               \
    list->data[list->length++] = item;                                         \
  }                                                                            \
                                                                               \
  void list_##TYPE##_free(list_##TYPE##_t *list) {                             \
    assert(list->data != nullptr && "Data can't be null!");                    \
                                                                               \
    free((void *)list->data);                                                  \
    list->length = 0;                                                          \
    list->capacity = 0;                                                        \
  }                                                                            \
                                                                               \
  void list_##TYPE##_pop(list_##TYPE##_t *list) {                              \
    assert(list->length > 0 && "Popping when the list is empty !");            \
                                                                               \
    list->data[--list->length] = (TYPE){};                                     \
                                                                               \
    if (list->length < list->capacity / 4 && list->capacity > 8) {             \
      list_##TYPE##_set_capacity(list, list->capacity / 2);                    \
    }                                                                          \
  }

DEFINE_ARRAY(int)
DEFINE_ARRAY(float)