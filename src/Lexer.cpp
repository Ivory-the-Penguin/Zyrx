#include "Lexer.hpp"
#include "src/Token.hpp"
#include <cctype>
#include <string>
#include <string_view>
#include <variant>

const std::vector<Token> &Lexer::GetTokens() const { return tokens_; }

void Lexer::GetTokensFromSource() {
  column_ = 0;
  lineNumber_ = 0;

  while (column_ < source_.size()) {
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
      AddToken((Match('=') ? TokenType::COLON_EQUAL : TokenType::COLON));
      break;
    case '=':
      AddToken((Match('=') ? TokenType::EQUAL_CMP : TokenType::EQUAL));
      break;
    case '-':
      if (Match('-')) {
        uint64_t start = ++column_;
        while (PeekChar() != '\n' && PeekChar() != '\0') {
          column_++;
        }

        std::string_view view(source_.begin() + start,
                              source_.begin() + column_--);

        while (std::isspace(view.front())) {
          view.remove_prefix(1);
        }

        AddComment(view);
      } else {
        AddToken(TokenType::MINUS);
      }
      break;
    case '<':
      if (Match('-')) {
        AddToken(TokenType::CONST_DEF);
      } else {
        AddToken(
            (Match('=') ? TokenType::LESS_EQUAL_CMP : TokenType::LESS_CMP));
      }
      break;
    case '\n':
      AddToken(TokenType::NEW_LINE);
      lineNumber_++;
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

void Lexer::AddLiteral(int literal) {
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

void Lexer::AddLiteral(std::string_view literal) {
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

void Lexer::AddLiteral(double literal) {
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

void Lexer::AddComment(std::string_view comment) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = TokenType::COMMENT,
      .type = DataType::NONE,
      .lexeme = "",
      .comment = comment,
      .literal = std::monostate(),
  });
}

char Lexer::PeekChar() const {
  if (column_ + 1 < source_.size()) {
    return source_[column_ + 1];
  }
  return '\0';
}

bool Lexer::Match(char match) {
  if (PeekChar() == match) {
    column_++;
    return true;
  }

  return false;
}