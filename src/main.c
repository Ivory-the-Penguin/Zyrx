#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "sv.h"

// Returning string must be freed (if its heap allocated)
char* get_file_string(allocator_t allocator, const char* path) {
  FILE* file = fopen(path, "rb");
  ZYRX_ASSERT(file != NULL, "Couldn't open file");

  fseek(file, 0, SEEK_END);
  uint64_t file_size = ftell(file);
  rewind(file);

  char* file_string = (char*)ALLOC(allocator, sizeof(char) * (file_size + 1));
  size_t bytes_read = fread(file_string, 1, file_size, file);
  file_string[bytes_read] = '\0';

  fclose(file);

  return file_string;
}

#define ARENA_SIZE (10 * 1024 * 1024)

int main(void) {
  uint8_t* buffer = (uint8_t*)ALLOC(heap, ARENA_SIZE);
  arena_t arena = arena_make(buffer, ARENA_SIZE);
  allocator_t alloc = arena_make_allocator(&arena);

  char* file_string = get_file_string(alloc, "test.zrx");
  string_view_t file_view = SV(file_string);
  (void)file_view;

  while (file_view.length > 0) {
    string_view_t line = sv_chop_by_delimiter(&file_view, '\n');
    sv_trim(&line);
    printf("|" SV_FMT "|\n", (int)line.length, line.data);
  }

  FREE(heap, buffer);
}