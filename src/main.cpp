#include "Lexer.hpp"
#include <iostream>

int main() {
  Lexer lexer;

  lexer.SetSource("\\\\ This is a comment!\n"
                  "main <- proc() int {\n"
                  "\treturn 0\n"
                  "}");

  lexer.GetTokensFromSource();

  for (auto token : lexer.GetTokens()) {
    token.Output();
  }
  std::cout << '\n';

  return 0;
}