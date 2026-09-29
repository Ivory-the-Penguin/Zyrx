#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IVY_STD_IMPL
#include "ivystd/arena.h"
#include "ivystd/assert.h"
#include "ivystd/sv.h"
#include "vec/vec.h"

// Returning string must be freed (if its heap allocated)
char* get_file_string(allocator_t allocator, const char* path) {
  FILE* file = fopen(path, "rb");
  IVY_ASSERT(file != NULL, "Couldn't open file");

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
  TOKENTYPE_UNKNOWN = -1,
  TOKENTYPE_COMMENT,
  TOKENTYPE_CONST_DEF,
  TOKENTYPE_DATA_TYPE,
  TOKENTYPE_LEFT_BRACE,
  TOKENTYPE_RIGHT_BRACE,
  TOKENTYPE_LEFT_PAREN,
  TOKENTYPE_RIGHT_PAREN,
  TOKENTYPE_SEMICOLON,
  TOKENTYPE_IDENTIFIER,
  TOKENTYPE_PROC,
  TOKENTYPE_RETURN,
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

typedef vec_t(token_t) vec_token_t;

static uint64_t current_line;

static inline token_t token_identifier(uint64_t column,
                                       string_view_t identifier) {
  IVY_ASSERT(identifier.length > 0, "Identifier is empty");
  return (token_t){
      .token_type = TOKENTYPE_IDENTIFIER,
      .line = current_line,
      .column_beg = column,
      .column_end = column + identifier.length - 1,
      .as.lexeme = identifier,
  };
}

static inline token_t token_comment(uint64_t column, uint64_t line_end,
                                    string_view_t comment) {
  IVY_ASSERT(comment.length > 0, "Identifier is empty");
  return (token_t){
      .token_type = TOKENTYPE_COMMENT,
      .line = current_line,
      .column_beg = column,
      .column_end = line_end,
      .as.comment = comment,
  };
}

#define ARENA_SIZE (1 * 1024 * 1024)

int main(void) {
  uint8_t* buffer = (uint8_t*)ALLOC(heap, ARENA_SIZE);
  arena_t arena = arena_make(buffer, ARENA_SIZE);
  allocator_t alloc = arena_make_allocator(&arena);
  (void)alloc;

  char* file_string = get_file_string(heap, "test.zrx");
  string_view_t file_view = SV(file_string);

  vec_token_t tokens;
  vec_init(&tokens);

  while (file_view.length > 0) {
    string_view_t line = sv_chop_by_delimiter(&file_view, '\n');
    sv_trim(&line);

    printf(SV_FMT "\n", (int)line.length, line.data);

    current_line++;
  }

  vec_deinit(&tokens);
  FREE(heap, buffer);
}