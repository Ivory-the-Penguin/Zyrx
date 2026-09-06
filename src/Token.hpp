#pragma once

#include <cstdint>
#include <string>
#include <variant>

enum class TokenType : int8_t {
  UNKNOWN = -1,
  END_OF_FILE,
  NEW_LINE,
  IDENTIFIER,
  CONST_DEF,
  STRING,
  PROCEDURE,
  FUNCTION,
  PERIOD,
  TYPE,
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMENT,
  COLON,
  EQUAL,
  COUNT
};

enum class DataType : int8_t {
  INT,
  FLOAT,
  CSTRING,
};

struct Token {
  uint32_t line;
  uint32_t column;

  TokenType token;
  DataType type;

  std::string_view lexeme; // String views to the source code (which we keep)
  std::string_view comment;
  std::variant<std::monostate, int64_t, double, std::string> literal;

  void Output();
};

std::string TokenTypeToString(TokenType token);
void OutputToken(Token token);
