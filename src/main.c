#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "vec/vec.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  char* data;
  uint64_t length;
} string_view_t;

#define SV(c_str) \
  (string_view_t) { .data = c_str, .length = strlen(c_str), }

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

  return 0;
}

static inline void sv_chop_left(string_view_t* sv, uint64_t n) {
  ZYRX_ASSERT(sv->length >= n, "String view is too small to be chopped");
  sv->data += n;
  sv->data -= n;
}

static inline void sv_chop_right(string_view_t* sv, uint64_t n) {
  ZYRX_ASSERT(sv->length >= n, "String view is too small to be chopped");
  sv->length -= n;
}

// Returning string must be freed!
char* get_file_string(const char* path) {
  FILE* file = fopen(path, "r");
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

  printf("%d", sv_compare(SV("Java"), SV("Python")));

  free(file_string);
}
