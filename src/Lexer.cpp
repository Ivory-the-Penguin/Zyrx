#include "Lexer.hpp"
#include "Token.hpp"
#include <cctype>
#include <string>
#include <string_view>
#include <variant>

const std::vector<Token> &Lexer::GetTokens() const { return tokens_; }

void Lexer::GetTokensFromSource() {
  location_ = 0;
  lineNumber_ = 0;
  column_ = 0;

  while (location_ < source_.size()) {
    switch (source_[location_]) {
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
    case ';':
      AddToken(TokenType::SEMICOLON);
    case ':':
      AddToken((Match('=') ? TokenType::COLON_EQUAL : TokenType::COLON));
      break;
    case '=':
      AddToken((Match('=') ? TokenType::EQUAL_CMP : TokenType::EQUAL));
      break;
    case '-':
      if (Match('-')) {
        uint64_t start = Advance();
        while (PeekChar() != '\n' && PeekChar() != '\0') {
          Advance();
        }

        std::string_view view(source_.begin() + start,
                              source_.begin() + location_);

        while (!view.empty() && std::isspace(view.front())) {
          view.remove_prefix(1);
        }
        while (!view.empty() && std::isspace(view.back())) {
          view.remove_suffix(1);
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
      column_ = 0;
    default:
      if (std::isalpha(source_[location_])) {
        AddIdentifier(ConsumeLexeme());
      }
    }

    Advance();
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

void Lexer::AddIdentifier(std::string_view identifier) {
  tokens_.push_back(Token{
      .line = lineNumber_,
      .column = column_,
      .token = TokenType::IDENTIFIER,
      .type = DataType::NONE,
      .lexeme = identifier,
      .comment = "",
      .literal = std::monostate(),
  });
}

char Lexer::PeekChar() const {
  if (location_ + 1 < source_.size()) {
    return source_[location_ + 1];
  }
  return '\0';
}

bool Lexer::Match(char match) {
  if (PeekChar() == match) {
    Advance();
    return true;
  }

  return false;
}

int64_t Lexer::Advance(int64_t n) {
  column_ += n;
  location_ += n;

  return location_;
}

std::string_view Lexer::ConsumeLexeme() {
  if (!std::isalpha(source_[location_])) {
    return nullptr;
  }

  uint64_t start = location_;
  while (Match('_') || std::isalnum(PeekChar())) {
    Advance();
  }

  return std::string_view(source_.begin() + start, source_.begin() + Advance());
}
