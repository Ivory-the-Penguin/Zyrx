#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "vec/vec.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

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
  for (uint64_t i = 0; i < MIN(a.length, b.length); i++) {
    if (a.data[i] > b.data[i]) {
      return 1;
    } else if (a.data[i] < b.data[i]) {
      return -1;
    }
  }

  if (a.length > b.length) {
    return 1;
  } else if (a.length < b.length) {
    return -1;
  }

  return 0;
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

string_view_t sv_chop_by_delimiter(string_view_t* sv, char delimiter) {
  while (sv->length > 0 && sv->data[0] == delimiter) {
    sv_chop_left(sv, 1);
  }

  if (sv->length == 0) {
    return *sv;
  }

  uint64_t end = 0;
  do {
    end++;
  } while (end < sv->length && sv->data[end] != delimiter);

  string_view_t out;
  if (end < sv->length) {
    out = (string_view_t){
        .data = sv->data,
        .length = end,
    };
    sv_chop_left(sv, end + 1);
    return out;
  }

  out = *sv;
  sv_chop_left(sv, sv->length);
  return out;
}

// is_type is what IS a delimiter
string_view_t sv_chop_by_type(string_view_t* sv, int (*is_type)(int c)) {
  while (sv->length > 0 && is_type((unsigned char)sv->data[0])) {
    sv_chop_left(sv, 1);
  }

  if (sv->length == 0) {
    return *sv;
  }

  uint64_t end = 0;
  do {
    end++;
  } while (end < sv->length && !is_type((unsigned char)sv->data[end]));

  string_view_t out;
  if (end < sv->length) {
    out = (string_view_t){
        .data = sv->data,
        .length = end,
    };
    sv_chop_left(sv, end + 1);
    return out;
  }

  out = *sv;
  sv_chop_left(sv, sv->length);
  return out;
}

// is_type is what ISN'T a delimiter
string_view_t sv_chop_by_type_rev(string_view_t* sv, int (*is_type)(int c)) {
  while (sv->length > 0 && !is_type((unsigned char)sv->data[0])) {
    sv_chop_left(sv, 1);
  }

  if (sv->length == 0) {
    return *sv;
  }

  uint64_t end = 0;
  do {
    end++;
  } while (end < sv->length && is_type((unsigned char)sv->data[end]));

  string_view_t out;
  if (end < sv->length) {
    out = (string_view_t){
        .data = sv->data,
        .length = end,
    };
    sv_chop_left(sv, end + 1);
    return out;
  }

  out = *sv;
  sv_chop_left(sv, sv->length);
  return out;
}

// Returning string must be freed!
char* get_file_string(const char* path) {
  FILE* file = fopen(path, "rb");
  ZYRX_ASSERT(file != NULL, "Couldn't open file");

  fseek(file, 0, SEEK_END);
  uint64_t file_size = ftell(file);
  rewind(file);

  char* file_string = (char*)malloc(sizeof(char) * (file_size + 1));
  size_t bytes_read = fread(file_string, 1, file_size, file);
  file_string[bytes_read] = '\0';

  fclose(file);

  return file_string;
}

int main() {
  char* file_string = get_file_string("test.zrx");
  string_view_t file_view = SV(file_string);
  (void)file_view;

  while (file_view.length > 0) {
    string_view_t type = sv_chop_by_delimiter(&file_view, '\n');
    sv_trim(&type);
    printf("|" SV_FMT "|\n", (int)type.length, type.data);
  }

  free(file_string);
}