#include "Lexer.hpp"
#include "src/Token.hpp"
#include <string>

const std::vector<Token> &Lexer::GetTokens() const { return tokens_; }

void Lexer::GetTokensFromLine(std::string string) {
  column_ = 0;

  while (column_ < string.size()) {
    { // SINGLE CHARACTER TOKENS
      switch (string[column_]) {
      case '(':
        AddToken(TokenType::LEFT_PARENTHESIS);
        break;
      case ')':
        AddToken(TokenType::RIGHT_PARENTHESIS);
        break;
      case '{':
        AddToken(TokenType::LEFT_BRACE);
        break;
      case '}':
        AddToken(TokenType::RIGHT_BRACE);
        break;
      case '.':
        AddToken(TokenType::PERIOD);
        break;
      case ':':
        AddToken(TokenType::COLON);
        break;
      case '=':
        AddToken(TokenType::EQUAL);
        break;
      }
    }

    column_++;
  }

  AddToken(TokenType::NEW_LINE);
}
