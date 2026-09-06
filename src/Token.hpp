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
  EQUAL_CMP,
  LESS_CMP,
  BIGGER_CMP,
  LESS_EQUAL_CMP,
  BIGGER_EQUAL_CMP,
  BANG_EQUAL_CMP,
  BANG,
  COLON_EQUAL,
  MINUS,
  PLUS,
  STAR,
  DIVIDE,
  LITERAL,
  SEMICOLON,
  COUNT
};

enum class DataType : int8_t {
  NONE = -1,
  INT,
  FLOAT,
  CSTRING,
};

struct Token {
  uint64_t line;
  uint64_t column;

  TokenType token;
  DataType type;

  std::string_view lexeme; // String views to the source code (which we keep)
  std::string_view comment;
  std::variant<std::monostate, int64_t, double, std::string_view> literal;

  void Output();
};

std::string TokenTypeToString(TokenType token);
void OutputToken(Token token);
