#include "Lexer.hpp"
#include <string>

const std::vector<Token> &Lexer::GetTokens() const { return tokens_; }

void Lexer::GetTokensFromLine(std::string string) {
  column_ = 0;

  while (column_ < string.size()) {
    { // SINGLE CHARACTER TOKENS
      switch (string[column_]) {
      case '(':
        AddToken(TOKEN_LEFT_PARENTHESIS);
        break;
      case ')':
        AddToken(TOKEN_RIGHT_PARENTHESIS);
        break;
      case '{':
        AddToken(TOKEN_LEFT_BRACE);
        break;
      case '}':
        AddToken(TOKEN_RIGHT_BRACE);
        break;
      case '.':
        AddToken(TOKEN_PERIOD);
        break;
      case ':':
        AddToken(TOKEN_COLON);
        break;
      case '=':
        AddToken(TOKEN_EQUAL);
        break;
      default:
        break;
      }
    }

    column_++;
  }

  AddToken(TOKEN_NEW_LINE);
}
