#include "Lexer.hpp"
#include <iostream>

int main() {
  Lexer lexer("\\\\ This is a comment!\n"
              "main <- proc() int {\n"
              "\tsome_var := 10\n"
              "\treturn 0\n"
              "}");

  lexer.GetTokensFromSource();

  for (auto token : lexer.GetTokens()) {
    token.Output();
  }
  std::cout << '\n';

  return 0;
}