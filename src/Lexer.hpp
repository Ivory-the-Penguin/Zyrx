#pragma once

#include "Token.hpp"
#include <vector>

class Lexer {
public:
  Lexer(std::string source) : source_(source) {}

  const std::vector<Token> &GetTokens() const;

  void GetTokensFromSource();

private:
  void AddToken(TokenType token);
  void AddToken(int literal);
  void AddToken(std::string_view literal);
  void AddToken(double literal);

  char PeekChar(char match = '\0');

  uint32_t lineNumber_;
  uint32_t column_;
  std::string source_;
  std::vector<Token> tokens_;
};
