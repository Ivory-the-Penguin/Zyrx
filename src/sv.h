#ifndef ZYRX_SV_H
#define ZYRX_SV_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "assert.h"

typedef struct {
  const char* data;
  uint64_t length;
} string_view_t;

#define SV(c_str) \
  (string_view_t) { .data = c_str, .length = strlen(c_str), }

#define SV_FMT "%.*s"

/*
-1 means a is smaller than b,
0 means a is equal to b,
1 means a is bigger than b,
*/
static inline int8_t sv_compare(string_view_t a, string_view_t b) {
  int cmp = memcmp(a.data, b.data, (a.length < b.length ? a.length : b.length));

  if (cmp == 0) {
    if (a.length == b.length) {
      return 0;
    }

    return (a.length > b.length ? 1 : -1);
  }

  return (cmp > 0 ? 1 : -1);
}

static inline void sv_chop_left(string_view_t* sv, uint64_t n) {
  ZYRX_ASSERT(sv->length >= n, "String view is too small to be chopped");
  sv->data += n;
  sv->length -= n;
}

static inline void sv_chop_right(string_view_t* sv, uint64_t n) {
  ZYRX_ASSERT(sv->length >= n, "String view is too small to be chopped");
  sv->length -= n;
}

static inline void sv_trim_left(string_view_t* sv) {
  while (sv->length > 0 && isspace((unsigned char)sv->data[0])) {
    sv_chop_left(sv, 1);
  }
}

static inline void sv_trim_right(string_view_t* sv) {
  while (sv->length > 0 && isspace((unsigned char)sv->data[sv->length - 1])) {
    sv_chop_right(sv, 1);
  }
}

static inline void sv_trim(string_view_t* sv) {
  sv_trim_left(sv);
  sv_trim_right(sv);
}

string_view_t sv_chop_by_delimiter(string_view_t* sv, char delimiter);

// is_type is what IS a delimiter
string_view_t sv_chop_by_type(string_view_t* sv, int (*is_type)(int c));

// is_type is what ISN'T a delimiter
string_view_t sv_chop_by_type_rev(string_view_t* sv, int (*is_type)(int c));

#endif