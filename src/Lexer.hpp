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

  void AddLiteral(int literal);
  void AddLiteral(std::string_view literal);
  void AddLiteral(double literal);

  void AddComment(std::string_view comment);

  char PeekChar(char match = '\0');

  uint64_t lineNumber_;
  uint64_t column_;
  std::string source_;
  std::vector<Token> tokens_;
};
