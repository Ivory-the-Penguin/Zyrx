#include "Token.hpp"
#include <iostream>

std::string TokenTypeToStringView(TokenType token) {
  switch (token) {
  case TokenType::END_OF_FILE:
    return "EOF";
    break;
  case TokenType::NEW_LINE:
    return "NEW_LINE";
    break;
  case TokenType::IDENTIFIER:
    return "IDENTIFIER";
    break;
  case TokenType::CONST_DEF:
    return "<-";
    break;
  case TokenType::STRING:
    return "STRING";
    break;
  case TokenType::PROCEDURE:
    return "proc";
    break;
  case TokenType::FUNCTION:
    return "func";
    break;
  case TokenType::PERIOD:
    return ".";
    break;
  case TokenType::TYPE:
    return "TYPE";
    break;
  case TokenType::LEFT_PARENTHESIS:
    return "(";
    break;
  case TokenType::RIGHT_PARENTHESIS:
    return ")";
    break;
  case TokenType::LEFT_BRACE:
    return "{";
    break;
  case TokenType::RIGHT_BRACE:
    return "}";
    break;
  case TokenType::COMMENT:
    return "\\\\";
    break;
  case TokenType::COLON:
    return ":";
    break;
  case TokenType::EQUAL:
    return "=";
    break;

  case TokenType::UNKNOWN:
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
  case TokenType::COMMENT:
    buffer.append(" (");
    buffer.append(comment);
    buffer.append(")");
    break;

  case TokenType::IDENTIFIER:
    buffer.append("(");
    buffer.append(identifier);
    buffer.append(")");
    break;
  case TokenType::NEW_LINE:
    buffer.append("\n");
  default:
    break;
  }

  std::cout << buffer << ' ';
}
