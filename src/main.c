#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "sv.h"

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

typedef struct allocator_t {
  void* ctx;

  void* (*alloc)(struct allocator_t* self, uint64_t bytes);
  void (*free)(struct allocator_t* self, void* ptr);
} allocator_t;

void* _heap_alloc(allocator_t* self, uint64_t bytes) {
  (void)self;
  return malloc(((uint64_t)bytes));
}

void _heap_free(allocator_t* self, void* ptr) {
  (void)self;
  free(ptr);
}

allocator_t heap = (allocator_t){
    .ctx = NULL,
    .alloc = _heap_alloc,
    .free = _heap_free,
};

typedef struct {
  uint8_t* buffer;
  uint64_t offset;
  uint64_t capacity;
} arena_t;

#define ALIGN_BYTES(bytes) (uint64_t)(((bytes) + 7) & ~7)

static inline arena_t arena_make(uint8_t* buffer, uint64_t n) {
  memset(buffer, 0, n);
  return (arena_t){
      .buffer = buffer,
      .offset = 0,
      .capacity = n,
  };
}

void* _arena_alloc(allocator_t* self, uint64_t bytes) {
  arena_t* ctx = (arena_t*)self->ctx;

  uint64_t aligned_bytes = ALIGN_BYTES(bytes);

  ZYRX_ASSERT(ctx->offset + aligned_bytes <= ctx->capacity,
              "Arena ran out of memory!");

  ctx->offset += aligned_bytes;

  return ctx->buffer + ctx->offset - aligned_bytes;
}

void _arena_free(allocator_t* self, void* ptr) {
  (void)self;
  (void)ptr;
}

static inline allocator_t arena_make_allocator(arena_t* arena) {
  return (allocator_t){
      .ctx = (void*)arena,
      .alloc = _arena_alloc,
      .free = _arena_free,
  };
}

static inline void arena_clear(arena_t* arena) {
  memset(arena->buffer, 0, arena->offset);
  arena->offset = 0;
}

int main(void) {
  uint8_t* buffer = (uint8_t*)heap.alloc(&heap, 1024);
  arena_t arena = arena_make(buffer, 1024);
  allocator_t alloc = arena_make_allocator(&arena);

  int* a = alloc.alloc(&alloc, sizeof(int));
  *a = 1321521;

  printf("%d", *a);

  arena_clear(&arena);
  heap.free(&heap, buffer);
}

// int main() {
//   char* file_string = get_file_string("test.zrx");
//   string_view_t file_view = SV(file_string);
//   (void)file_view;

//   while (file_view.length > 0) {
//     string_view_t line = sv_chop_by_delimiter(&file_view, '\n');
//     sv_trim(&line);
//     printf("|" SV_FMT "|\n", (int)line.length, line.data);
//   }

//   free(file_string);
// }