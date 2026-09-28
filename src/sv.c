#include "sv.h"

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
