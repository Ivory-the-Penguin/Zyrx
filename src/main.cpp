#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

typedef enum {
  TOKEN_UNKNOWN = -1,
  TOKEN_EOF,
  TOKEN_NEW_LINE,
  TOKEN_IDENTIFIER,
  TOKEN_CONST_DEF,
  TOKEN_STRING,
  TOKEN_PROCEDURE,
  TOKEN_FUNCTION,
  TOKEN_PERIOD,
  TOKEN_TYPE,
  TOKEN_LEFT_PARENTHESIS,
  TOKEN_RIGHT_PARENTHESIS,
  TOKEN_LEFT_BRACES,
  TOKEN_RIGHT_BRACES,
  TOKEN_COMMENT,

  TOKEN_COUNT
} TokenType;

typedef enum {
  TYPE_INT,
} Type;

typedef struct {
  uint line;
  uint column;

  TokenType token;

  union {
    std::string_view identifier;
    std::string_view comment;
    Type type;
  };
} Token;

std::string_view TokenTypeToStringView(TokenType token) {
  switch (token) {
  case TOKEN_EOF:
    return "EOF";
    break;
  case TOKEN_NEW_LINE:
    return "NEW_LINE";
    break;
  case TOKEN_IDENTIFIER:
    return "IDENTIFIER";
    break;
  case TOKEN_CONST_DEF:
    return "<-";
    break;
  case TOKEN_STRING:
    return "STRING";
    break;
  case TOKEN_PROCEDURE:
    return "proc";
    break;
  case TOKEN_FUNCTION:
    return "func";
    break;
  case TOKEN_PERIOD:
    return ".";
    break;
  case TOKEN_TYPE:
    return "TYPE";
    break;
  case TOKEN_LEFT_PARENTHESIS:
    return "(";
    break;
  case TOKEN_RIGHT_PARENTHESIS:
    return ")";
    break;
  case TOKEN_LEFT_BRACES:
    return "{";
    break;
  case TOKEN_RIGHT_BRACES:
    return "}";
    break;
  case TOKEN_COMMENT:
    return "\\\\";
    break;

  case TOKEN_UNKNOWN:
    return "UNKNOWN";
    break;

  default:
    break;
  }

  return "NOT_A_TOKEN";
}

void GetTokensFromLine(std::string_view line, std::vector<Token> &tokens,
                       uint lineNumber = 0) {

  uint currentIndex = 0;
  while (currentIndex < line.size()) {
    if (line[currentIndex] == '\\' && line[++currentIndex] == '\\') {
      while (!std::isalnum(line[currentIndex])) {
        currentIndex++;
      }

      tokens.push_back({.line = lineNumber,
                        .column = currentIndex,
                        .token = TOKEN_COMMENT,
                        .comment = line.substr(currentIndex)});
      break;
    }
  }

  tokens.push_back(
      {.line = lineNumber, .column = currentIndex, .token = TOKEN_NEW_LINE});
}

void OutputToken(Token token) {
  std::string buffer;

  buffer.append(TokenTypeToStringView(token.token));

  switch (token.token) {
  case TOKEN_COMMENT:
    buffer.append(" (");
    buffer.append(token.comment);
    buffer.append(")");
    break;

  case TOKEN_IDENTIFIER:
    buffer.append("(");
    buffer.append(token.identifier);
    buffer.append(")");
    break;
  case TOKEN_NEW_LINE:
    buffer.append("\n");
  default:
    break;
  }

  std::cout << buffer << ' ';
}

int main() {
  std::vector<Token> tokens;
  GetTokensFromLine("\\\\ This is a comment!", tokens);
  GetTokensFromLine("\\\\ THIS is ANOTHER comment, mwahahahahahahaha", tokens);

  std::string buffer;
  for (auto token : tokens) {
    OutputToken(token);
  }
  std::cout << '\n';

  return 0;
}