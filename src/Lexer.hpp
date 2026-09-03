#pragma once

#include "Token.hpp"
#include <vector>

class Lexer {
public:
  const std::vector<Token> &GetTokens() const;
  void GetTokensFromLine(std::string string);

private:
  void AddToken(TokenType token) {
    tokens_.push_back({
        .line = lineNumber_,
        .column = column_,
        .token = token,
    });
  };

  char PeekChar(const std::string &string) const {
    if (column_ + 1 < string.size()) {
      return string[column_];
    }
    return '\0';
  }

  uint32_t lineNumber_;
  uint32_t column_;
  std::vector<Token> tokens_;
};
