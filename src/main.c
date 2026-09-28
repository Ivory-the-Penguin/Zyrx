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

typedef enum {
  TOKEN_UNKNOWN = -1,
  TOKEN_COMMENT,
  TOKEN_CONST_DEF,
  TOKEN_DATA_TYPE,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_SEMICOLON,
  TOKEN_IDENTIFIER,
  TOKEN_PROC,
  TOKEN_RETURN,
} token_type_t;

typedef enum {
  DATATYPE_INT,
} data_type_t;

typedef struct {
  token_type_t token_type;
  data_type_t data_type;

  uint64_t line;
  uint64_t column_beg;
  uint64_t column_end;

  union {
    int int_literal;

    string_view_t comment;
    string_view_t lexeme;
  } as;
} token_t;

#define ARENA_SIZE (1 * 1024 * 1024)

int main(void) {
  uint8_t* buffer = (uint8_t*)ALLOC(heap, ARENA_SIZE);
  arena_t arena = arena_make(buffer, ARENA_SIZE);
  allocator_t alloc = arena_make_allocator(&arena);
  (void)alloc;

  char* file_string = get_file_string(heap, "test.zrx");
  string_view_t file_view = SV(file_string);

  while (file_view.length > 0) {
    string_view_t line = sv_chop_by_delimiter(&file_view, '\n');
    sv_trim(&line);
    printf("|" SV_FMT "|\n", (int)line.length, line.data);
  }

  FREE(heap, buffer);
}