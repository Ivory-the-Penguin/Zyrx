#include "Token.hpp"
#include <iostream>

std::string TokenTypeToStringView(TokenType token) {
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
  case TOKEN_LEFT_BRACE:
    return "{";
    break;
  case TOKEN_RIGHT_BRACE:
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

void Token::Output() {
  std::string buffer;

  buffer.append(TokenTypeToStringView(token));

  switch (token) {
  case TOKEN_COMMENT:
    buffer.append(" (");
    buffer.append(comment);
    buffer.append(")");
    break;

  case TOKEN_IDENTIFIER:
    buffer.append("(");
    buffer.append(identifier);
    buffer.append(")");
    break;
  case TOKEN_NEW_LINE:
    buffer.append("\n");
  default:
    break;
  }

  std::cout << buffer << ' ';
}
