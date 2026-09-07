#pragma once

#include "Token.hpp"
#include <string_view>
#include <unordered_map>
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

  void AddIdentifier(std::string_view);

  char PeekChar() const;
  bool Match(char match);
  int64_t Advance(int64_t n = 1);

  std::string_view ConsumeLexeme();
  std::string_view ConsumeComment();

  uint64_t lineNumber_;
  uint64_t column_;
  uint64_t location_;

  std::string source_;
  std::vector<Token> tokens_;
  std::unordered_map<std::string_view, TokenType> reservedKeywords_ = {
      {"proc", TokenType::PROCEDURE},
      {"func", TokenType::FUNCTION},
      {"return", TokenType::RETURN}};
};
