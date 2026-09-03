#include <cctype>
#include <iostream>
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

  case TOKEN_COUNT:
    return "TOKEN_COUNT";
    break;
  }

  return "NOT_A_TOKEN";
}

void GetTokensFromLine(std::string_view line, std::vector<Token> &tokens,
                       uint lineNumber = 0) {

  uint startIndex, currentIndex = 0;
  while (currentIndex < line.size()) {
    if (line[currentIndex] == '\\' && line[++currentIndex] == '\\') {
      while (!std::isalnum(line[currentIndex])) {
        currentIndex++;
      }

      tokens.push_back(Token{.line = lineNumber,
                             .column = currentIndex,
                             .token = TOKEN_COMMENT,
                             .comment = line.substr(currentIndex)});
      break;
    }
  }
}

int main() {
  std::vector<Token> tokens;
  GetTokensFromLine("\\\\ This is a comment!", tokens);
  GetTokensFromLine("\\\\ THIS is ANOTHER comment, mwahahahhaaha", tokens);

  for (auto i : tokens) {
    std::cout << TokenTypeToStringView(i.token) << ' ' << i.comment;
  }
  std::cout << '\n';

  return 0;
}