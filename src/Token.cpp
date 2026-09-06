#include "Token.hpp"
#include <iostream>

std::string TokenTypeToString(TokenType token) {
  switch (token) {
  case TokenType::END_OF_FILE:
    return "END_OF_FILE";
  case TokenType::NEW_LINE:
    return "NEW_LINE";
  case TokenType::IDENTIFIER:
    return "IDENTIFIER";
  case TokenType::CONST_DEF:
    return "<-";
  case TokenType::PROCEDURE:
    return "proc";
  case TokenType::FUNCTION:
    return "func";
  case TokenType::PERIOD:
    return ".";
  case TokenType::TYPE:
    return "TYPE";
  case TokenType::LEFT_PARENTHESIS:
    return "(";
  case TokenType::RIGHT_PARENTHESIS:
    return ")";
  case TokenType::LEFT_BRACE:
    return "{";
  case TokenType::RIGHT_BRACE:
    return "}";
  case TokenType::COMMENT:
    return "--";
  case TokenType::COLON:
    return ":";
  case TokenType::EQUAL:
    return "=";
  case TokenType::EQUAL_CMP:
    return "==";
  case TokenType::LESS_CMP:
    return "<";
  case TokenType::BIGGER_CMP:
    return ">";
  case TokenType::LESS_EQUAL_CMP:
    return "<=";
  case TokenType::BIGGER_EQUAL_CMP:
    return ">=";
  case TokenType::BANG_EQUAL_CMP:
    return "!=";
  case TokenType::BANG:
    return "!";
  case TokenType::COLON_EQUAL:
    return ":=";
  case TokenType::MINUS:
    return "-";
  case TokenType::PLUS:
    return "+";
  case TokenType::STAR:
    return "*";
  case TokenType::DIVIDE:
    return "/";
  case TokenType::LITERAL:
    return "LITERAL";

  case TokenType::UNKNOWN:
    return "UNKNOWN";
  default:
    break;
  }

  return "NOT_A_TOKEN";
}

std::string DataTypeToString(DataType type) {
  switch (type) {
  case DataType::NONE:
    return "NONE";
  case DataType::INT:
    return "INTEGER";
  case DataType::FLOAT:
    return "FLOAT";
  case DataType::CSTRING:
    return "CSTRING";
  }

  return "NOT_A_TYPE";
}

void Token::Output() {
  std::string buffer;

  buffer.append(TokenTypeToString(token));

  switch (token) {
  case TokenType::COMMENT:
    buffer.append(" (");
    buffer.append(comment);
    buffer.append(")");
    break;

  case TokenType::IDENTIFIER:
    buffer.append("(");
    buffer.append(lexeme);
    buffer.append(")");
    break;
  default:
    break;
  }

  if (type != DataType::NONE) {
    buffer.append("(");
    buffer.append(DataTypeToString(type));
    buffer.append(")");
  }

  std::cout << buffer << (token == TokenType::NEW_LINE ? '\n' : ' ');
}
