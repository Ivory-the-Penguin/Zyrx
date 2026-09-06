#pragma once

#include "Token.hpp"
#include <vector>

class Lexer {
public:
  Lexer(std::string source) : source_(source) {}

  const std::vector<Token> &GetTokens() const;

  void GetTokensFromSource();

private:
  void AddToken(TokenType token) {
    tokens_.push_back({
        .line = lineNumber_,
        .column = column_,
        .token = token,
    });
  };

  char PeekChar() const {
    if (column_ + 1 < source_.size()) {
      return source_[column_ + 1];
    }
    return '\0';
  }

  uint32_t lineNumber_;
  uint32_t column_;
  std::string source_;
  std::vector<Token> tokens_;
};
