#pragma once

#include <cstdint>
#include <string>

typedef enum {
  TOKEN_UNKNOWN = -1,
  TOKEN_EOF,
  TOKEN_NEW_LINE,
  TOKEN_IDENTIFIER,
  TOKEN_CONST_DEF,
  TOKEN_STRING,
  TOKEN_PROCEDURE,
  TOKEN_FUNCTION,
  TOKEN_PERIOD,
  TOKEN_TYPE,
  TOKEN_LEFT_PARENTHESIS,
  TOKEN_RIGHT_PARENTHESIS,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMENT,

  TOKEN_COUNT
} TokenType;

enum Type {
  TYPE_INT,
};

struct Token {
  uint32_t line;
  uint32_t column;

  TokenType token;

  std::string identifier;
  std::string comment;
  Type type;

  void Output();
};

std::string TokenTypeToStringView(TokenType token);
void OutputToken(Token token);
