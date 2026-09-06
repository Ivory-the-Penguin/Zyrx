#include "Lexer.hpp"
#include "src/Token.hpp"
#include <string>

const std::vector<Token> &Lexer::GetTokens() const { return tokens_; }

void Lexer::GetTokensFromSource() {
  column_ = 0;

  while (column_ < source_.size()) {
    { // SINGLE CHARACTER TOKENS
      switch (source_[column_]) {
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
      case '\n':
        AddToken(TokenType::NEW_LINE);
        lineNumber_++;
      }
    }

    column_++;
  }

  AddToken(TokenType::END_OF_FILE);
}

void Lexer::AddToken(TokenType token) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = token,
      .type = DataType::NONE,
      .lexeme = "",
      .comment = "",
      .literal = std::monostate(),
  });
};

void Lexer::AddToken(int literal) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = TokenType::LITERAL,
      .type = DataType::INT,
      .lexeme = "",
      .comment = "",
      .literal = literal,
  });
}

void Lexer::AddToken(std::string_view literal) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = TokenType::LITERAL,
      .type = DataType::CSTRING,
      .lexeme = "",
      .comment = "",
      .literal = literal,
  });
}

void Lexer::AddToken(double literal) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = TokenType::LITERAL,
      .type = DataType::FLOAT,
      .lexeme = "",
      .comment = "",
      .literal = literal,
  });
}

char Lexer::PeekChar() const {
  if (column_ + 1 < source_.size()) {
    return source_[column_ + 1];
  }
  return '\0';
}
