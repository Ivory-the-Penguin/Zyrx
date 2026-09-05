#pragma once

#include <cstdint>
#include <string>

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

  std::string identifier;
  std::string comment;
  DataType type;

  void Output();
};

std::string TokenTypeToStringView(TokenType token);
void OutputToken(Token token);
