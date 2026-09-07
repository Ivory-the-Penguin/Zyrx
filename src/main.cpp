#include "Lexer.hpp"
#include <iostream>

int main() {
  Lexer lexer("-- This is a comment!\n"
              "main <- proc() int {\n"
              "  some_var := 123.456;\n"
              "  return 0;\n"
              "}");

  lexer.GetTokensFromSource();

  for (auto token : lexer.GetTokens()) {
    token.Output();
  }
  std::cout << '\n';

  return 0;
}